#pragma once

#include <string>
#include <array>

namespace WSR
{
    class Vector2
    {
    public:
        static const Vector2 zero;
        static const Vector2 one;

        static const Vector2 up;
        static const Vector2 down;
        static const Vector2 left;
        static const Vector2 right;

        union
        {
            struct
            {
                float x;
                float y;
            };

            std::array<float, 2> xy;
        };

        Vector2();
        Vector2(float value);
        Vector2(float x, float y);
        Vector2(const Vector2& other);
        Vector2(Vector2&& other) noexcept;

        ~Vector2() = default;
        
        Vector2& operator=(const Vector2& other);
        Vector2& operator=(Vector2&& other) noexcept;
        float operator[](int i) const;

        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;

        Vector2 operator+(const Vector2& other) const;
        Vector2& operator+=(const Vector2& other);

        Vector2 operator-(const Vector2& other) const;
        Vector2& operator-=(const Vector2& other);

        Vector2 operator*(float scalar) const;
        Vector2& operator*=(float scalar);

        Vector2 operator/(float scalar) const;
        Vector2& operator/=(float scalar);

        bool Equals(const Vector2& other) const;
        float Magnitude() const;
        float SqrMagnitude() const;
        Vector2 Normalized() const;
        void Normalize();

        static float Angle(const Vector2& leftHandSide, const Vector2& rightHandSide);
        static float Cross(const Vector2& leftHandSide, const Vector2& rightHandSide);
        static float Distance(const Vector2& leftHandSide, const Vector2& rightHandSide);
        static float Dot(const Vector2& leftHandSide, const Vector2& rightHandSide);
        static Vector2 Lerp(const Vector2& start, const Vector2& end, float time);
        static Vector2 Scale(const Vector2& inVector, const Vector2& scalarVector);
        static Vector2 Slerp(const Vector2& start, const Vector2& end, float time);

        std::string ToString() const;
    };
}
