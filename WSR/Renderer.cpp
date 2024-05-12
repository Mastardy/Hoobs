#include "Renderer.hpp"

#include <thread>

#include "Utils/Logging.hpp"

namespace WSR
{
    Renderer::Renderer(DWORD width, DWORD height)
    {
        m_Width = width;
        m_Height = height;
        m_DibBits.resize(static_cast<size_t>(m_Width) * static_cast<size_t>(m_Height));
        m_PoolSize = std::thread::hardware_concurrency() - 1;
        WSR::Logging::Info("Pool size: " + std::to_string(m_PoolSize));
        m_Pool.resize(m_PoolSize);
    }
    
    void Renderer::Loop(HDC hdc, HWND hwnd, const BITMAPINFO& bmi)
    {
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
                auto i = x + y * m_Width;
                m_DibBits[i].r = static_cast<UINT8>(static_cast<float>(x) / static_cast<float>(m_Width) * 255);
                m_DibBits[i].g = static_cast<UINT8>(static_cast<float>(y) / static_cast<float>(m_Height) * 255);
                m_DibBits[i].b = 0;
                m_DibBits[i].a = 255;
            }
        }
    }
}
