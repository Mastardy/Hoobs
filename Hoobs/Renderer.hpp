#pragma once
#include <vector>
#include <windows.h>

#include "Core/ThreadPool.hpp"
#include "Core/Vertex.hpp"
#include "Mathematics/Color.hpp"

namespace WSR
{
    class Renderer
    {
    public:
        Renderer(DWORD, DWORD);
        
        void Loop(HDC, HWND, const BITMAPINFO&);
        
    private:
        void ClearBuffer();
        void ClearBufferThreaded(size_t start, size_t end);
        
        void Render();

        void SetColor(UINT8, UINT8, UINT8);
        void SetPixel(size_t, size_t);
        void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
        void FillTopTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
        void FillBotTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

        static inline float EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c);
        Vertex VertexShader(const Vertex& v) const;

        Color m_CurrentColor;
        
        std::vector<Color> m_DibBits;
        const static Color m_BackgroundColor;
        DWORD m_Width;
        DWORD m_Height;
        
        ThreadPool m_ThreadPool;
        size_t m_PoolSize;
    };
}
