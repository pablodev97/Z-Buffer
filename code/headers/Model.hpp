/*
 * File: Model.hpp
 * File Created: 2nd February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <vector>
#include <string>
#include <Point.hpp>
#include "Rasterizer.hpp"
#include "Light.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>
#include "Transform.hpp"
#include <Shared_Ptr.hpp>
#include "Mesh.hpp"
#include "Material.hpp"

namespace example
{

	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;
	using namespace toolkit;

	class Model
	{
	private:

		typedef Color_Buffer_Rgba8888					Color_Buffer;
		typedef Color_Buffer::Color						Color;
		typedef Point4f									Vertex;
		typedef vector< Vertex   >						Vertex_Buffer;
		typedef vector< Color    >						Vertex_Colors;
		typedef vector< int		 >						Index_Buffer;
		typedef vector< std::shared_ptr< Mesh >	 >		Mesh_Buffer;
		typedef vector< std::shared_ptr< Material >	 >	Material_Buffer;

	private:

		/**
		@brief All the vertices of the model
		*/
		Vertex_Buffer     original_vertices;
		/**
		@brief All the normals of the model
		*/
		Vertex_Buffer     original_normals;
		/**
		@brief ALl the colors of the model
		*/
		Vertex_Colors     original_colors;
		/**
		@brief Mesh buffer 
		*/
		Mesh_Buffer		  mesh_buffer;
		Material_Buffer	  material_buffer;
		Vertex_Buffer     transformed_vertices;
		Vertex_Buffer     transformed_normals;
		Vertex_Colors     transformed_colors;
		vector<Point4i>	  display_vertices;
		
		Transform3f				transform;
		std::shared_ptr<Model>	parent;
		std::string				error;

		float speed_rotation;

	public:

		Model(const std::string& path, vector<Color> color);
		
		void update(Matrix44f inverse_camera, Projection3f projection, std::shared_ptr<Light> light);
		
		void render(Rasterizer<Color_Buffer_Rgba8888>& rasterizer);
		
		float angleBwPoints(const Vertex& V1, const Vertex& V2);

		inline void set_parent(std::shared_ptr<Model> parent) { this->parent = parent; }

		inline void set_speed_rotation(float speed_rotation) { this->speed_rotation = speed_rotation; }
		
		inline Matrix44f get_matrix_transform()
		{
			if (parent)
				return parent->get_matrix_transform() * transform.get_matrix_transform();
			else
				return transform.get_matrix_transform();
		}

		inline Transform3f & get_transform()
		{
			return transform;
		}

		inline const Vertex_Buffer& get_transformed_vertices() const
		{
			return transformed_vertices;
		}

		inline const vector<Point4i>& get_display_vertices() const
		{
			return display_vertices;
		}

		inline const Vertex_Colors& get_transformed_colors() const
		{
			return transformed_colors;
		}
	};

}

#endif