#include "../Utils/Logging.hpp"

#include "Matrix4.hpp"
#include <stdexcept>
#include <sstream>


namespace WSR
{
	const Matrix4 Matrix4::zero = Matrix4(0);
	const Matrix4 Matrix4::identity = Matrix4(1, 0, 0, 0,
											  0, 1, 0, 0,
											  0, 0, 1, 0,
											  0, 0, 0, 1);

	Matrix4::Matrix4() : Matrix4(0) {}

	Matrix4::Matrix4(float value)
	{
		for(int i = 0; i < 16; i++)
		{
			data[i] = value;
		}
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		for(int i = 0; i < 16; i++)
		{
			data[i] = other.data[i];
		}
	}

	Matrix4::Matrix4(Matrix4&& other) noexcept
	{
		for(int i = 0; i < 16; i++)
		{
			data[i] = other.data[i];
		}
	}

	Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20,
	                 float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	{
		data[0] = m00;
		data[1] = m01;
		data[2] = m02;
		data[3] = m03;
		data[4] = m10;
		data[5] = m11;
		data[6] = m12;
		data[7] = m13;
		data[8] = m20;
		data[9] = m21;
		data[10] = m22;
		data[11] = m23;
		data[12] = m30;
		data[13] = m31;
		data[14] = m32;
		data[15] = m33;
	}

	Matrix4& Matrix4::operator=(const Matrix4& other)
	{
		if(this == &other) return *this;
		
		for(int i = 0; i < 16; i++)
		{
			data[i] = other.data[i];
		}
		return *this;
	}

	Matrix4& Matrix4::operator=(Matrix4&& other) noexcept
	{
		if(this == &other) return *this;
		
		for(int i = 0; i < 16; i++)
		{
			data[i] = other.data[i];
		}
		
		return *this;
	}

	float Matrix4::operator[](int i) const
	{
		if(i < 0 || i > 15) throw std::out_of_range("Matrix4[] index out of range!");
		return data[i];
	}

	bool Matrix4::operator==(const Matrix4& other) const
	{
		for(int i = 0; i < 16; i++)
		{
			if(abs(data[i] - other.data[i]) >= 0.0001f) return false;
		}
		return true;
	}

	bool Matrix4::operator!=(const Matrix4& other) const
	{
		return !(*this == other);
	}

	Matrix4 Matrix4::operator+(const Matrix4& other) const
	{
		Matrix4 mat4;
		for(int i = 0; i < 16; i++)
		{
			mat4.data[i] = data[i] + other.data[i];
		}
		return mat4;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& other)
	{
		*this = *this + other;
		return *this;
	}

	Matrix4 Matrix4::operator-(const Matrix4& other) const
	{
		Matrix4 mat4;
		for(int i = 0; i < 16; i++)
		{
			mat4.data[i] = data[i] - other.data[i];
		}
		return mat4;
	}

	Matrix4& Matrix4::operator-=(const Matrix4& other)
	{
		*this = *this - other;
		return *this;
	}

	Matrix4 Matrix4::operator*(const Matrix4& other) const
	{
		Matrix4 mat4;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				mat4.row_col[i][j] = 0;
				
				for(int k = 0; k < 4; k++)
				{
					mat4.row_col[i][j] += row_col[i][k] * other.row_col[k][j];
				}
			}
		}
		return mat4;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& other)
	{
		*this = *this * other;
		return *this;
	}

	Matrix4 Matrix4::operator*(float scalar) const
	{
		Matrix4 mat4;
		for(int i = 0; i < 16; i++)
		{
			mat4.data[i] = data[i] * scalar;
		}
		return mat4;
	}

	Matrix4& Matrix4::operator*=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	float Matrix4::Determinant() const
	{
		const float m00 = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] -
			data[9] * data[6] * data[15] + data[9] * data[7] * data[14] +
			data[13] * data[6] * data[11] - data[13] * data[7] * data[10];

		const float m10 = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] +
			data[8] * data[6] * data[15] - data[8] * data[7] * data[14] -
			data[12] * data[6] * data[11] + data[12] * data[7] * data[10];

		const float m20 = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] -
			data[8] * data[5] * data[15] + data[8] * data[7] * data[13] +
			data[12] * data[5] * data[11] - data[12] * data[7] * data[9];

		const float m30 = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] +
			data[8] * data[5] * data[14] - data[8] * data[6] * data[13] -
			data[12] * data[5] * data[10] + data[12] * data[6] * data[9];

		return data[0] * m00 + data[1] * m10 + data[2] * m20 + data[3] * m30;
	}

	Matrix4 Matrix4::Inverse() const
	{
		const float m00 = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] -
			data[9] * data[6] * data[15] + data[9] * data[7] * data[14] +
			data[13] * data[6] * data[11] - data[13] * data[7] * data[10];

		const float m10 = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] +
			data[8] * data[6] * data[15] - data[8] * data[7] * data[14] -
			data[12] * data[6] * data[11] + data[12] * data[7] * data[10];

		const float m20 = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] -
			data[8] * data[5] * data[15] + data[8] * data[7] * data[13] +
			data[12] * data[5] * data[11] - data[12] * data[7] * data[9];

		const float m30 = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] +
			data[8] * data[5] * data[14] - data[8] * data[6] * data[13] -
			data[12] * data[5] * data[10] + data[12] * data[6] * data[9];

		const float m01 = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] +
			data[9] * data[2] * data[15] - data[9] * data[3] * data[14] -
			data[13] * data[2] * data[11] + data[13] * data[3] * data[10];

		const float m11 = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] -
			data[8] * data[2] * data[15] + data[8] * data[3] * data[14] +
			data[12] * data[2] * data[11] - data[12] * data[3] * data[10];

		const float m21 = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] +
			data[8] * data[1] * data[15] - data[8] * data[3] * data[13] -
			data[12] * data[1] * data[11] + data[12] * data[3] * data[9];

		const float m31 = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] -
			data[8] * data[1] * data[14] + data[8] * data[2] * data[13] +
			data[12] * data[1] * data[10] - data[12] * data[2] * data[9];

		const float m02 = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] -
			data[5] * data[2] * data[15] + data[5] * data[3] * data[14] +
			data[13] * data[2] * data[7] - data[13] * data[3] * data[6];

		const float m12 = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] +
			data[4] * data[2] * data[15] - data[4] * data[3] * data[14] -
			data[12] * data[2] * data[7] + data[12] * data[3] * data[6];

		const float m22 = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] -
			data[4] * data[1] * data[15] + data[4] * data[3] * data[13] +
			data[12] * data[1] * data[7] - data[12] * data[3] * data[5];

		const float m32 = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] +
			data[4] * data[1] * data[14] - data[4] * data[2] * data[13] -
			data[12] * data[1] * data[6] + data[12] * data[2] * data[5];

		const float m03 = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] +
			data[5] * data[2] * data[11] - data[5] * data[3] * data[10] -
			data[9] * data[2] * data[7] + data[9] * data[3] * data[6];

		const float m13 = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] -
			data[4] * data[2] * data[11] + data[4] * data[3] * data[10] +
			data[8] * data[2] * data[7] - data[8] * data[3] * data[6];

		const float m23 = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] +
			data[4] * data[1] * data[11] - data[4] * data[3] * data[9] -
			data[8] * data[1] * data[7] + data[8] * data[3] * data[5];

		const float m33 = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] -
			data[4] * data[1] * data[10] + data[4] * data[2] * data[9] +
			data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

		const float det = data[0] * m00 + data[1] * m10 + data[2] * m20 + data[3] * m30;

		if(abs(det) < 0.0001f)
		{
			Logging::Warning("Matrix4 - Cannot do inverse when the determinant is zero.");
			return Matrix4::zero;
		}

		const Matrix4 ret{m00, m01, m02, m03, m10, m11, m12, m13,
						  m20, m21, m22, m23, m30, m31, m32, m33};

		return ret * (1.0f / det);
	}

	Matrix4 Matrix4::Transpose() const
	{
		Matrix4 mat4(*this);
		std::swap(mat4.data[1], mat4.data[4]);
		std::swap(mat4.data[2], mat4.data[8]);
		std::swap(mat4.data[3], mat4.data[12]);
		std::swap(mat4.data[6], mat4.data[9]);
		std::swap(mat4.data[7], mat4.data[13]);
		std::swap(mat4.data[11], mat4.data[14]);
		return mat4;
	}

	bool Matrix4::IsIdentity() const
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(i == j)
				{
					if(abs(row_col[i][j] - 1) >= 0.0001f) return false;
				}
				else
				{
					if(abs(row_col[i][j]) >= 0.0001f) return false;
				}
			}
		}
		return true;
	}

	bool Matrix4::IsZero() const
	{
		for(int i = 0; i < 16; i++)
		{
			if(abs(data[i]) > 0.0001f) return false;
		}
		return true;
	}

	Matrix4 Matrix4::Translate(const Vector3& translation)
	{
		return {
			1, 0, 0, translation.x,
					   0, 1, 0, translation.y,
					   0, 0, 1, translation.z,
					   0, 0, 0, 1
		};
	}

	Matrix4 RotateX(float value)
	{
		float cos = std::cos(value);
		float sin = std::sin(value);
		return {
			1,	0,		0,		0,
			0,	cos,	sin,	0,
			0, -sin,	cos,	0,
			0,	0,		0,		1
		};
	}

	Matrix4 RotateY(float value)
	{
		float cos = std::cos(value);
		float sin = std::sin(value);
		return {
			cos,	0,	-sin,	0,
			0,		1,	0,		0,
			sin,	0,	cos,	0,
			0,		0,	0,		1
		};
	}

	Matrix4 RotateZ(float value)
	{
		float cos = std::cos(value);
		float sin = std::sin(value);
		return {
			cos,	sin,	0,	0,
			-sin,	cos,	0,	0,
			0,		0,		1,	0,
			0,		0,		0,	1
		};
	}

	Matrix4 Matrix4::Rotate(const Vector3& rotation)
	{
		return RotateX(rotation.x) * RotateY(rotation.y) * RotateZ(rotation.z);
	}

	Matrix4 Matrix4::Scale(const Vector3& scale)
	{
		return {
			scale.x,	0,			0,			0,
			0,			scale.y,	0,			0,
			0,			0,			scale.z,	0,
			0,			0,			0,			1
		};
	}

	Matrix4 Matrix4::Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale)
	{
		return Translate(translation) * Rotate(rotation) * Scale(scale);
	}

	Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
	{
		const Vector3 f((at - eye).Normalized());
		Vector3 u = up.Normalized();
		Vector3 s(Vector3::Cross(f, u).Normalized());
		u = Vector3::Cross(s, f);

		return {
			s.x,	u.x,	-f.x,	-Vector3::Dot(s, eye),
			s.y,	u.y,	-f.y,	-Vector3::Dot(u, eye),
			s.z,	u.z,	-f.z,	Vector3::Dot(f, eye),
			0,		0,		0,		1
		};
	}

	std::string Matrix4::ToString() const
	{
		std::stringstream ss;
		ss << data[0] << ' ' << data[1] << ' ' << data[2] << ' ' << data[3] << '\n' << data[4] << ' ' << data[5] << ' ' << data[6] << ' ' << data[7] << '\n' << data[8] << ' ' << data[9] << ' ' << data[10] << ' ' << data[11] << '\n' << data[12] << ' ' << data[13] << ' ' << data[14] << ' ' << data[15] << '\n';
		return ss.str();
	}
}