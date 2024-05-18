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
        for(auto i = 0; i < 5; i++)
        {
            auto x = static_cast<float>(m_Width) / 2.0f + static_cast<float>(sin(WSR::Time::GetTime() * (i + 1))) * i * 20;
            auto y = static_cast<float>(m_Height) / 2.0f + static_cast<float>(cos(WSR::Time::GetTime() * (i + 1))) * i * 20;
            SetColor(255, 0, 0);
            for(auto j = -1; j <= 1; j++)
            {
                for(auto k = -1; k <= 1; k++)
                {
                    SetPixel(static_cast<size_t>(x) + j, static_cast<size_t>(y) + k);
                }
            }
        }
    }

    void Renderer::SetColor(UINT8 r, UINT8 g, UINT8 b)
    {
        m_CurrentColor = { b, g, r, 255 };
    }

    void Renderer::SetPixel(size_t x, size_t y)
    {
        if(x < 0 || y < 0 || x >= m_Width || y >= m_Height) return;

        auto pixel = m_DibBits.data() + y * m_Width + x;
        *pixel = m_CurrentColor;
    }
}
