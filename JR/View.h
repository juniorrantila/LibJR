#pragma once
#include <JR/Types.h>

template<typename T>
class View {
public:
    constexpr View(T const* data, u32 size)
        : m_data(data)
        , m_size(size)
    {
    }

    constexpr T const* data() const { return m_data; }
    constexpr u32 size() const { return m_size; }

    constexpr T const* begin() const { return m_data; }
    constexpr T const* end() const { return &m_data[m_size]; }

    constexpr T const* begin() { return m_data; }
    constexpr T const* end() { return &m_data[m_size]; }

    T operator[](u32 index)
    {
        return m_data[index];
    }

    T operator[](u32 index) const
    {
        return m_data[index];
    }

private:
    T const* m_data { nullptr };
    u32 m_size { 0 };
};
