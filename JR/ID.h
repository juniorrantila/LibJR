#pragma once
#include <JR/Types.h>

template<typename T>
class ID {
public:
    constexpr explicit ID(u32 raw_id)
        : m_raw_id(raw_id)
    {
    }

    constexpr u32 raw_id() const { return m_raw_id; }
private:
    u32 m_raw_id;
};
