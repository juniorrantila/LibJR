#pragma once
#include <type_traits>

template<typename T>
concept Number = std::is_integral_v<T> or std::is_floating_point_v<T>;

