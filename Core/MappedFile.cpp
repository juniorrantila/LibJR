#include "Core/System.h"
#include <Core/Folder.h>
#include <Core/MappedFile.h>
#include <JR/Defer.h>
#include <JR/Keywords.h>
#include <JR/String.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Core {

void MappedFile::close() const
{
    if (is_valid()) {
        munmap((void*)m_data, m_size);
        invalidate();
    }
}

ErrorOr<MappedFile> MappedFile::open(StringView path)
{
    let fd = TRY(Core::System::open(path));
    Defer close_fd = [=] {
        ::close(fd);
    };
    let file_info = TRY(Core::System::fstat(fd));
    if (!file_info.is_regular_file())
        return Error::from_string_literal("file is not a regular file");
    let data = TRY(Core::System::mmap(file_info.size(),
        fd));
    return MappedFile {
        path,
        (c_string)data,
        (u32)file_info.size()
    };
}

ErrorOr<MappedFile> MappedFile::open_relative_to(
    Folder const& folder,
    StringView path)
{
    let fd = TRY(Core::System::openat(folder.fd(), path));
    Defer close_fd = [=] {
        ::close(fd);
    };
    let file_info = TRY(Core::System::fstat(fd));
    let data = TRY(Core::System::mmap(file_info.size(), fd));
    return MappedFile {
        path,
        (c_string)data,
        (u32)file_info.size()
    };
}

Maybe<StringView> MappedFile::read_line()
{
    if (eof())
        return {};
    let start = m_cursor;
    u32 end = start;
    for (; end < m_size; end++) {
        if (m_data[end] == '\n')
            break;
    }
    m_cursor = end + 1;
    return StringView(&m_data[start], end-start);
}

}
