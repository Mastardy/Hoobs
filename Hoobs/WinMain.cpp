#include <chrono>
#include <random>
#include <windows.h>

#include "Renderer.hpp"
#include "Utils/Logging.hpp"
#include "Utils/Console.hpp"
#include "Utils/Time.hpp"

constexpr DWORD WIDTH = 640;
constexpr DWORD HEIGHT = 360;

auto close = false;
auto minimized = false;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    auto CLASS_NAME = L"Hoobs";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0, CLASS_NAME, L"Hoobs", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT + 39,
        nullptr, nullptr, hInstance, nullptr);

    if (hWnd == nullptr) return 0;

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};

    BITMAPINFOHEADER bmih
    {
        sizeof(BITMAPINFOHEADER), WIDTH, HEIGHT, 1, 32, BI_RGB,
         0, WIDTH, HEIGHT, 0, 0
    };
    
    BITMAPINFO bmi
    {
        bmih, {{0, 0, 0, 0}}
    };

    Hoobs::Console console;
    Hoobs::Renderer renderer(WIDTH, HEIGHT);

    Hoobs::Logging::Init();
    Hoobs::Time::Init();
    
    auto hdc = GetDC(hWnd);
    auto diffStr = new wchar_t[256];

    double fpsTimer = 0.0f;
    long fps = 0;
    
    while (!close)
    {
        Hoobs::Time::Update();
        fpsTimer += Hoobs::Time::GetDeltaTime();
        ++fps;
        
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer.Loop(hdc, hWnd, bmi);

        if(fpsTimer < 1 || minimized) continue;
        auto _ = swprintf_s(diffStr, 256, L"Hoobs - %fms - %ifps", 1.0f / static_cast<float>(fps) * 1000.0f, static_cast<int>(fps));
        SetWindowText(WindowFromDC(hdc), diffStr);
        fpsTimer = 0.0f;
        fps = 0;
    }

    delete[] diffStr;
    ReleaseDC(hWnd, hdc);

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        close = true;
        return 0;
    case WM_SIZE:
        {
            minimized = wParam == SIZE_MINIMIZED;
        }
        break;
    default:
        break;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}