#pragma once
#include <JR/ErrorOr.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace Core::System {

ErrorOr<int> openat(int folder_fd, StringView filename,
    int oflag = O_RDONLY);

ErrorOr<int> open(StringView filename, int oflag = O_RDONLY);

ErrorOr<void> close(int fd);

struct Stat {
    constexpr bool is_regular_file() const
    {
        return S_ISREG(raw_stat.st_mode);
    }
    
    constexpr bool is_folder() const
    {
        return S_ISDIR(raw_stat.st_mode);
    }

    constexpr var size() const { return raw_stat.st_size; }
    struct stat raw_stat;
};
ErrorOr<Stat> fstat(int fd);

ErrorOr<void*> mmap(
    size_t len, int fd,
    int prot = PROT_READ,
    int flags = MAP_PRIVATE,
    __off_t offset = 0,
    void* address_hint = nullptr);

}
