#include "Time.h"

using namespace std::chrono;

namespace WSR
{
    float Time::timeScale = 1.0f;

    high_resolution_clock::time_point Time::appStart;
    high_resolution_clock::time_point Time::curFrame;
    high_resolution_clock::time_point Time::lastFrame;

    float Time::time = 0.0f;
    float Time::deltaTime = 0.0f;

    void Time::Init()
    {
        appStart = high_resolution_clock::now();
        curFrame = appStart;
        lastFrame = appStart;
    }

    void Time::Update()
    {
        CalculateCurrentFrame();
        CalculateTime();
        CalculateDeltaTime();
    }

    void Time::CalculateCurrentFrame()
    {
        curFrame = steady_clock::now();
    }

    void Time::CalculateTime()
    {
        time = static_cast<float>(duration_cast<std::chrono::milliseconds>(curFrame - appStart).count()) / 1000.0f;
    }

    void Time::CalculateDeltaTime()
    {
        deltaTime = static_cast<float>(duration_cast<std::chrono::microseconds>(curFrame - lastFrame).count()) / 1000000.0f;
        lastFrame = curFrame;
    }

    float Time::GetTime()
    {
        return time;
    }

    float Time::GetDeltaTime()
    {
        return deltaTime * timeScale;
    }
}
