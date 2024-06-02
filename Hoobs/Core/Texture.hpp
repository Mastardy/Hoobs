#pragma once
#include <vector>

#include "../Mathematics/Color.hpp"
#include "../Mathematics/Vector2.hpp"

namespace Hoobs
{
    class Texture
    {
    public:
        Texture() = delete;
        Texture(const char*);
        ~Texture();

        Texture(const Texture&) = default;
        Texture(Texture&&) = default;
        Texture& operator=(const Texture&) = default;
        Texture& operator=(Texture&&) = default;
        
        const Color& ColorAt(const Vector2&) const;
        const Color& ColorAt(float, float) const;

        Vector2 GetSize() const { return {static_cast<float>(width), static_cast<float>(height)}; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        
    private:
        int width;
        int height;
        int channels;
        std::vector<Color> pixels;
    };
}
