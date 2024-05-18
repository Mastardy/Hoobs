#include "Vector2.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <sstream>

namespace WSR
{
    //
    // Constants
    //

    const Vector2 Vector2::zero = Vector2();
    const Vector2 Vector2::one = Vector2(1.0f);

    const Vector2 Vector2::right = Vector2(1.0f, 0.0f);
    const Vector2 Vector2::up = Vector2(0.0f, 1.0f);
    const Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
    const Vector2 Vector2::down = Vector2(0.0f, -1.0f);

    //
    // Constructors
    //

    Vector2::Vector2() : x{0}, y{0}
    {
    }

    Vector2::Vector2(float value) : x{value}, y{value}
    {
    }

    Vector2::Vector2(float x, float y) : x{x}, y{y}
    {
    }

    Vector2::Vector2(const Vector2& other) : x{other.x}, y{other.y}
    {
    }

    Vector2::Vector2(Vector2&& other) noexcept : x{other.x}, y{other.y}
    {
    }

    // Operators
    //

    Vector2& Vector2::operator=(const Vector2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vector2& Vector2::operator=(Vector2&& other) noexcept
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    float Vector2::operator[](int i) const
    {
        if (i != 0 && i != 1)
        {
            throw std::out_of_range("Vector2[] - Index out of range.");
        }

        return xy[i];
    }

    bool Vector2::operator==(const Vector2& other) const
    {
        return abs(x - other.x) < 0.0001f && abs(y - other.y) < 0.0001f;
    }

    bool Vector2::operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    Vector2 Vector2::operator+(const Vector2& other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2& Vector2::operator+=(const Vector2& other)
    {
        *this = *this + other;
        return *this;
    }

    Vector2 Vector2::operator-(const Vector2& other) const
    {
        return {x - other.x, y - other.y};
    }

    Vector2& Vector2::operator-=(const Vector2& other)
    {
        *this = *this - other;
        return *this;
    }

    Vector2 Vector2::operator*(float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    Vector2& Vector2::operator*=(float scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    Vector2 Vector2::operator/(float scalar) const
    {
        return {x / scalar, y / scalar};
    }

    Vector2& Vector2::operator/=(float scalar)
    {
        *this = *this / scalar;
        return *this;
    }

    //
    // Member Methods
    //

    bool Vector2::Equals(const Vector2& other) const
    {
        return abs(other.x - x) < FLT_EPSILON && abs(other.y - y) < FLT_EPSILON;
    }

    float Vector2::Magnitude() const
    {
        return sqrtf(SqrMagnitude());
    }

    float Vector2::SqrMagnitude() const
    {
        return x * x + y * y;
    }

    Vector2 Vector2::Normalized() const
    {
        const float magnitude = Magnitude();
        if (abs(magnitude) < 0.0001f) return zero;
        return Vector2(*this / magnitude);
    }

    void Vector2::Normalize()
    {
        const auto normalized = this->Normalized();
        x = normalized.x;
        y = normalized.y;
    }

    //
    // Static Methods
    //

    float Vector2::Angle(const Vector2& leftHandSide, const Vector2& rightHandSide)
    {
        auto dot = Dot(leftHandSide.Normalized(), rightHandSide.Normalized());
        dot = dot < -1.0f ? -1.0f : (dot > 1.0f ? 1.0f : dot);
        return acosf(dot);
    }

    float Vector2::Cross(const Vector2& leftHandSide, const Vector2& rightHandSide)
    {
        return leftHandSide.x * rightHandSide.y - leftHandSide.y * rightHandSide.x;
    }

    float Vector2::Distance(const Vector2& leftHandSide, const Vector2& rightHandSide)
    {
        return (leftHandSide - rightHandSide).Magnitude();
    }

    float Vector2::Dot(const Vector2& leftHandSide, const Vector2& rightHandSide)
    {
        return leftHandSide.x * rightHandSide.x + leftHandSide.y * rightHandSide.y;
    }

    Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float time)
    {
        return start * (1.0f - time) + end * time;
    }

    Vector2 Vector2::Scale(const Vector2& inVector, const Vector2& scalarVector)
    {
        return {inVector.x * scalarVector.x, inVector.y * scalarVector.y};
    }

    Vector2 Vector2::Slerp(const Vector2& start, const Vector2& end, float time)
    {
        float dot = Dot(start, end);
        dot = dot < -1.f ? -1.f : (dot > 1.f ? 1.f : dot);
        const float theta = acosf(dot) * time;
        return start * cosf(theta) + (end - start * dot) * sinf(theta);
    }

    std::string Vector2::ToString() const
    {
        std::stringstream ss;
        ss << "X: " << x << " Y: " << y;
        return ss.str();
    }
}
