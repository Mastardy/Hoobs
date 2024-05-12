#include "Renderer.hpp"

#include <thread>

#include "Utils/Logging.hpp"
#include "Utils/Time.hpp"

namespace WSR
{
    const Color Renderer::m_BackgroundColor = { 48, 24, 24, 255 };
    
    void WaveRGB(float t, UINT8& r, UINT8& g, UINT8& b)
    {
        float frequencyR = 0.3f;
        float frequencyG = 0.2f;
        float frequencyB = 0.1f;
        float phaseR = 0.0f;
        float phaseG = 2.0f;
        float phaseB = 4.0f;

        float valueR = sinf(frequencyR * t + phaseR) * 127 + 128;
        float valueG = sinf(frequencyG * t + phaseG) * 127 + 128;
        float valueB = sinf(frequencyB * t + phaseB) * 127 + 128;
        
        r = static_cast<UINT8>(valueR < 0 ? 0 : valueR > 255 ? 255 : valueR);
        g = static_cast<UINT8>(valueG < 0 ? 0 : valueG > 255 ? 255 : valueG);
        b = static_cast<UINT8>(valueB < 0 ? 0 : valueB > 255 ? 255 : valueB);
    }
    
    Renderer::Renderer(DWORD width, DWORD height)
    {
        m_Width = width;
        m_Height = height;
        m_DibBits.resize(static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height));
        
        m_PoolSize = std::thread::hardware_concurrency() - 1;
        WSR::Logging::Info("Pool size: " + std::to_string(m_PoolSize));
        m_Pool.resize(m_PoolSize);

        m_CircleRadius = 50;
        m_CircleX = static_cast<float>(m_Width) / 2.0f;
        m_CircleY = static_cast<float>(m_Height) / 2.0f;
    }
    
    void Renderer::Loop(HDC hdc, HWND hwnd, const BITMAPINFO& bmi)
    {
        m_CircleX = static_cast<float>(m_Width) / 2.0f + sinf(WSR::Time::GetTime()) * 100;
        m_CircleY = static_cast<float>(m_Height) / 2.0f + cosf(WSR::Time::GetTime()) * 100;

        ClearBuffer();
        
        for(size_t x = 0; x < m_PoolSize; x++)
        {
            m_Pool[x] = std::thread(&Renderer::Render, this, x * m_Width / m_PoolSize, 0, (x + 1) * m_Width / m_PoolSize, m_Height);
        }

        for (auto& t : m_Pool)
        {
            t.join();
        }

        SetDIBitsToDevice(hdc, 0, 0, m_Width, m_Height, 0, 0, 0, m_Height, m_DibBits.data(), &bmi, DIB_RGB_COLORS);
    }

    void Renderer::Render(size_t startX, size_t startY, size_t endX, size_t endY)
    {
        for (auto y = startY; y < endY; y++)
        {
            for (auto x = startX; x < endX; x++)
            {

                auto distance = sqrtf(powf(static_cast<float>(x) - static_cast<float>(m_CircleX), 2) + powf(static_cast<float>(y) - static_cast<float>(m_CircleY), 2));
                if (distance <= m_CircleRadius)
                {
                    auto i = x + y * m_Width;
                    WaveRGB(WSR::Time::GetTime() * 5, m_DibBits[i].r, m_DibBits[i].g, m_DibBits[i].b);
                    m_DibBits[i].a = 255;
                    continue;
                }
            }
        }
    }

    void Renderer::ClearBuffer()
    {
        std::fill(m_DibBits.begin(), m_DibBits.end(), m_BackgroundColor);
    }
}
