#pragma once
#include <JR/Types.h>

class Time {
public:
    static constexpr Time from_milliseconds(f32 ms)
    {
        return ms * 1000.0;
    }

    static constexpr Time from_seconds(f32 s)
    {
        return s;
    }

    constexpr f32 in_seconds() const { return m_time_in_seconds; }
    constexpr f32 in_milliseconds() const
    {
        return m_time_in_seconds * 1000.0;
    }

private:
    constexpr Time(f32 time_in_seconds)
        : m_time_in_seconds(time_in_seconds)
    {
    }

    f32 m_time_in_seconds;
};
