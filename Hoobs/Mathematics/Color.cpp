#include "Color.hpp"

namespace WSR
{
    const Color Color::BLACK = Color(0);
    const Color Color::GRAY = Color(128);
    const Color Color::WHITE = Color(255);
    const Color Color::RED = Color(0, 0, 255);
    const Color Color::GREEN = Color(0, 255, 0);
    const Color Color::BLUE = Color(255, 0, 0);
    const Color Color::CYAN = Color(255, 255, 0);
    const Color Color::YELLOW = Color(0, 255, 255);
    const Color Color::MAGENTA = Color(255, 0, 255);
    
    
    Color::Color(): b(0), g(0), r(0), a(0) {}
    Color::Color(UINT8 v): b(v), g(v), r(v), a(255) {}
    Color::Color(UINT8 b, UINT8 g, UINT8 r, UINT8 a): b(b), g(g), r(r), a(a) {}

    Color::Color(const Color& color) = default;
    Color::Color(Color&& color) noexcept
    {
        b = color.b;
        g = color.g;
        r = color.r;
        a = color.a;
    }

    Color& Color::operator=(const Color& color) = default;
    Color& Color::operator=(Color&& color) noexcept
    {
        b = color.b;
        g = color.g;
        r = color.r;
        a = color.a;
        return *this;
    }

    UINT8 Color::operator[](UINT8 index) const
    {
        switch (index)
        {
            case 0: return b;
            case 1: return g;
            case 2: return r;
            case 3: return a;
            default: return 0;
        }
    }

    UINT8 Color::Grayscale() const
    {
        return static_cast<UINT8>(0.299f * static_cast<float>(r) + 0.587f * static_cast<float>(g) + 0.114f * static_cast<float>(b));
    }

    Color::operator std::string() const
    {
        return "(" + std::to_string(b) + ", " + std::to_string(g) + ", " + std::to_string(r) + ", " + std::to_string(a) + ")";
    }
    
    Color::~Color() = default;
}
