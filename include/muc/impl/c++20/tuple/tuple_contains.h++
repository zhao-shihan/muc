#pragma once

#include "muc/impl/c++20/tuple/tuple_count.h++"
#include "muc/impl/c++20/tuple/tuple_like.h++"

#include <type_traits>

namespace muc {

template<tuple_like T, typename U>
struct tuple_contains :
    std::conditional_t<tuple_count_v<T, U> >= 1, std::true_type,
                       std::false_type> {};

template<tuple_like T, typename U>
inline constexpr auto tuple_contains_v = tuple_contains<T, U>::value;

template<tuple_like T, typename U>
struct tuple_contains_unique :
    std::conditional_t<tuple_count_v<T, U> == 1, std::true_type,
                       std::false_type> {};

template<tuple_like T, typename U>
inline constexpr auto tuple_contains_unique_v{
    tuple_contains_unique<T, U>::value};

} // namespace muc
