#include <Core/System.h>
#include <JR/Defer.h>
#include <JR/String.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace Core::System {

ErrorOr<int> openat(int folder_fd, StringView filename,
    int oflag)
{
    let path = TRY(filename.to_string());
    Defer destroy_path = [&] {
        path.destroy();
    };
    var fd = ::openat(folder_fd, path.as_c_string(), oflag);
    if (fd == -1)
        return Error::from_errno();
    return fd;
}

ErrorOr<int> open(StringView filename, int oflag)
{
    let filepath = TRY(filename.to_string());
    var fd = ::open(filepath.as_c_string(), oflag);
    if (fd == -1)
        return Error::from_errno();
    return fd;
}

ErrorOr<void> close(int fd)
{
    if (::close(fd) == -1)
        return Error::from_errno();
    return {};
}

ErrorOr<Stat> fstat(int fd)
{
    struct stat st;
    if (fstat(fd, &st) == -1)
        return Error::from_errno();
    return Stat { st };
}

ErrorOr<void*> mmap(
    size_t len, int fd,
    int prot,
    int flags,
    __off_t offset,
    void* address_hint)
{
    var data = ::mmap(address_hint, len, prot, flags, fd, offset);
    if (data == MAP_FAILED)
        return Error::from_errno();
    return data;
}

}
