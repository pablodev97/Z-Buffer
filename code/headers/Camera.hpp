#pragma once
#include "Point.hpp"
#include "Transform.hpp"

using namespace toolkit;

namespace example
{
	class Camera
	{
		Transform3f transform;

		Projection3f projection;

	public:

		Camera();

		inline void set_transform(Transform3f transform) { this->transform = transform; }

		inline void set_projection(Projection3f projection) { this->projection = projection; }

		Projection3f get_projection();

		Matrix44f get_inverse();

		void update();
	};
}