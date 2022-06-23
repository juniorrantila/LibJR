#pragma once
#include <JR/Vector.h>
#include <JR/ErrorOr.h>
#include <JR/Lock.h>

template<typename T>
class MultiProducerQueue {
public:
    static ErrorOr<MultiProducerQueue> create(u32 initial_capacity = 32)
    {
        return { TRY(Vector<T>::create(initial_capacity)) };
    }

    constexpr void destroy() const
    {
        if (is_valid()) {
            m_data.destroy();
        }
    }

    constexpr bool is_valid() const
    {
        return m_data.is_valid();
    }

    ErrorOr<void> enqueue(T value)
    {
        m_lock.hold();
        TRY(m_data.append(std::move(value)));
        m_lock.release();
        return {};
    }

    T unchecked_dequeue()
    {
        return std::move(m_data[m_tail++]);
    }

    bool is_empty() const { return m_tail == m_data.size(); }

    constexpr MultiProducerQueue(MultiProducerQueue&& other)
        : m_data(std::move(other.m_data))
        , m_tail(std::move(other.m_tail))
    {
        other.m_tail = 0;
    }

private:
    constexpr MultiProducerQueue(Vector<T>&& data)
        : m_data(std::move(data))
    {
    }

    Vector<T> m_data;
    u32 m_tail { 0 };
    Lock m_lock;
};
