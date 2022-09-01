#pragma once
#include <Core/Forward.h>
#include <JR/ErrorOr.h>
#include <JR/StringView.h>
#include <JR/Maybe.h>

namespace Core {

class MappedFile {
public:
    static ErrorOr<MappedFile> open(StringView path);

    static ErrorOr<MappedFile> open_relative_to(Folder const& root_folder,
        StringView path);

    StringView view() const { return { m_data, m_size }; }
    StringView filename() const { return m_filename; }
    Maybe<StringView> read_line();

    MappedFile(MappedFile&& other)
        : m_filename(other.filename())
        , m_data(other.m_data)
        , m_size(other.m_size)
    {
        other.m_data = nullptr;
    }

    void close() const;

    constexpr bool is_valid() const
    {
        return m_data != nullptr;
    }

    constexpr bool eof() const { return m_cursor == m_size; }

private:
    MappedFile(StringView filename, c_string data, u32 size)
        : m_filename(filename)
        , m_data(data)
        , m_size(size)
    {
    }

    constexpr void invalidate() const
    {
        var& self = mutable_self();
        self.m_data = nullptr;
        self.m_size = 0;
    }

    constexpr MappedFile& mutable_self() const
    {
        return *const_cast<MappedFile*>(this);
    }

    StringView m_filename {};
    c_string m_data { nullptr };
    u32 m_size { 0 };
    u32 m_cursor { 0 };
};

}
