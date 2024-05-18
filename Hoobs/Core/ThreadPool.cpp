#include "ThreadPool.hpp"

namespace WSR
{
    ThreadPool::ThreadPool(size_t threadsCount)
    {
        Start(threadsCount);
    }

    ThreadPool::~ThreadPool()
    {
        Stop();
    }

    void ThreadPool::Start(size_t threadsCount)
    {
        for(size_t i = 0; i < (threadsCount == 0 ? std::thread::hardware_concurrency() : threadsCount); i++)
        {
            m_Threads.emplace_back(&ThreadPool::ThreadLoop, this);
        }
    }

    void ThreadPool::QueueJob(const std::function<void()>& job)
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_JobsQueue.push(job);
        }
        m_QueueCondition.notify_one();
    }

    void ThreadPool::Stop()
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_ShouldRun = false;
        }
        m_QueueCondition.notify_all();
        for(std::thread& active_thread : m_Threads)
        {
            active_thread.join();
        }
        m_Threads.clear();
    }

    void ThreadPool::Join()
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_BusyCondition.wait(lock, [this]
        {
            return m_JobsQueue.empty();
        });
    }
    
    void ThreadPool::ThreadLoop()
    {
        while(true)
        {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                m_QueueCondition.wait(lock, [this]
                {
                    return !m_ShouldRun || !m_JobsQueue.empty();
                });
                if(!m_ShouldRun) return;
                job = m_JobsQueue.front();
                m_JobsQueue.pop();
            }
            job();
            m_BusyCondition.notify_one();
        }
    }
}
