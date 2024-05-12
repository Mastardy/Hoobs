#include <chrono>
#include <random>
#include <windows.h>

#include "Renderer.hpp"
#include "Utils/Logging.hpp"
#include "Utils/Console.hpp"
#include "Utils/Time.hpp"

constexpr DWORD WIDTH = 1000;
constexpr DWORD HEIGHT = 500;

BITMAPINFO bmi = {};
BITMAPINFOHEADER bmih;

auto close = false;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    auto CLASS_NAME = L"WSR";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0, CLASS_NAME, L"WSR", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT + 39,
        nullptr, nullptr, hInstance, nullptr);

    if (hWnd == nullptr) return 0;

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};

    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = WIDTH;
    bmih.biHeight = HEIGHT;
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = WIDTH * HEIGHT * 4;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    bmi.bmiHeader = bmih;

    WSR::Console console;
    WSR::Renderer renderer(WIDTH, HEIGHT);

    WSR::Logging::Init();
    WSR::Time::Init();

    auto hdc = GetDC(hWnd);
    auto diffStr = new wchar_t[256];
    
    while (!close)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        WSR::Time::Update();
        renderer.Loop(hdc, hWnd, bmi);
        
        auto _ = swprintf_s(diffStr, 256, L"WSR - %fms - %ifps", WSR::Time::GetDeltaTime() * 1000.0f, static_cast<int>(1.0f / WSR::Time::GetDeltaTime()));
        SetWindowText(WindowFromDC(hdc), diffStr);
    }
    
    delete[] diffStr;
    ReleaseDC(hWnd, hdc);
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        close = true;
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
