#include "Renderer.hpp"

namespace WSR
{
    void Renderer::Loop(HDC hdc, HWND hwnd, const BITMAPINFO& bmi)
    {
        auto k = 0;
        for (DWORD y = 0; y < HEIGHT; y++)
        {
            for (DWORD x = 0; x < WIDTH; x++)
            {
                dibBits[k].r = 255;
                dibBits[k].g = 255;
                dibBits[k].b = 0;
                dibBits[k++].a = 255;
            }
        }
    
        SetDIBitsToDevice(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, 0, HEIGHT, dibBits.data(), &bmi, DIB_RGB_COLORS);
    }

}