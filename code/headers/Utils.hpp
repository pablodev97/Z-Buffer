/*
 * File: Utils.hpp
 * File Created: 22nd February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#pragma once

#include <Point.hpp>

namespace example
{
	using toolkit::Point4i;

	class Utils
	{
	public:
		/**
		@brief Clipps the polygon whether its inside the window or not
		*/
		int polygon_clipper(const Point4i* vertices, int* first, const int* last, const int viewport_width, const int viewport_height, Point4i* clipped_vertices);

	};
}