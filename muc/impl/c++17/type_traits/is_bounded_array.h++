#pragma once

#include <cstddef>
#include <type_traits>

namespace muc {

template<typename T>
struct is_bounded_array : std::false_type {};

template<typename T, std::size_t N>
struct is_bounded_array<T[N]> : std::true_type {};

template<typename T>
inline constexpr bool is_bounded_array_v{is_bounded_array<T>::value};

} // namespace muc
