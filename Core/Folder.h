#pragma once
#include <JR/ErrorOr.h>
#include <JR/Maybe.h>
#include <JR/String.h>
#include <JR/Vector.h>
#include <JR/View.h>
#include <dirent.h>

namespace Core {

class Folder {
public:
    static ErrorOr<Folder> open(StringView path);
    static ErrorOr<Folder> open_relative_to(Folder const& root_folder, StringView path);

    ErrorOr<StringView> find_name_of_first_file_matching_name_not_including_extension(StringView name) const;

    constexpr StringView name() const
    {
        return m_name;
    }

    int fd() const { return m_fd; }

    void close() const;

    constexpr bool is_valid() const
    {
        return m_fd != -1;
    }

    ErrorOr<View<StringView>> file_names();

private:
    constexpr Folder(StringView name, DIR* dir, int fd)
        : m_name(name)
        , m_dir(dir)
        , m_fd(fd)
    {
    }

    constexpr void invalidate() const
    {
        mutable_self().m_fd = -1;
    }

    constexpr Folder& mutable_self() const
    {
        return *const_cast<Folder*>(this);
    }

    Maybe<Vector<StringView>> m_file_names;
    StringView m_name {};
    DIR* m_dir { nullptr };
    int m_fd { -1 };
};

}
