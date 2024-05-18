#include "Rect.hpp"

#include <string>
#include <sstream>

namespace WSR
{
	Rect::Rect() : x{0}, y{0}, w{0}, h{0} {}
	Rect::Rect(float value) : x{value}, y{value}, w{value}, h{value} {}
	Rect::Rect(const Rect& other) : xywh{other.xywh} {}
	Rect::Rect(Rect&& other) noexcept : xywh{other.xywh} {}
	Rect::Rect(float x, float y, float w, float h) : x{x}, y{y}, w{w}, h{h} {}
	Rect::Rect(Vector2 pos, Vector2 size) : position{std::move(pos)}, size{std::move(size)} {}

	Rect& Rect::operator=(const Rect& other)
	{
		xywh = other.xywh;
		return *this;
	}

	Rect& Rect::operator=(Rect&& other) noexcept
	{
		xywh = other.xywh;
		return *this;
	}

	bool Rect::operator==(const Rect& other) const
	{
		return position == other.position && size == other.size;
	}

	bool Rect::operator!=(const Rect& other) const
	{
		return !(*this == other);
	}

	float Rect::Area() const
	{
		return w * 2 + h * 2;
	}

	Vector2 Rect::Center() const
	{
		return {x + (w / 2), y + (h / 2)};
	}

	bool Rect::Contains(const Vector2& point) const
	{
		const Vector2 min = Min();
		const Vector2 max = Max();

		return point.x >= min.x && point.y >= min.y && point.x <= max.x && point.y <= max.y;
	}

	Vector2 Rect::Max() const
	{
		return {w > 0 ? x + w : x, h > 0 ? y + h : y};
	}

	Vector2 Rect::Min() const
	{
		return {w < 0 ? w - x : x, h < 0 ? h - y : y};
	}

	std::string Rect::ToString() const
	{
		std::stringstream str;
		str << "X: " << x << " Y: " << y << " Width: " << w << " Height: " << h;
		return str.str();
	}
}