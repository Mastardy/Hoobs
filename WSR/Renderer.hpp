#pragma once
#include <thread>
#include <vector>
#include <windows.h>

#include "Mathematics/Color.hpp"

namespace WSR
{
    class Renderer
    {
    public:
        Renderer(DWORD width, DWORD height);
        
        void Loop(HDC, HWND, const BITMAPINFO&);
    
    private:
        void Render(size_t, size_t, size_t, size_t);
        void ClearBuffer();
        
        std::vector<Color> m_DibBits;
        const static Color m_BackgroundColor;
        DWORD m_Width;
        DWORD m_Height;
        
        std::vector<std::thread> m_Pool;
        size_t m_PoolSize;

        float m_CircleX;
        float m_CircleY;
        float m_CircleRadius;
    };
}
