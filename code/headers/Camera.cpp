#include "Camera.hpp"

namespace example
{
	Camera::Camera() : transform({}), projection({ 0, 0, 0, 0 })
	{
	}

	Projection3f Camera::get_projection()
	{
		return projection;
	}

	Matrix44f Camera::get_inverse()
	{
		return transform.get_matrix_inverse();
	}

	void Camera::update()
	{
		static float z = 0.2f;
		float zoom = -0.0003f;

		if (z > 0.23f)
		{
			zoom *= -1.f;
			z += zoom;
		}
			
		if (z < 0.18f)
		{
			zoom *= -1.f;
			z += zoom;
		}

		z += zoom;

		transform.set_rotation({ z, 0, 0 });
	}

}