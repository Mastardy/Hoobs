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
                dibBits[k].r = static_cast<UINT8>(static_cast<float>(x) / static_cast<float>(WIDTH) * 255);
                dibBits[k].g = static_cast<UINT8>(static_cast<float>(y) / static_cast<float>(HEIGHT) * 255);
                dibBits[k].b = 0;
                dibBits[k++].a = 255;
            }
        }
    
        SetDIBitsToDevice(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, 0, HEIGHT, dibBits.data(), &bmi, DIB_RGB_COLORS);
    }

}