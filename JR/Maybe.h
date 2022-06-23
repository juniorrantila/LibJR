#pragma once
#include <JR/Types.h>
#include <utility>

template<typename T>
class Maybe {
public:
    constexpr Maybe<T> create_from_cheap_copy(T value)
    {
        return Maybe { std::move(value) };
    }

    constexpr Maybe(T&& value)
        : m_has_value(true)
    {
        __builtin_memcpy(&m_storage, &value, sizeof(T));
    }

    constexpr Maybe(Maybe&& other)
        : m_has_value(other.m_has_value)
    {
        __builtin_memcpy(&m_storage, &other.m_storage, sizeof(T));
        other.m_has_value = false;
    }


    constexpr Maybe()
        : m_has_value(false)
    {
    }

    constexpr Maybe<T>& operator = (T&& value)
    {
        __builtin_memcpy(&m_storage, &value, sizeof(T));
        m_has_value = true;
        return *this;
    }

    constexpr Maybe<T>& operator = (Maybe<T>&& other)
    {
        __builtin_memcpy(&m_storage, &other.m_storage, sizeof(T));
        m_has_value = other.m_has_value;
        other.m_has_value = false;
        return *this;
    }


    constexpr void reset()
    {
        m_has_value = false;
    }

    constexpr bool has_value() const { return m_has_value; }
    
    constexpr T release_value()
    {
        return std::move(value());
    }

    constexpr T& value()
    {
        return *reinterpret_cast<T*>(&m_storage);
    }

    constexpr T const& value() const
    {
        return *reinterpret_cast<T const*>(&m_storage);
    }

    constexpr T* operator -> ()
    {
        return &value();
    }

    constexpr T const* operator -> () const
    {
        return &value();
    }

    T& operator*()
    {
        return value();
    }

    T const& operator*() const
    {
        return value();
    }

private:
    alignas(T) u8 m_storage[sizeof(T)];
    bool m_has_value;
};

template<typename T>
class Maybe<T*> {
public:
    constexpr Maybe(T* value)
        : m_value(value)
    {
    }

    constexpr Maybe()
        : m_value(nullptr)
    {
    }

    constexpr void reset()
    {
        m_value = nullptr;
    }

    constexpr T* release_value() { return m_value; }
    constexpr bool has_value() const { return m_value != nullptr; }

    constexpr T* & value() { return m_value; }
    constexpr T const * const& value() const { return m_value; }

    constexpr T** operator -> ()
    {
        return &value();
    }

    constexpr T const* operator -> () const
    {
        return &value();
    }

    T const* operator*() const
    {
        return value();
    }

private:
    T* m_value { nullptr };
};
