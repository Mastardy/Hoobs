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
        void Render(size_t, size_t, size_t, size_t);
    
    private:
        std::vector<Color> m_DibBits;
        DWORD m_Width;
        DWORD m_Height;

        std::vector<std::thread> m_Pool;
        size_t m_PoolSize;
    };
}
