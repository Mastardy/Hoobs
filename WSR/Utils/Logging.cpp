#include "Logging.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <windows.h>

namespace WSR
{
    std::string GetCurrentTime()
    {
        const time_t now = std::time(nullptr);
        std::tm timeInfo{};
        (void)localtime_s(&timeInfo, &now);

        const auto format = "%d/%m/%Y %H:%M:%S";

        char buffer[80];
        (void)std::strftime(buffer, sizeof(buffer), format, &timeInfo);

        return {buffer};
    }
    
    void Logging::Init()
    {
        Info("Starting Logging...");
    }

    void Logging::Debug(const std::string& msg)
    {
        std::cout << "[";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        std::cout << "Debug";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        std::cout << " - " << GetCurrentTime() << "] " << msg << "\n";
    }

    void Logging::Info(const std::string& msg)
    {
        std::cout << "[";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << "Info";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        std::cout << " - " << GetCurrentTime() << "] " << msg << "\n";
    }

    void Logging::Warning(const std::string& msg)
    {
        std::cout << "[";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        std::cout << "Warning";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        std::cout << " - " << GetCurrentTime() << "] " << msg << "\n";
    }

    void Logging::Error(const std::string& msg)
    {
        std::cout << "[";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        std::cout << "Error";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        std::cout << " - " << GetCurrentTime() << "] " << msg << "\n";
    }

    std::string Logging::Bool(const bool& condition)
    {
        return condition ? "True" : "False";
    }

    void Logging::Pause()
    {
        std::cin.get();
    }
}
