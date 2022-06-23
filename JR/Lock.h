#pragma once
#include <JR/Atomic.h>
#include <JR/Keywords.h>

class Lock {
public:
    void hold()
    {
        var no = false;
        while(!m_lock_held.compare_exchange_weak(no, true)) { 
            no = false;
        }
    }

    void release()
    {
        var yes = true;
        while(!m_lock_held.compare_exchange_weak(yes, false)) {
            yes = true;
        }
    }
private:
    Atomic<bool> m_lock_held { false };
};
