#pragma once

#include "muc/impl/c++20/tuple/to_std_tuple.h++"
#include "muc/impl/c++20/tuple/tuple_like.h++"

#include <concepts>
#include <tuple>
#include <type_traits>

namespace muc {

namespace internal {

template<tuple_like, typename>
struct std_tuple_count;

template<typename... Ts, typename T>
struct std_tuple_count<std::tuple<Ts...>, T>
    : std::integral_constant<std::size_t, (... + static_cast<std::size_t>(
                                                     std::same_as<T, Ts>))> {};

} // namespace internal

template<tuple_like T, typename U>
struct tuple_count : internal::std_tuple_count<to_std_tuple_t<T>, U> {};

template<tuple_like T, typename U>
inline constexpr auto tuple_count_v{tuple_count<T, U>::value};

} // namespace muc
