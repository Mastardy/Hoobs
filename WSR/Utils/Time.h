#pragma once

#include <chrono>

using namespace std::chrono;

namespace WSR
{
    class Time
    {
    public:
        static float timeScale;

        static void Init();

        static void Update();

        static float GetTime();
        static float GetDeltaTime();

    private:
        static void CalculateCurrentFrame();
        static void CalculateTime();
        static void CalculateDeltaTime();
        
        static steady_clock::time_point appStart;
        static steady_clock::time_point curFrame;
        static steady_clock::time_point lastFrame;

        static float time;
        static float deltaTime;
    };
}