#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include <vector>
#include <Point.hpp>
#include "Rasterizer.hpp"
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace example
{

	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;

	class Material
	{
	private:

		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;
		typedef vector< Color  >      Vertex_Colors;

	private:

		Vertex_Colors	original_colors;
		Vertex_Colors   transformed_colors;
		float			kd;

	public:

		Material(Vertex_Colors colors, float kd) : original_colors(colors), kd(kd) {}

		Vertex_Colors get_transformed_colors() { return transformed_colors; }
	};

}

#endif