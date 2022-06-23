#pragma once
#include <JR/Concepts.h>
#include <JR/Forward.h>
#include <JR/Keywords.h>
#include <JR/Types.h>

struct SingleSplit;

class StringView {
public:
    constexpr StringView(char const* data, u32 size)
        : m_data(data)
        , m_size(size)
    {
    }

    constexpr StringView() { }

    constexpr StringView(c_string data)
        : m_data(data)
        , m_size(__builtin_strlen(data))
    {
    }

    constexpr bool contains(char character) const
    {
        for (let c : *this) {
            if (c == character)
                return true;
        }
        return false;
    }

    constexpr bool ends_with(StringView value) const
    {
        i64 starting_index = (i64)m_size - (i64)value.m_size;
        if (starting_index < 0)
            return false;
        let view = split_view(starting_index, m_size);
        return view == value;
    }

    constexpr bool operator==(StringView other) const
    {
        if (size() != other.size())
            return false;
        return __builtin_strncmp(data(), other.data(),
                   size())
            == 0;
    }

    constexpr bool operator==(c_string other) const
    {
        return StringView(other) == *this;
    }

    ErrorOr<SingleSplit> split_on_first(char character) const;

    constexpr StringView split_view(u32 start, u32 end) const
    {
        return { &m_data[start], end - start };
    }

    constexpr char const* data() const { return m_data; }
    constexpr u32 size() const { return m_size; }

    constexpr char const* begin() const { return m_data; }
    constexpr char const* end() const { return &m_data[m_size]; }

    template<Number T>
    ErrorOr<T> as_number() const;

    ErrorOr<char*> to_allocated_c_string() const;
    ErrorOr<String> to_string() const;

private:
    char const* m_data { nullptr };
    u32 m_size { 0 };
};

template<>
class View<StringView> {
public:
    constexpr View(StringView* data, u32 size)
        : m_data(data)
        , m_size(size)
    {
    }

    constexpr StringView const* data() const { return m_data; }
    constexpr u32 size() const { return m_size; }

    StringView flatten_with(char delimiter);

    constexpr StringView const* begin() const { return m_data; }
    constexpr StringView const* end() const { return &m_data[m_size]; }

private:
    StringView const* m_data;
    u32 m_size;
};
