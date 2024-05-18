#pragma once
#include <functional>
#include <mutex>
#include <queue>

namespace WSR
{
    class ThreadPool
    {
    public:
        ThreadPool(size_t threadsCount = 0);
        ~ThreadPool();

        ThreadPool(const ThreadPool& other) = delete;
        ThreadPool(ThreadPool&& other) = delete;
        ThreadPool& operator=(const ThreadPool& other) = delete;
        ThreadPool& operator=(ThreadPool&& other) = delete;
        
        void Start(size_t threadsCount = 0);
        void QueueJob(const std::function<void()>& job);
        void Stop();

        void Join();

    private:
        void ThreadLoop();

        bool m_ShouldRun = true;
        std::mutex m_QueueMutex;
        std::queue<std::function<void()>> m_JobsQueue;
        std::vector<std::thread> m_Threads;
        std::condition_variable m_QueueCondition;
        std::condition_variable m_BusyCondition;
        std::atomic<int> m_BusyThreads = 0;
    };
}