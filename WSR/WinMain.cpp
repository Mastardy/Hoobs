#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <windows.h>

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 500;

struct Color
{
    UINT8 b;
    UINT8 g;
    UINT8 r;
    UINT8 a;
};

BITMAPINFO bmi = {};
BITMAPINFOHEADER bmih;
    
// Create a sample for dibBits
std::vector<Color> dibBits(HEIGHT * WIDTH);

std::mt19937 gen(std::time(nullptr));
std::uniform_int_distribution<int> dis(0, 255);

void DrawBitmap(HDC hdc)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto k = 0;
    for(auto y = 0; y < HEIGHT; y++)
    {
        for(auto x = 0; x < WIDTH; x++)
        {            
            dibBits[k].r = 255;
            dibBits[k].g = 0;
            dibBits[k].b = 0;
            dibBits[k++].a = 255;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    SetDIBitsToDevice(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, 0, HEIGHT, dibBits.data(), &bmi, DIB_RGB_COLORS);

    auto deltaTime = static_cast<float>(duration) / 1000.0f;
    int fps = static_cast<int>(1.0f / (deltaTime / 1000));
        
    // Create a LPCWSTR for title with diff.top
    auto diffStr = new wchar_t[256];
    auto _ = swprintf_s(diffStr, 256, L"WSR - %fms - %ifps", deltaTime, fps);
    SetWindowText(WindowFromDC(hdc), diffStr);
} 

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

    if(hWnd == nullptr) return 0;

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
    
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Render Method
        DrawBitmap(hdc);
            
        EndPaint(hWnd, &ps);
    }
    
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}