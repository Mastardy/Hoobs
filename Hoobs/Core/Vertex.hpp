#pragma once
#include "../Mathematics/Color.hpp"
#include "../Mathematics/Vector3.hpp"

namespace Hoobs
{
    struct Vertex
    {
        Hoobs::Vector3 Position;
        Hoobs::Color Color;
        Hoobs::Vector2 TexCoord;
    };
}