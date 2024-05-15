#pragma once
#include <vector>
#include <windows.h>

#include "Core/ThreadPool.hpp"
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
        void ClearBufferThreaded(size_t start, size_t end);
        
        std::vector<Color> m_DibBits;
        const static Color m_BackgroundColor;
        DWORD m_Width;
        DWORD m_Height;
        
        ThreadPool m_ThreadPool;
        size_t m_PoolSize;

        float m_CircleX;
        float m_CircleY;
        float m_CircleRadius;
    };
}
