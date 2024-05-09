#pragma once
#include <vector>
#include <windows.h>

namespace WSR
{
    struct Color
    {
        UINT8 b;
        UINT8 g;
        UINT8 r;
        UINT8 a;
    };
    
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
