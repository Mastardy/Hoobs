#pragma once

#include <string>

#include "Console.hpp"

namespace Hoobs
{
    class Logging
    {
    public:
        static void Init();

        static void Debug(const std::string& msg);
        static void Info(const std::string& msg);
        static void Warning(const std::string& msg);
        static void Error(const std::string& msg);

        static std::string Bool(bool condition);

    private:
        static Console* m_Console;
    };
}
