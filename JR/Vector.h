#pragma once
// #include <JR/ConcurrentVector.h>
#include <JR/Forward.h>
#include <JR/ErrorOr.h>
#include <JR/ID.h>
#include <JR/Keywords.h>
#include <JR/Types.h>

template<typename T>
class Vector {
public:
    static ErrorOr<Vector<T>> create(u32 initial_capacity = 32)
    {
        var data = (T*)__builtin_malloc(sizeof(T) * initial_capacity);
        if (data == nullptr)
            return Error::allocation_failure();
        return Vector { data, initial_capacity };
    }

    constexpr void destroy() const
    {
        if (is_valid()) {
            __builtin_free(m_data);
            mutable_self().invalidate();
        }
    }

    constexpr Vector<T> move() const
    {
        var vec = Vector { m_data, m_size, m_capacity };
        mutable_self().invalidate();
        return vec;
    }

    constexpr void clear()
    {
        m_size = 0;
    }

    // Vector(ConcurrentVector<T>&& vector);

    T const* begin() const { return m_data; }
    T const* end() const { return &m_data[m_size]; };

    T* begin() { return m_data; }
    T* end() { return &m_data[m_size]; };

    ErrorOr<ID<T>> append(T&& value)
    {
        if (m_capacity <= m_size)
            TRY(expand());
        m_data[m_size] = std::move(value);
        return ID<T>(m_size++);
    }

    ID<T> unchecked_append(T&& value)
    {
        m_data[m_size] = std::move(value);
        return ID<T>(m_size++);
    }

    ErrorOr<void> ensure_capacity(u32 capacity)
    {
        if (m_capacity < capacity)
            TRY(expand_to(capacity));
        return {};
    }

    constexpr bool is_valid() const
    {
        return m_data != nullptr;
    }

    View<T> view() const { return { m_data, m_size }; }
    u32 size() const { return m_size; };

    T const& at(u32 index) const { return m_data[index]; };
    T const& operator[](u32 index) const { return at(index); };
    T& operator[](u32 index) { return m_data[index]; };

    T const& at(ID<T> id) const { return at(id.raw_id()); }
    T const& operator[](ID<T> id) const { return at(id.raw_id()); };
    T& operator[](ID<T> id) { return m_data[id.raw_id()]; }

private:
    Vector<T>& mutable_self() const 
    {
        return *const_cast<Vector<T>*>(this);
    }

    constexpr Vector(T* data, u32 initial_capacity)
        : m_data(data)
        , m_capacity(initial_capacity)
    {
    }

    constexpr Vector(T* data, u32 size, u32 capacity)
        : m_data(data)
        , m_size(size)
        , m_capacity(capacity)
    {
    }

    ErrorOr<void> expand_to(u32 new_capacity)
    {
        var new_data = (T*)__builtin_realloc(m_data,
            sizeof(T) * new_capacity);
        if (new_data == nullptr)
            return Error::allocation_failure();
        m_data = new_data;
        m_capacity = new_capacity;
        return {};

    }

    ErrorOr<void> expand()
    {
        return expand_to(m_capacity * 2);
    }

    constexpr void invalidate()
    {
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    T* m_data { nullptr };
    u32 m_size { 0 };
    u32 m_capacity { 0 };
};
