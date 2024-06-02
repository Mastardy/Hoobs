#pragma once
#include <string>
#include <windows.h>

namespace Hoobs
{
    class Color
    {
    public:
        static const Color BLACK;
        static const Color GRAY;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color CYAN;
        static const Color YELLOW;
        static const Color MAGENTA;
    
        UINT8 b;
        UINT8 g;
        UINT8 r;
        UINT8 a;

        Color();
        Color(UINT8 v);
        Color(UINT8 b, UINT8 g, UINT8 r, UINT8 a = 255);
        
        Color(const Color& color);
        Color(Color&& color) noexcept;
        
        Color& operator=(const Color& color);
        Color& operator=(Color&& color) noexcept;
        
        UINT8 operator[](UINT8 index) const;

        Color operator*(float scalar) const;
        Color operator+(const Color& color) const;

        UINT8 Grayscale() const;

        operator std::string() const;
        
        ~Color();
    };
}