#include "Mesh.hpp"
#include "Model.hpp"

namespace example
{
	Mesh::Mesh(Model& owner, const Index_Buffer& indices) :
		indices(indices),
		owner(owner)
	{
		
	}

	void Mesh::render(Rasterizer<Color_Buffer_Rgba8888>& rasterizer)
	{
		
		auto& display_vertices = owner.get_display_vertices();

		for (int* i = indices.data(), *end = i + indices.size(); i < end; i += 3)
		{
			if (!is_frontface(owner.get_transformed_vertices().data(), i))
			{
				// The color is established from the media of the 3 vertices:

				Color color_mixed = {
					(owner.get_transformed_colors()[*i].data.component.r +
					 owner.get_transformed_colors()[*i + 1].data.component.r +
					 owner.get_transformed_colors()[*i + 2].data.component.r) / 3,
					(owner.get_transformed_colors()[*i].data.component.g +
					 owner.get_transformed_colors()[*i + 1].data.component.g +
					 owner.get_transformed_colors()[*i + 2].data.component.g) / 3,
					(owner.get_transformed_colors()[*i].data.component.b +
					 owner.get_transformed_colors()[*i + 1].data.component.b +
					 owner.get_transformed_colors()[*i + 2].data.component.b) / 3,
					1				
				};

				
				rasterizer.set_color(color_mixed);

				// The polygon is filled:

				rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), i, i + 3);
			}
		}

		// The hidden framebuffer is copied in the window framebuffer:

		rasterizer.get_color_buffer().gl_draw_pixels(0, 0);
	}

	bool Mesh::is_frontface(const Vertex* const projected_vertices, const int* const indices)
	{
		const Vertex& v0 = projected_vertices[indices[0]];
		const Vertex& v1 = projected_vertices[indices[1]];
		const Vertex& v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}
}
