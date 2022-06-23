#pragma once
#include <JR/Vector.h>

#if 0
template <typename T>
class ConcurrentVector {
public:
    static ErrorOr<ConcurrentVector> create();
    void unchecked_append(T&& value);
    ErrorOr<void> ensure_capacity(u32 capacity);

private:
};
#endif

template<typename T>
using ConcurrentVector = Vector<T>;
