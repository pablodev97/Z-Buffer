/*
 * File: Mesh.hpp
 * File Created: 5th February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#ifndef MESH_HEADER
#define MESH_HEADER

#include <vector>
#include <Point.hpp>
#include "Rasterizer.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Material.hpp"

namespace example
{
	class Model;
	using namespace toolkit;

	/**
	@brief Class that manages the render of the model
	*/
	class Mesh
	{
	private:

		typedef Point4f						Vertex;
		typedef std::vector< Vertex   >		Vertex_Buffer;
		typedef std::vector< int    >		Index_Buffer;
		typedef Color_Buffer_Rgba8888		Color_Buffer;
		typedef Color_Buffer::Color			Color;

	private:

		/**
		@brief Model indices to acces the owner values
		*/
		Index_Buffer	indices;
		/**
		@brief Model that owns this mesh
		*/
		Model&			owner;

	public:
		
		/**
		@brief Constructor of the mesh
		@param owner Model that owns this mesh
		@param indices Model indices to acces the owner values
		*/
		Mesh(Model& owner, const Index_Buffer& indices);
		
		/**
		@brief Method that renders the mesh
		@param rasterizer 
		*/
		void render(Rasterizer<Color_Buffer_Rgba8888>& rasterizer);

		/**
		@brief Checks if the vertex is front face to the camera
		@param projected_vertices Vertices to check
		@param indices Index to access the vertices
		*/
		bool is_frontface(const Vertex* const projected_vertices, const int* const indices);
	};
}

#endif