#include "Renderer.hpp"

#include "Utils/Logging.hpp"
#include "Utils/Time.hpp"

namespace Hoobs
{
    const Color Renderer::m_BackgroundColor = {48, 24, 24, 255};
    
    const Vertex leftTriangle[] =
    {
        {{0.25f, 0.25f, 0}, {0, 0}},
        {{0.25f, 0.75f, 0}, {0, 1}},
        {{0.75f, 0.25f, 0}, {1, 0}}
    };

    const Vertex rightTriangle[] =
    {
        {{0.25f, 0.75f, 0}, {0, 1}},
        {{0.75f, 0.75f, 0}, {1, 1}},
        {{0.75f, 0.25f, 0}, {1, 0}}
    };

    float Renderer::EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

    Vertex Renderer::VertexShader(const Vertex& v) const
    {
        return {{v.Position.x * m_Width, v.Position.y * m_Height, v.Position.z}, v.TexCoord};
    }

    Color FragmentShader(float u, float v, float w, const Vertex& v1, const Vertex& v2, const Vertex& v3, const Texture& texture)
    {
        auto uTex = v1.TexCoord.x * u + v2.TexCoord.x * v + v3.TexCoord.x * w;
        auto vTex = v1.TexCoord.y * u + v2.TexCoord.y * v + v3.TexCoord.y * w;
        auto color = texture.ColorAt(uTex, vTex);
        
        return color;
    }

    void Renderer::FillTopTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, bool unique)
    {
        float v1v4m = (v1.Position.x - v4.Position.x) / (v1.Position.y - v4.Position.y);
        float v1v3m = (v1.Position.x - v3.Position.x) / (v1.Position.y - v3.Position.y);

        m_ThreadPool.QueueJob([this, v1, v2, v3, v4, v1v4m, v1v3m, unique]()
        {
            float curx = v1.Position.x;
            float curx2 = v1.Position.x;

            for (auto scanline = static_cast<int>(v1.Position.y); scanline >= static_cast<int>(v4.Position.y - (unique ? 0 : 1)); scanline
                 --)
            {
                for (auto x = static_cast<int>(curx); x <= static_cast<int>(curx2); x++)
                {
                    Vector3 a = v2.Position - v1.Position;
                    Vector3 b = v3.Position - v1.Position;
                    Vector3 c = Vector3(static_cast<float>(x), static_cast<float>(scanline), 0) - v1.Position;
                    float den = a.x * b.y - b.x * a.y;
                    float v = (c.x * b.y - b.x * c.y) / den;
                    float w = (a.x * c.y - c.x * a.y) / den;
                    float u = 1.0f - v - w;

                    SetPixel(static_cast<size_t>(x), static_cast<size_t>(scanline),
                             FragmentShader(u, v, w, v1, v2, v3, m_Texture));
                }

                curx -= v1v3m;
                curx2 -= v1v4m;
            }
        });
    }

    void Renderer::FillBotTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
    {
        auto v3v1m = (v3.Position.x - v1.Position.x) / (v3.Position.y - v1.Position.y);
        auto v3v4m = (v3.Position.x - v4.Position.x) / (v3.Position.y - v4.Position.y);

        m_ThreadPool.QueueJob([this, v1, v2, v3, v3v1m, v3v4m]()
        {
            float curx = v3.Position.x;
            float curx2 = v3.Position.x;

            for (auto scanline = static_cast<int>(v3.Position.y); scanline <= static_cast<int>(v1.Position.y); scanline
                 ++)
            {
                for (auto x = static_cast<int>(curx); x <= static_cast<int>(curx2); x++)
                {
                    Vector3 a = v2.Position - v1.Position;
                    Vector3 b = v3.Position - v1.Position;
                    Vector3 c = Vector3(static_cast<float>(x), static_cast<float>(scanline), 0) - v1.Position;
                    float den = a.x * b.y - b.x * a.y;
                    float v = (c.x * b.y - b.x * c.y) / den;
                    float w = (a.x * c.y - c.x * a.y) / den;
                    float u = 1.0f - v - w;

                    SetPixel(static_cast<size_t>(x), static_cast<size_t>(scanline),
                             FragmentShader(u, v, w, v1, v2, v3, m_Texture));
                }

                curx += v3v1m;
                curx2 += v3v4m;
            }
        });
    }
    
    void Renderer::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    {
        if (EdgeFunction(v1.Position, v2.Position, v3.Position) < 0) return;

        auto topVertex = VertexShader(v1);
        auto midVertex = VertexShader(v2);
        auto botVertex = VertexShader(v3);

        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);
        if (midVertex.Position.y > topVertex.Position.y) std::swap(midVertex, topVertex);
        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);

        if (abs(topVertex.Position.y - midVertex.Position.y) < 0.001f)
        {
            if (midVertex.Position.x < topVertex.Position.x) std::swap(midVertex, topVertex);
        }
        else if (abs(botVertex.Position.y - midVertex.Position.y) < 0.001f)
        {
            if (midVertex.Position.x < botVertex.Position.x) std::swap(midVertex, botVertex);
        }
        
        SetColor(48, 48, 48);

        if (abs(midVertex.Position.y - botVertex.Position.y) < 0.001f)
        {
            FillTopTriangle(topVertex, midVertex, botVertex, midVertex, true);
        }
        else if (abs(midVertex.Position.y - topVertex.Position.y) < 0.001f)
        {
            FillBotTriangle(topVertex, midVertex, botVertex, midVertex);
        }
        else
        {
            Vertex v4 = {
                {
                    topVertex.Position.x + (midVertex.Position.y - topVertex.Position.y) / (botVertex.Position.y -
                        topVertex.Position.y) * (botVertex.Position.x - topVertex.Position.x),
                    midVertex.Position.y,
                    0
                },
                {0, 0}
            };
            FillTopTriangle(topVertex, botVertex, midVertex, v4, false);
            FillBotTriangle(midVertex, topVertex, botVertex, v4);
        }
    }

    Renderer::Renderer(DWORD width, DWORD height)
    {
        m_Width = width;
        m_Height = height;
        m_DibBits.resize(static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height));

        m_PoolSize = std::thread::hardware_concurrency() - 1;
        m_Texture = Texture("awesomeface.png");
        Hoobs::Logging::Info("Pool size: " + std::to_string(m_PoolSize));
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
        DrawTriangle(leftTriangle[1], leftTriangle[2], leftTriangle[0]);
        DrawTriangle(rightTriangle[1], rightTriangle[2], rightTriangle[0]);

        m_ThreadPool.Join();
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

    void Renderer::SetPixel(size_t x, size_t y, const Color& color)
    {
        if (x < 0 || y < 0 || x >= m_Width || y >= m_Height) return;
        *(m_DibBits.data() + y * m_Width + x) = color;
    }
}
