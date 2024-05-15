#include "Time.hpp"

using namespace std::chrono;

namespace WSR
{
    Time::TimeSince::TimeSince()
    {
        timePoint = high_resolution_clock::now();
    }

    Time::TimeSince::TimeSince(const float since)
    {
        timePoint = time_point_cast<microseconds>(high_resolution_clock::now() - duration_cast<microseconds>(duration<float>(since)));
    }

    Time::TimeSince::operator float() const
    {
        return static_cast<float>(duration_cast<microseconds>(high_resolution_clock::now() - timePoint).count()) / 1000000.0f;
    }    
    
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
