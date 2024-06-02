#include "Texture.hpp"

#include "../Utils/Logging.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

namespace Hoobs
{
    Texture::Texture(const char* path)
    {
        Logging::Info("Loading texture: " + std::string(path));
        
        stbi_set_flip_vertically_on_load(true);
        
        auto data = stbi_load(path, &width, &height, &channels, 4);
        if (!data)
        {
            Logging::Error("Failed to load texture: " + std::string(path));
            return;
        }
        
        for (int i = 0; i < width * height * 4; i += 4)
        {
            pixels.emplace_back(data[i + 2], data[i + 1], data[i], data[i + 3]);
        }
        
        stbi_image_free(data);
    }

    Texture::~Texture() = default;

    const Color& Texture::ColorAt(const Vector2& uv) const
    {
        return ColorAt(uv.x, uv.y);
    }

    const Color& Texture::ColorAt(float u, float v) const
    {
        if (pixels.empty()) return Color::MAGENTA;
        
        int x = static_cast<int>(u * static_cast<float>(width - 1));
        int y = static_cast<int>(v * static_cast<float>(height - 1));
        
        int index = (y * width + x);
        if (index < 0 || index >= width * height)
        {
            return Color::MAGENTA;
        }
        
        return pixels[index];
    }
}
