#pragma once
#include <JR/Types.h>
#include <unistd.h>

namespace Core {

class Threads {
public:
    static u16 in_machine() {
        return sysconf(_SC_NPROCESSORS_ONLN);
    }
};

}
