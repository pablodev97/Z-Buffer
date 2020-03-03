/*
 * File: Light.hpp
 * File Created: 8th February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#pragma once

#include <Point.hpp>
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace example
{

	using namespace toolkit;

	/**
	@brief Class that simulates a light
	*/
	class Light
	{
	private:

		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;

	private:
		/**
		@brief Position of the light
		*/
		Point3f position;
		/**
		@brief Color of the light
		*/
		Color color;
	
	public:

		/**
		@brief Constructor of the light
		@param position Light's position
		@param color Light's color
		*/
		Light(Point3f position, Color color) :
			position(position), color(color)
		{}

		/**
		@brief Returns the light position
		*/
		Point3f get_position() { return position; }

		/**
		@brief Returns the light color
		*/
		Color get_light_color() { return color; }

	};
}