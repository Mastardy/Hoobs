#pragma once
#include "../Mathematics/Color.hpp"
#include "../Mathematics/Vector3.hpp"

namespace Hoobs
{
    struct Vertex
    {
        Hoobs::Vector3 Position;
        WSR::Color Color;
    };
}