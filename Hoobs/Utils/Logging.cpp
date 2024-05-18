#include "Logging.hpp"

#include <sstream>
#include <ctime>
#include <windows.h>

namespace WSR
{
    Console* Logging::m_Console = nullptr;
    
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
        m_Console = Console::GetInstance();
        if(m_Console == nullptr) throw std::runtime_error("Console is nullptr!");
        Info("Starting Logging...");
    }

    void Logging::Debug(const std::string& msg)
    {
        if(m_Console == nullptr) return;
        m_Console->QueueMessage("[", 7);
        m_Console->QueueMessage("Debug", 10);
        m_Console->QueueMessage(" - " + GetCurrentTime() + "] " + msg + "\n", 7);
    }

    void Logging::Info(const std::string& msg)
    {
        if(m_Console == nullptr) return;
        m_Console->QueueMessage("[", 7);
        m_Console->QueueMessage("Info", 11);
        m_Console->QueueMessage(" - " + GetCurrentTime() + "] " + msg + "\n", 7);
    }

    void Logging::Warning(const std::string& msg)
    {
        if(m_Console == nullptr) return;
        m_Console->QueueMessage("[", 7);
        m_Console->QueueMessage("Warning", 6);
        m_Console->QueueMessage(" - " + GetCurrentTime() + "] " + msg + "\n", 7);
    }

    void Logging::Error(const std::string& msg)
    {
        if(m_Console == nullptr) return;
        m_Console->QueueMessage("[", 7);
        m_Console->QueueMessage("Error", 4);
        m_Console->QueueMessage(" - " + GetCurrentTime() + "] " + msg + "\n", 7);
    }

    std::string Logging::Bool(bool condition)
    {
        return condition ? "True" : "False";
    }
}
