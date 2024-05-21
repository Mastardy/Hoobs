#include "Renderer.hpp"

#include "Utils/Logging.hpp"
#include "Utils/Time.hpp"

namespace WSR
{
    const Color Renderer::m_BackgroundColor = {48, 24, 24, 255};

    const Vertex triangle[] =
    {
        {{0.25f, 0.5f, 0}, {0, 255, 0, 255}},
        {{0.5f, 1.0f, 0}, {0, 0, 255, 255}},
        {{0.75f, 0.0f, 0}, {255, 0, 0, 255}}
    };

    float Renderer::EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

    Vertex Renderer::VertexShader(const Vertex& v) const
    {
        return {{v.Position.x * m_Width, v.Position.y * m_Height, v.Position.z}, v.Color};
    }

    void Renderer::FillTopTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        float v1v2m = (v1.Position.x - v2.Position.x) / (v1.Position.y - v2.Position.y);
        float v1v3m = (v1.Position.x - v3.Position.x) / (v1.Position.y - v3.Position.y);

        float curx = v1.Position.x;
        float curx2 = v1.Position.x;

        for(auto scanline = static_cast<int>(v1.Position.y); scanline > static_cast<int>(v2.Position.y); scanline--)
        {
            for(auto x = static_cast<int>(curx); x <= static_cast<int>(curx2); x++)
            {
                SetPixel(static_cast<size_t>(x), static_cast<size_t>(scanline));
            }

            curx -= v1v3m;
            curx2 -= v1v2m;
        }
    }

    void Renderer::FillBotTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        auto v3v1m = (v3.Position.x - v1.Position.x) / (v3.Position.y - v1.Position.y);
        auto v3v2m = (v3.Position.x - v2.Position.x) / (v3.Position.y - v2.Position.y);

        auto curx = v3.Position.x;
        auto curx2 = v3.Position.x;
            
        for(auto scanline = static_cast<int>(v3.Position.y); scanline <= static_cast<int>(v1.Position.y); scanline++)
        {                
            for(auto x = static_cast<int>(curx); x <= static_cast<int>(curx2); x++)
            {
                SetPixel(static_cast<size_t>(x), static_cast<size_t>(scanline));
            }

            curx += v3v1m;
            curx2 += v3v2m;
        }
    }

    
    void Renderer::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        auto topVertex = VertexShader(v1);
        auto midVertex = VertexShader(v2);
        auto botVertex = VertexShader(v3);

        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);
        if (midVertex.Position.y > topVertex.Position.y) std::swap(midVertex, topVertex);
        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);

        SetColor(48, 48, 48);
        
        if(abs(midVertex.Position.y - botVertex.Position.y) < 0.001f)
        {
            FillTopTriangle(topVertex, midVertex, botVertex);
        }
        else if(abs(midVertex.Position.y - topVertex.Position.y) < 0.001f)
        {
            FillBotTriangle(topVertex, midVertex, botVertex);
        }
        else
        {
            Vertex v4 = {
                {
                    topVertex.Position.x + (midVertex.Position.y - topVertex.Position.y) / (botVertex.Position.y - topVertex.Position.y) * (botVertex.Position.x - topVertex.Position.x),
                    midVertex.Position.y,
                    0
                },
                {0, 0, 0, 0}
            };
            FillTopTriangle(topVertex, v4, midVertex);
            FillBotTriangle(midVertex, v4, botVertex);
        }
    }

    Renderer::Renderer(DWORD width, DWORD height)
    {
        m_Width = width;
        m_Height = height;
        m_DibBits.resize(static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height));

        m_PoolSize = std::thread::hardware_concurrency() - 1;
        WSR::Logging::Info("Pool size: " + std::to_string(m_PoolSize));
        m_ThreadPool.Start(m_PoolSize);
    }

    void Renderer::Loop(HDC hdc, HWND hwnd, const BITMAPINFO& bmi)
    {
        ClearBuffer();

        Render();

        SetDIBitsToDevice(hdc, 0, 0, m_Width, m_Height, 0, 0, 0, m_Height, m_DibBits.data(), &bmi, DIB_RGB_COLORS);

        return;
    }

    void Renderer::ClearBuffer()
    {
        auto size = m_DibBits.size();

        for (size_t x = 0; x < m_PoolSize; x++)
        {
            m_ThreadPool.QueueJob([this, x, size]()
            {
                this->ClearBufferThreaded(size / m_PoolSize * x, size / m_PoolSize * (x + 1));
            });
        }

        m_ThreadPool.Join();
    }

    void Renderer::ClearBufferThreaded(size_t start, size_t end)
    {
        std::fill(m_DibBits.begin() + start, m_DibBits.begin() + end, m_BackgroundColor);
    }

    void Renderer::Render()
    {
        DrawTriangle(triangle[1], triangle[0], triangle[2]);
    }

    void Renderer::SetColor(UINT8 r, UINT8 g, UINT8 b)
    {
        m_CurrentColor = {b, g, r, 255};
    }

    void Renderer::SetPixel(size_t x, size_t y)
    {
        if (x < 0 || y < 0 || x >= m_Width || y >= m_Height) return;
        *(m_DibBits.data() + y * m_Width + x) = m_CurrentColor;
    }
}
