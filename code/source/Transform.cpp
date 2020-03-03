#include "Transform.hpp"

namespace example
{
	void Transform3f::set_translation(Translation3f translation)
	{
		this->translation_original = translation;
		this->rotation_pivot = { 0.f, 0.f, 0.f };
	}

	void Transform3f::set_rotation_pivot(Translation3f rotation_pivot)
	{
		this->rotation_pivot = rotation_pivot;
	}

	void Transform3f::set_rotation(vector<float> rotation_xyz)
	{
		switch (pivot)
		{
		case X:
			angle = rotation_xyz[0];
			break;
		case Y:
			angle = rotation_xyz[1];
			break;
		case Z:
			angle = rotation_xyz[2];
			break;
		}

		angle_fixed = 0.f;

		rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(rotation_xyz[0]);
		rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(rotation_xyz[1]);
		rotation_z.set< Rotation3f::AROUND_THE_Z_AXIS >(rotation_xyz[2]);
	}

	void Transform3f::update(float speed_rotation)
	{
		angle += speed_rotation;
		angle_fixed += speed_rotation;

		switch (pivot)
		{
		case X:
			rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(angle);

			translation_transformed[0] = translation_original[0] + rotation_pivot[0] * cos(angle_fixed);
			translation_transformed[1] = translation_original[1] - rotation_pivot[0] * sin(angle_fixed);
			translation_transformed[2] = translation_original[2] + rotation_pivot[0] * sin(angle_fixed);

			break;
		case Y:
			rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);

			translation_transformed[0] = translation_original[0] - rotation_pivot[1] * sin(angle_fixed);
			translation_transformed[1] = translation_original[1] + rotation_pivot[1] * cos(angle_fixed);
			translation_transformed[2] = translation_original[2] + rotation_pivot[1] * cos(angle_fixed);

			break;
		case Z:
			rotation_z.set< Rotation3f::AROUND_THE_Z_AXIS >(angle);

			translation_transformed[0] = translation_original[0] - rotation_pivot[1] * sin(angle_fixed);
			translation_transformed[1] = translation_original[1] - rotation_pivot[1] * cos(angle_fixed);
			translation_transformed[2] = translation_original[2] + rotation_pivot[1] * sin(angle_fixed);

			break;
		}
	}
	Matrix44f Transform3f::get_matrix_inverse()
	{
		Matrix44f transform = translation_original * rotation_x * rotation_y * rotation_z;
		Matrix44f invOut = transform;

		float m[16] = { transform[0][0], transform[0][1], transform[0][2], transform[0][3],
						transform[1][0], transform[1][1], transform[1][2], transform[1][3],
						transform[2][0], transform[2][1], transform[2][2], transform[2][3],
						transform[3][0], transform[3][1], transform[3][2], transform[3][3] };

		float inv[16], det;

		inv[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			//return false;

		det = 1.0 / det;

		invOut[0][0] = inv[0] * det;
		invOut[0][1] = inv[1] * det;
		invOut[0][2] = inv[2] * det;
		invOut[0][3] = inv[3] * det;
		invOut[1][0] = inv[4] * det;
		invOut[1][1] = inv[5] * det;
		invOut[1][2] = inv[6] * det;
		invOut[1][3] = inv[7] * det;
		invOut[2][0] = inv[8] * det;
		invOut[2][1] = inv[9] * det;
		invOut[2][2] = inv[10] * det;
		invOut[2][3] = inv[11] * det;
		invOut[3][0] = inv[12] * det;
		invOut[3][1] = inv[13] * det;
		invOut[3][2] = inv[14] * det;
		invOut[3][3] = inv[15] * det;

		return invOut;
	}
}