#include "Renderer.hpp"

#include "Utils/Logging.hpp"
#include "Utils/Time.hpp"

namespace WSR
{
    const Color Renderer::m_BackgroundColor = {48, 24, 24, 255};

#pragma region "Vertex"

    const Vertex triangle[] =
    {
        {{500, 0, 0}, {0, 255, 0, 255}},
        {{0, 500, 0}, {0, 0, 255, 255}},
        {{1000, 500, 0}, {255, 0, 0, 255}}
    };

    // const Vertex triangle[] =
    // {
    //     {{0, 0, 0}, {0, 255, 0, 255}},
    //     {{500, 500, 0}, {0, 0, 255, 255}},
    //     {{1000, 0, 0}, {255, 0, 0, 255}}
    // };

    float Renderer::EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

    void Renderer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    {
        auto topVertex = v0;
        auto midVertex = v1;
        auto botVertex = v2;

        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);
        if (midVertex.Position.y > topVertex.Position.y) std::swap(midVertex, topVertex);
        if (botVertex.Position.y > midVertex.Position.y) std::swap(botVertex, midVertex);

        auto topY = static_cast<int>(topVertex.Position.y);
        auto midY = static_cast<int>(midVertex.Position.y);
        auto botY = static_cast<int>(botVertex.Position.y);

        auto topBotM = (botVertex.Position.y - topVertex.Position.y) / (botVertex.Position.x - topVertex.Position.x);
        auto topMidM = (midVertex.Position.y - topVertex.Position.y) / (midVertex.Position.x - topVertex.Position.x);
        auto botMidM = (botVertex.Position.y - midVertex.Position.y) / (botVertex.Position.x - midVertex.Position.x);

        auto topBotC = topVertex.Position.y - topBotM * topVertex.Position.x;
        auto topMidC = topVertex.Position.y - topMidM * topVertex.Position.x;
        auto botMidC = midVertex.Position.y - botMidM * midVertex.Position.x;

        SetColor(24, 48, 24);

        for (auto y = botY; y <= midY; y++)
        {
            float startX = 0;
            float endX = 0;
            if (botMidM < 0.001f)
            {
                startX = botVertex.Position.x;
                endX = midVertex.Position.x;
            }
            else
            {
                startX = (static_cast<float>(y) - topBotC) / topBotM;
                endX = (static_cast<float>(y) - botMidC) / botMidM;
            }
            
            if (startX > endX) std::swap(startX, endX);
            
            for (auto x = static_cast<int>(startX); x < static_cast<int>(endX); x++)
            {
                SetPixel(static_cast<size_t>(x), static_cast<size_t>(y));
            }
        }

        for (auto y = midY; y <= topY; y++)
        {
            float startX = 0;
            float endX = 0;

            if (topMidM < 0.001f)
            {
                startX = topVertex.Position.x;
                endX = botVertex.Position.x;
            }
            else
            {
                startX = (static_cast<float>(y) - topBotC) / topBotM;
                endX = (static_cast<float>(y) - topMidC) / topMidM;
            }

            if (startX > endX) std::swap(startX, endX);
            
            for (auto x = static_cast<int>(startX); x < static_cast<int>(endX); x++)
            {
                SetPixel(static_cast<size_t>(x), static_cast<size_t>(y));
            }
        }
    }

#pragma endregion

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
