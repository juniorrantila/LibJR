#pragma once
#include <JR/Vector.h>
#include <JR/ErrorOr.h>

template<typename T>
class Queue {
public:
    static ErrorOr<Queue> create(u32 initial_capacity = 32)
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
        TRY(m_data.append(std::move(value)));
        return {};
    }

    T unchecked_dequeue()
    {
        return std::move(m_data[m_tail++]);
    }

    bool is_empty() const { return m_tail == m_data.size(); }

    constexpr Queue(Queue&& other)
        : m_data(std::move(other.m_data))
        , m_tail(std::move(other.m_tail))
    {
        other.m_tail = 0;
    }

private:
    constexpr Queue(Vector<T>&& data)
        : m_data(std::move(data))
    {
    }

    Vector<T> m_data;
    u32 m_tail { 0 };
};
