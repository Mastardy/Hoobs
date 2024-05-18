#pragma once

#include <array>

#include "Vector3.hpp"

namespace WSR
{
	class Matrix4
	{
	public:
		static const Matrix4 zero;
		static const Matrix4 identity;

		union
		{
			std::array<float, 16> data;
			std::array<std::array<float, 4>, 4> row_col;
		};

		Matrix4();
		Matrix4(float value);
		Matrix4(const Matrix4& other);
		Matrix4(Matrix4&& other) noexcept;
		Matrix4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
		~Matrix4() = default;

		Matrix4& operator=(const Matrix4& other);
		Matrix4& operator=(Matrix4&& other) noexcept;
		float operator[](int i) const;

		bool operator==(const Matrix4& other) const;
		bool operator!=(const Matrix4& other) const;

		Matrix4 operator+(const Matrix4& other) const;
		Matrix4& operator+=(const Matrix4& other);
		Matrix4 operator-(const Matrix4& other) const;
		Matrix4& operator-=(const Matrix4& other);
		Matrix4 operator*(const Matrix4& other) const;
		Matrix4& operator*=(const Matrix4& other);
		Matrix4 operator*(float scalar) const;
		Matrix4& operator*=(float scalar);

		float Determinant() const;
		Matrix4 Inverse() const;
		Matrix4 Transpose() const;
		bool IsIdentity() const;
		bool IsZero() const;

		static Matrix4 Translate(const Vector3& translation);
		static Matrix4 Rotate(const Vector3& rotation);
		static Matrix4 Scale(const Vector3& scale);
		static Matrix4 Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale);

		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		std::string ToString() const;
	};
}