﻿#pragma once

#include <chrono>

namespace WSR
{
    using namespace std::chrono;

    class Time
    {
    public:
        class TimeSince
        {
        public:
            TimeSince();
            TimeSince(const double& since);
            
            operator double() const;
            
        private:
            time_point<high_resolution_clock> timePoint;
        };
        
    public:
        static float timeScale;

        static void Init();

        static void Update();

        static double GetTime();
        static double GetDeltaTime();

    private:
        static void CalculateCurrentFrame();
        static void CalculateTime();
        static void CalculateDeltaTime();
        
        static steady_clock::time_point appStart;
        static steady_clock::time_point curFrame;
        static steady_clock::time_point lastFrame;

        static double time;
        static double deltaTime;
    };
}