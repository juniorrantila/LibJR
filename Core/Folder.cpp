#include <Core/System.h>
#include <JR/String.h>
#include <Core/Folder.h>
#include <JR/Defer.h>
#include <JR/GuardDefer.h>
#include <JR/Keywords.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Core {

ErrorOr<Folder> Folder::open(StringView path)
{
    let fd = TRY(Core::System::open(path));
    GuardDefer close_fd = [&] {
        ::close(fd);
    };
    let file_info = TRY(Core::System::fstat(fd));
    if (!file_info.is_folder())
        return Error::from_string_literal("file is not a folder");
    close_fd.let_down_guard();
    
    var dir = fdopendir(fd);
    if (!dir)
        return Error::from_errno();

    return Folder { path, dir, fd };
}

ErrorOr<Folder> Folder::open_relative_to(Folder const& root_folder,
    StringView path)
{
    let fd = TRY(Core::System::openat(root_folder.fd(), path));
    GuardDefer close_fd = [=] {
        ::close(fd);
    };
    let file_info = TRY(Core::System::fstat(fd));
    if (!file_info.is_folder())
        return Error::from_string_literal("file is not a folder");
    close_fd.let_down_guard();

    var dir = fdopendir(fd);
    if (!dir)
        return Error::from_errno();

    return Folder { path, dir, fd };
}

ErrorOr<View<StringView>> Folder::file_names()
{
    if (m_file_names.has_value())
        return m_file_names->view();
    m_file_names = TRY(Vector<StringView>::create());

    struct dirent* dirent = nullptr;
    while ((dirent = readdir(m_dir)) != NULL)
        TRY(m_file_names->append(dirent->d_name));
    rewinddir(m_dir);

    return m_file_names->view();
}

void Folder::close() const
{
    if (is_valid()) {
        // ::close(m_fd);
        closedir(m_dir);
        invalidate();
    }
}
}
