#pragma once

#include <array>

namespace WSR
{
    class Matrix3
    {
    public:
        static const Matrix3 zero;
        static const Matrix3 identity;

        union
        {
            std::array<float, 9> data;
            std::array<std::array<float, 3>, 3> row_col{};
        };

        Matrix3();
        Matrix3(float value);
        Matrix3(const Matrix3& other);
        Matrix3(Matrix3&& other) noexcept;
        Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);

        Matrix3& operator=(const Matrix3& other);
        Matrix3& operator=(Matrix3&& other) noexcept;

        ~Matrix3() = default;

        float operator[](int i) const;

        bool operator==(const Matrix3& other) const;
        bool operator!=(const Matrix3& other) const;

        Matrix3 operator+(const Matrix3& other) const;
        Matrix3& operator+=(const Matrix3& other);
        Matrix3 operator-(const Matrix3& other) const;
        Matrix3& operator-=(const Matrix3& other);
        Matrix3 operator*(const Matrix3& other) const;
        Matrix3& operator*=(const Matrix3& other);
        Matrix3 operator*(float scalar) const;
        Matrix3& operator*=(float scalar);

        float Determinant() const;
        Matrix3 Transpose() const;
        Matrix3 Inverse() const;
        bool IsIdentity() const;
        bool IsZero() const;

        static float Matrix2Determinant(float a, float b, float c, float d);
    };
}
