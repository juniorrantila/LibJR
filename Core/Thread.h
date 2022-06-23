#pragma once
#include <JR/ErrorOr.h>
#include <JR/Function.h>
#include <JR/Keywords.h>
#include <thread>

namespace Core {

enum class ThreadPriority {
    Low,
    Normal,
    High,
    RealTime,
};

class Thread {
public:
    static ErrorOr<Thread> create(Function<void()>&& function)
    {
        return Thread { std::move(function) };
    }

    void start()
    {
        m_thread = std::jthread(m_function);
        var priority = 0;
        switch (m_priority) {
        case ThreadPriority::Normal:
            break;
        case ThreadPriority::Low:
            priority = 0;
            break;
        case ThreadPriority::High:
            priority = 50;
            break;
        case ThreadPriority::RealTime:
            priority = 99;
            break;
        }
        if (m_priority != ThreadPriority::Normal) {
            sched_param schedule;
            int policy = 0;
            let handle = m_thread.native_handle();
            pthread_getschedparam(handle, &policy, &schedule);
            schedule.sched_priority = priority;
            pthread_setschedparam(handle, SCHED_FIFO, &schedule);
        }
    }

    void set_priority(ThreadPriority priority)
    {
        m_priority = priority;
    }

    void join()
    {
        m_thread.join();
    }

private:
    Thread(Function<void()>&& function)
        : m_function(std::move(function))
    {
    }

    Function<void()> m_function;
    std::jthread m_thread;
    ThreadPriority m_priority { ThreadPriority::Normal };
};

}
