#pragma once
#include <JR/ErrorOr.h>
#include <JR/String.h>
#include <dlfcn.h>

namespace Core {

class Library {
public:
    static ErrorOr<Library> open(c_string path)
    {
        var handle = dlopen(path, RTLD_LAZY);
        if (!handle)
            return Error::from_string_literal(dlerror());
        return { handle };
    }

    static ErrorOr<Library> open(StringView filename)
    {
        let path = TRY(String::create_from(filename));
        var handle = dlopen(path.as_c_string(), RTLD_LAZY);
        if (!handle)
            return Error::from_string_literal(dlerror());
        return { handle };
    }

    static ErrorOr<Library> open(String filename)
    {
        var handle = dlopen(filename.as_c_string(),
            RTLD_LAZY);
        if (!handle)
            return Error::from_string_literal(dlerror());
        return { handle };
    }

    void close() const
    {
        if (is_valid()) {
            dlclose(m_handle);
            invalidate();
        }
    }

    constexpr bool is_valid() const
    {
        return m_handle != nullptr;
    }

    template<typename T>
    ErrorOr<T> fetch_symbol(c_string symbol_name) const
    {
        var symbol = dlsym(m_handle, symbol_name);
        if (!symbol)
            return Error::from_string_literal(dlerror());
        return (T)symbol;
    }

private:
    constexpr Library(void* handle)
        : m_handle(handle)
    {
    }

    constexpr void invalidate() const
    {
        mutable_self().m_handle = nullptr;
    }

    constexpr Library& mutable_self() const
    {
        return *const_cast<Library*>(this);
    }

    void* m_handle { nullptr };
};

}
