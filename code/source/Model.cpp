#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include "Model.hpp"
#include "tiny_obj_loader.h"
#include <cassert>
#include <iostream> 
#include <algorithm> 
#include <math.h>
#include <ctime>
#include "Utils.hpp"

namespace example
{
#define PI 3.14159265

	Model::Model(const std::string& path, vector<Color> colors) : speed_rotation(0.f)
	{
		// Carga de un obj
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;

		// Comprobamos que el obj es accesible
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, path.c_str()))
		{
			return;
		}
		// Comprobamos que tenga vertices y shapes
		if (attrib.vertices.size() == 0 || attrib.normals.size() == 0 || shapes.size() == 0)
		{
			return;
		}

		size_t number_of_indices = 0;
		vector<size_t> number_of_indeces_per_mesh;

		number_of_indeces_per_mesh.resize(shapes.size());

		for (size_t s = 0; s < shapes.size(); s++) {
			number_of_indices += shapes[s].mesh.indices.size();
			number_of_indeces_per_mesh[s] = shapes[s].mesh.indices.size();
		}

		original_vertices.resize(number_of_indices);
		original_normals.resize(number_of_indices);
		original_colors.resize(number_of_indices);

		// vueltas por lado (cuadrado)
		for (size_t s = 0; s < shapes.size(); s++) {
			size_t index_offset = 0;

			int fv = shapes[s].mesh.indices.size();

			Index_Buffer	  original_indices;
			original_indices.resize(number_of_indeces_per_mesh[s]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

				original_vertices[idx.vertex_index] = Vertex({ vx, vy, vz, 1.f });
				original_normals [idx.vertex_index] = Vertex({ nx, ny, nz, 0.f });
				original_colors[idx.vertex_index] = colors[s];

				original_indices[v] = (idx.vertex_index);
			}

			std::shared_ptr<Mesh> mesh(std::make_shared<Mesh>(*this, original_indices));

			mesh_buffer.push_back(mesh);

			index_offset += fv;
		}

		transformed_vertices.resize(original_vertices.size());
		transformed_normals. resize(original_normals.size());
		transformed_colors.  resize(original_vertices.size());
		display_vertices.    resize(original_vertices.size());
	}

	void Model::update(Matrix44f inverse_camera, Projection3f projection, std::shared_ptr<Light> light)
	{
		// Se actualizan los parámetros de transformatión (sólo se modifica el ángulo)

		transform.update(speed_rotation);
		
		Point3f pos = light->get_position();

		Vertex light_vector({ pos[0], pos[1], pos[2] });

		// Se crean las matrices de transformación:
		Matrix44f transform = inverse_camera * get_matrix_transform();
		Matrix44f transform_normal =  inverse_camera * Matrix44f(projection) * get_matrix_transform();

		// Calcular la iluminación
		for (size_t index = 0, number_of_vertices = original_normals.size(); index < number_of_vertices; index++)
		{
			// Se transforman todos los vértices usando la matriz de transformación resultante:
			Vertex& vertex   = transformed_vertices[index] = transform * Matrix41f(original_vertices[index]);
			Vertex& vertex_n = transformed_normals [index] = transform_normal * Matrix41f(original_normals[index]);

			float angle = angleBwPoints(light_vector, vertex_n);

			// Calculando el coseno del ángulo creamos una intensidad.
			// La intensidad se mueve del 0 al 1.
			float cosTheta = cos(angle);

			if (cosTheta < 0.f) cosTheta *= -1.f;

			float intensity = 1.f - cosTheta;

			if (intensity > 1.f) intensity = 1.f;

			Color color = original_colors[index];
			
			color.data.component.r = ((int)(color.data.component.r * light->get_light_color().data.component.r)) >> 8;
			color.data.component.g = ((int)(color.data.component.g * light->get_light_color().data.component.g)) >> 8;
			color.data.component.b = ((int)(color.data.component.b * light->get_light_color().data.component.b)) >> 8;

			color.data.component.r *= intensity;
			color.data.component.g *= intensity;
			color.data.component.b *= intensity;

			transformed_colors[index] = color;

			float divisor = 1.f / vertex_n[3];

			vertex_n[0] *= divisor;
			vertex_n[1] *= divisor;
			vertex_n[2] *= divisor;
			vertex_n[3] = 1.f;
		}
	}

	void Model::render(Rasterizer<Color_Buffer_Rgba8888>& rasterizer)
	{
		// Se convierten las coordenadas transformadas y proyectadas a coordenadas
		// de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
		// La coordenada Z se escala a un valor suficientemente grande dentro del
		// rango de int (que es lo que espera fill_convex_polygon_z_buffer).
		Scaling3f scaling = Scaling3f(float(rasterizer.get_color_buffer().get_width() / 2), float(rasterizer.get_color_buffer().get_height() / 2), 100000000.f);
		Translation3f translation = Translation3f(float(rasterizer.get_color_buffer().get_width() / 2), float(rasterizer.get_color_buffer().get_height() / 2), 0.f);

		Transformation3f transformation = translation * scaling;

		for (size_t index = 0, number_of_vertices = transformed_vertices.size(); index < number_of_vertices; index++)
		{
			display_vertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(transformed_vertices[index]));
		}

		Vertex_Buffer clipped_vertices;

		for (std::shared_ptr<Mesh> mesh : mesh_buffer)
		{
			mesh->render(rasterizer);
		}
	}

	float Model::angleBwPoints(const Vertex& V1, const Vertex& V2)
	{
		float length1 = std::sqrt(std::pow(V1[0], 2) + std::pow(V1[1], 2) + std::pow(V1[2], 2));
		float length2 = std::sqrt(std::pow(V2[0], 2) + std::pow(V2[1], 2) + std::pow(V2[2], 2));

		Vertex normalized_V1 = Vertex({ V1[0] / length1, V1[1] / length1, V1[2] / length1 });
		Vertex normalized_V2 = Vertex({ V2[0] / length2, V2[1] / length2, V2[2] / length2 });

		float dot = normalized_V1[0] * normalized_V2[0] +
					normalized_V1[1] * normalized_V2[1] +
					normalized_V1[2] * normalized_V2[2];

		float length11 = std::sqrt(std::pow(normalized_V1[0], 2) + std::pow(normalized_V1[1], 2) + std::pow(normalized_V1[2], 2));
		float length22 = std::sqrt(std::pow(normalized_V2[0], 2) + std::pow(normalized_V2[1], 2) + std::pow(normalized_V2[2], 2));

		float a = dot / (length11 * length22);

		return acos(a);
	}

}