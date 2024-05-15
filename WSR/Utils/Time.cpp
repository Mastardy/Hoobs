#include "Time.hpp"

using namespace std::chrono;

namespace WSR
{
    Time::TimeSince::TimeSince()
    {
        timePoint = high_resolution_clock::now();
    }

    Time::TimeSince::TimeSince(const double& since)
    {
        timePoint = time_point_cast<microseconds>(high_resolution_clock::now() - duration_cast<microseconds>(duration<float>(since)));
    }

    Time::TimeSince::operator double() const
    {
        return static_cast<double>(duration_cast<microseconds>(high_resolution_clock::now() - timePoint).count()) / 1000000.0;
    }    
    
    float Time::timeScale = 1.0f;

    high_resolution_clock::time_point Time::appStart;
    high_resolution_clock::time_point Time::curFrame;
    high_resolution_clock::time_point Time::lastFrame;

    double Time::time = 0.0;
    double Time::deltaTime = 0.0;

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
        time = static_cast<double>(duration_cast<std::chrono::milliseconds>(curFrame - appStart).count()) / 1000.0;
    }

    void Time::CalculateDeltaTime()
    {
        deltaTime = static_cast<double>(duration_cast<std::chrono::microseconds>(curFrame - lastFrame).count()) / 1000000.0;
        lastFrame = curFrame;
    }

    double Time::GetTime()
    {
        return time;
    }

    double Time::GetDeltaTime()
    {
        return deltaTime * timeScale;
    }
}
