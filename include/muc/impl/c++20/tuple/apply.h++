#pragma once

#include "muc/impl/c++20/tuple/tuple_like.h++"

#include <functional>
#include <utility>

namespace muc {

namespace internal {

template<typename F, typename T, std::size_t... Is>
    requires tuple_like<std::decay_t<T>>
constexpr auto apply(F&& f, T&& t, std::index_sequence<Is...>) noexcept(
    noexcept(std::invoke(std::forward<F>(f),
                         get<Is>(std::forward<T>(t))...))) -> decltype(auto) {
    return std::invoke(std::forward<F>(f), get<Is>(std::forward<T>(t))...);
}

} // namespace internal

template<typename F, typename T>
    requires tuple_like<std::decay_t<T>>
constexpr auto apply(F&& f, T&& t) noexcept(noexcept(internal::apply(
    std::forward<F>(f), std::forward<T>(t),
    std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{})))
    -> decltype(auto) {
    return internal::apply(
        std::forward<F>(f), std::forward<T>(t),
        std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});
}

} // namespace muc
