#pragma once
#include <JR/Error.h>
#include <JR/Maybe.h>
#include <stdio.h>
#include <variant>

template<typename T>
class [[nodiscard]] ErrorOr {
public:
    ErrorOr() = delete;
    
    constexpr ErrorOr(T value)
        : m_error_or_value(std::move(value))
        , m_is_error(false)
    {
    }

    constexpr ErrorOr(Error error)
        : m_error_or_value(error)
        , m_is_error(true)
    {
    }

    constexpr T& value() 
    {
        return std::get<T>(m_error_or_value);
    }

    constexpr Error& error() 
    {
        return std::get<Error>(m_error_or_value);
    }

    constexpr T const& value() const
    {
        return std::get<T>(m_error_or_value);
    }

    constexpr Error const& error() const
    {
        return std::get<Error>(m_error_or_value);
    }

    constexpr T release_value()
    {
        return std::move(value());
    }

    constexpr Error release_error()
    {
        return std::move(error());
    }

    constexpr bool has_value() const { return !m_is_error; }
    constexpr bool is_error() const { return m_is_error; }

private:
    std::variant<Error, T> m_error_or_value;
    bool m_is_error { false };
};

template<>
class [[nodiscard]] ErrorOr<void> {
public:
    constexpr ErrorOr()
        : m_is_error(false)
    {

    }
    constexpr ErrorOr(Error error)
        : m_error(error)
        , m_is_error(true)
    {
    }

    constexpr void release_value() { }
    constexpr Error release_error() const { return m_error; }

    constexpr bool has_value() const { return !m_is_error; };
    constexpr bool is_error() const { return m_is_error; }

private:
    Error m_error {};
    bool m_is_error { false };
};

template <typename T>
T notify_if_error(ErrorOr<T> result)
{
    if (!result.is_error())
        return result.release_value();
    auto error = result.release_error();
    auto function_name = error.function_name();
    auto message = error.message();
    auto filename = error.filename();
    fprintf(stderr, "Notice: %*s: %*s [%*s:%d]\n",
            function_name.size(), function_name.data(),
            message.size(), message.data(),
            filename.size(), filename.data(),
            error.line_in_file());
    return T();
}
