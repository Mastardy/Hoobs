#pragma once

#include "Vector2.hpp"

#include <iostream>
#include <array>

namespace Hoobs
{
	class Rect
	{
	public:
		union
		{
			struct
			{
				float x;
				float y;
				float w;
				float h;
			};
			struct
			{
				Vector2 position;
				Vector2 size;
			};
			struct
			{
				std::array<float, 2> xy;
				std::array<float, 2> wh;
			};
			std::array<std::array<float, 2>, 2> xywh;
		};
		
		Rect();
		Rect(float value);
		Rect(const Rect& other);
		Rect(Rect&& other) noexcept;
		Rect(float x, float y, float w, float h);
		Rect(Vector2 pos, Vector2 size);

		Rect& operator=(const Rect& other);
		Rect& operator=(Rect&& other) noexcept;

		~Rect() = default;
		
		bool operator==(const Rect& other) const;
		bool operator!=(const Rect& other) const;
		
		friend std::ostream& operator<<(std::ostream& os, const Hoobs::Rect& rect) { return os << rect.ToString(); }

		float Area() const;
		Vector2 Center() const;
		bool Contains(const Vector2& point) const;
		Vector2 Max() const;
		Vector2 Min() const;

		std::string ToString() const;
	};
}