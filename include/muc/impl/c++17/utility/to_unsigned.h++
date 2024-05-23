#pragma once

#include <type_traits>

namespace muc {

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr auto to_unsigned(T k) -> std::make_unsigned_t<T> {
    return k;
}

} // namespace muc
