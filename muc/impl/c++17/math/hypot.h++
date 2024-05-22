#pragma once

#include <cmath>
#include <type_traits>

namespace muc {

template<typename... Ts, std::enable_if_t<((sizeof...(Ts) >= 2) and ... and
                                           std::is_floating_point_v<Ts>),
                                          bool> = true>
constexpr auto hypot2(Ts... x) -> auto {
    return (... + (x * x));
}

template<typename... Ts, std::enable_if_t<((sizeof...(Ts) >= 2) and ... and
                                           std::is_floating_point_v<Ts>),
                                          bool> = true>
auto hypot(Ts... x) -> auto {
    return std::sqrt(muc::hypot2(x...));
}

} // namespace muc
