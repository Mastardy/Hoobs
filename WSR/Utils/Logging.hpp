#pragma once

#include <string>

namespace WSR
{
    class Logging
    {
    public:
        static void Init();

        static void Debug(const std::string& msg);
        static void Info(const std::string& msg);
        static void Warning(const std::string& msg);
        static void Error(const std::string& msg);

        static std::string Bool(const bool& condition);

        static void Pause();
    };
}