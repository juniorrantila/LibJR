#pragma once
#include <JR/StringView.h>
#include <JR/ErrorOr.h>
#include <JR/Types.h>

class String {
public:
    static ErrorOr<String> create_from(StringView other)
    {
        var data = TRY(other.to_allocated_c_string());
        var size = other.size();
        return String { data, size };
    }

    static ErrorOr<String> formatted(c_string format, ...)
    __attribute__ ((__format__ (__printf__, 1, 2)));

    void destroy() const;

    constexpr bool is_valid() const
    {
        return m_data != nullptr;
    }

    constexpr StringView view() const
    {
        return { m_data, m_size };
    }

    constexpr c_string as_c_string() const
    {
        return m_data;
    }

private:
    constexpr String(char* data, u32 size)
        : m_data(data)
        , m_size(size)
    {
    }

    constexpr void invalidate() const
    {
        mutable_self().m_data = nullptr;
    }

    String& mutable_self() const
    {
        return *const_cast<String*>(this);
    }

    char* m_data { nullptr };
    u32 m_size { 0 };
};
