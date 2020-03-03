/*
 * File: Light.hpp
 * File Created: 24th February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#pragma once
#include <vector>
#include <Point.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>

namespace example
{
	using std::vector;
	using toolkit::Point4i;
	using toolkit::Point4f;
	using namespace toolkit;
	
	/**
	@brief Struct that contains the transform information
	*/
	struct Transform3f
	{
	public:
		/**
		@brief Enum to know which pivot is selected
		*/
		enum PivotRotation { X, Y, Z };

	private:
		/**
		@brief Original coords of the model
		*/
		Translation3f translation_original;
		/**
		@brief Transformed coords of the model
		*/
		Translation3f translation_transformed;
		/**
		@brief New pivot's coord to rotate the model
		*/
		Translation3f rotation_pivot;
		/**
		@brief Value of the rotation model in radians
		*/
		Rotation3f	  rotation_x;
		Rotation3f	  rotation_y;
		Rotation3f	  rotation_z;
		/**
		@brief Scale of the model
		*/
		Scaling3f	  scale;
		/**
		@brief Pivot of rotation selected
		*/
		PivotRotation pivot;
		/**
		@brief Actual angle of the rotation
		*/
		float		  angle;
		/**
		@brief Angle fixed 
		*/
		float		  angle_fixed;

	public:

		/**
		@brief Translates the model to the param value
		@param translation Translation value 
		*/
		void set_translation(Translation3f translation);
		

		/**
		@brief Sets the new pivot of rotation coords
		@param rotation_pivot New pivot coords
		*/
		void set_rotation_pivot(Translation3f rotation_pivot);

		/**
		@brief Sets the model rotation
		@param rotation_xyz Vector with the three values of each dimension
		*/
		void set_rotation(vector<float> rotation_xyz);

		/**
		@brief Sets the model scale
		@param scale Value of the scale
		*/
		inline void set_scale(Scaling3f scale) 
		{ 
			this->scale = scale; 
		}

		/**
		@brief Sets the pivot selected to rotate
		@param pivot Pivot of rotation
		*/
		inline void set_pivot(PivotRotation pivot)
		{
			this->pivot = pivot;
		}

		/**
		@brief Returns the matrix formed by the translation, the rotation and the scale
		*/
		inline Matrix44f get_matrix_transform()
		{
			return translation_transformed * rotation_x * rotation_y * rotation_z * scale;
		}

		/**
		@brief Updates the rotation
		@param speed_rotation Angle of rotation in radians added every frame
		*/
		void update(float speed_rotation);

		Matrix44f get_matrix_inverse();
	};

	
}