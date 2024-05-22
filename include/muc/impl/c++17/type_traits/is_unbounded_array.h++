#pragma once

#include <type_traits>

namespace muc {

template<class T>
struct is_unbounded_array : std::false_type {};

template<class T>
struct is_unbounded_array<T[]> : std::true_type {};

template<typename T>
inline constexpr bool is_unbounded_array_v{is_unbounded_array<T>::value};

} // namespace muc
