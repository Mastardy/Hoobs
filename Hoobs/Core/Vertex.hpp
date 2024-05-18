#pragma once
#include "../Mathematics/Color.hpp"
#include "../Mathematics/Vector3.hpp"

namespace WSR
{
    struct Vertex
    {
        WSR::Vector3 Position;
        WSR::Color Color;
    };
}