#include "Vector3.hpp"

#include <cmath>
#include <stdexcept>
#include <string>
#include <sstream>

namespace WSR
{
    //
    // Constants
    //

    const Vector3 Vector3::zero = Vector3();
    const Vector3 Vector3::one = Vector3(1.0f);

    const Vector3 Vector3::front = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::right = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::up = Vector3(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::back = Vector3(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::left = Vector3(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::down = Vector3(0.0f, 0.0f, -1.0f);

    //
    // Constructors
    //

    Vector3::Vector3() : x{0}, y{0}, z{0}
    {
    }

    Vector3::Vector3(float value) : x{value}, y{value}, z{value}
    {
    }

    Vector3::Vector3(const Vector2& other) : x{other.x}, y{other.y}, z{0}
    {
    }

    Vector3::Vector3(float x, float y) : x{x}, y{y}, z{0}
    {
    }

    Vector3::Vector3(const Vector3& other) : x{other.x}, y{other.y}, z{other.z}
    {
    }

    Vector3::Vector3(float x, float y, float z) : x{x}, y{y}, z{z}
    {
    }

    Vector3::Vector3(Vector3&& other) noexcept : x{other.x}, y{other.y}, z{other.z}
    {
    }

    //
    // Operators
    //

    Vector3& Vector3::operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3& Vector3::operator=(Vector3&& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    float Vector3::operator[](int i) const
    {
        if (i < 0 || i > 2)
        {
            throw std::out_of_range("Vector3[] - Index out of range.");
        }

        return xyz[i];
    }

    bool Vector3::operator==(const Vector3& other) const
    {
        return abs(x - other.x) < 0.0001f && abs(y - other.y) < 0.0001f && abs(z - other.z) < 0.0001f;
    }

    bool Vector3::operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        *this = *this + other;
        return *this;
    }

    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        *this = *this - other;
        return *this;
    }

    Vector3 Vector3::operator*(const Vector3& other) const
    {
        return {x * other.x, y * other.y, z * other.z};
    }

    Vector3& Vector3::operator*=(const Vector3& other)
    {
        *this = *this * other;
        return *this;
    }

    Vector3 Vector3::operator*(float scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3& Vector3::operator*=(float scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    Vector3 Vector3::operator/(float scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3& Vector3::operator/=(float scalar)
    {
        *this = *this / scalar;
        return *this;
    }

    Vector3 Vector3::operator/(const Vector3& other) const
    {
        return {x / other.x, y / other.y, z / other.z};
    }

    Vector3& Vector3::operator/=(const Vector3& other)
    {
        *this = *this / other;
        return *this;
    }


    //
    // Member Methods
    //

    bool Vector3::Equals(const Vector3& other) const
    {
        return abs(other.x - x) < FLT_EPSILON && abs(other.y - y) < FLT_EPSILON && abs(other.z - z) < FLT_EPSILON;
    }

    float Vector3::Magnitude() const
    {
        return sqrtf(SqrMagnitude());
    }

    float Vector3::SqrMagnitude() const
    {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::Normalized() const
    {
        const float magnitude = Magnitude();
        if (abs(magnitude) < 0.0001f) return zero;
        return Vector3(*this / magnitude);
    }

    void Vector3::Normalize()
    {
        const auto normalized = this->Normalized();
        x = normalized.x;
        y = normalized.y;
        z = normalized.z;
    }

    //
    // Static Methods
    //

    Vector3 Vector3::Cross(const Vector3& leftHandSide, const Vector3& rightHandSide)
    {
        return {
            (leftHandSide.y * rightHandSide.z) - (leftHandSide.z * rightHandSide.y),
            (leftHandSide.z * rightHandSide.x) - (leftHandSide.x * rightHandSide.z),
            (leftHandSide.x * rightHandSide.y) - (leftHandSide.y * rightHandSide.x)
        };
    }

    float Vector3::Distance(const Vector3& leftHandSide, const Vector3& rightHandSide)
    {
        return (leftHandSide - rightHandSide).Magnitude();
    }

    float Vector3::Dot(const Vector3& leftHandSide, const Vector3& rightHandSide)
    {
        return leftHandSide.x * rightHandSide.x + leftHandSide.y * rightHandSide.y + leftHandSide.z * rightHandSide.z;
    }

    Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float time)
    {
        return start * (1.0f - time) + end * time;
    }

    Vector3 Vector3::Scale(const Vector3& inVector, const Vector3& scalarVector)
    {
        return {inVector.x * scalarVector.x, inVector.y * scalarVector.y, inVector.z * scalarVector.z};
    }

    Vector3 Vector3::Slerp(const Vector3& start, const Vector3& end, float time)
    {
        float dot = Dot(start, end);
        dot = dot < -1.f ? -1.f : (dot > 1.f ? 1.f : dot);
        const float theta = acosf(dot) * time;
        return start * cosf(theta) + (end - start * dot) * sinf(theta);
    }

    std::string Vector3::ToString() const
    {
        std::stringstream ss;
        ss << "X: " << x << " Y: " << y << " Z: " << z;
        return ss.str();
    }
}
