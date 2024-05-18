#pragma once

#include <cstdio>
#include <mutex>
#include <queue>
#include <thread>

namespace WSR
{
    struct ConsoleMessage
    {
        unsigned short color;
        std::string message;
    };
    
    class Console
    {
    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console(Console&&) = delete;
        Console operator=(const Console&) = delete;
        Console operator=(Console&&) = delete;

        void QueueMessage(const std::string& message, unsigned short color);

        static Console* GetInstance();
        
    private:
        void ThreadLoop();
             
        FILE* m_Console;
        std::mutex m_QueueMutex;
        std::queue<ConsoleMessage> m_MessageQueue;
        std::condition_variable m_QueueCondition;
        bool m_ShouldClose;
        std::thread m_ConsoleThread;

        static Console* m_Instance;
    };
}
