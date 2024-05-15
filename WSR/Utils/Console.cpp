#include "Console.hpp"

#include <iostream>
#include <Windows.h>

namespace WSR
{
    Console* Console::m_Instance = nullptr;
    
    Console::Console()
    {
        m_Instance = this;
        m_ConsoleThread = std::thread(&Console::ThreadLoop, this);
    }

    Console::~Console()
    {
        m_ShouldClose = true;
        m_QueueCondition.notify_all();
        m_ConsoleThread.join();
    }

    void Console::ThreadLoop()
    {
        AllocConsole();
        (void)freopen_s(&m_Console, "CONOUT$", "w", stdout);
        SetConsoleTitle(L"WSR Console");

        while (!m_ShouldClose)
        {
            ConsoleMessage message;
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                m_QueueCondition.wait(lock, [this]
                {
                    return m_ShouldClose || !m_MessageQueue.empty();
                });
                if(m_ShouldClose) break;
                message = m_MessageQueue.front();
                m_MessageQueue.pop();
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), message.color);
            std::cout << message.message;
        }

        FreeConsole();
        (void)fclose(m_Console);
    }

    void Console::QueueMessage(const std::string& message, unsigned short color)
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_MessageQueue.push({color, message});
        }
        m_QueueCondition.notify_one();
    }

    Console* Console::GetInstance()
    {
        return m_Instance;
    }
}
