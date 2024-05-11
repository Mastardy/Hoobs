#pragma once
#include <vector>
#include <windows.h>

#include "Mathematics/Color.hpp"

namespace WSR
{
    class Renderer
    {
    public:
        Renderer(DWORD width, DWORD height) : WIDTH(width), HEIGHT(height)
        {
            dibBits.resize(WIDTH * HEIGHT);
        }
        
        void Loop(HDC, HWND, const BITMAPINFO&);
    
    private:
        std::vector<Color> dibBits;
        DWORD WIDTH;
        DWORD HEIGHT;
    };
}
