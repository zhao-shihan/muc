#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace muc {

/// @brief muc::multidentity is a function object type whose operator() returns
/// its arguments in a `std::tuple`. The effect is the same as
/// `std::forward_as_tuple`.
/// @warning Passing temporary objects as arguments is errorous and will produce
/// `std::tuple` holding dangling references.
struct multidentity {
    template<typename... Ts, std::enable_if_t<sizeof...(Ts) >= 1, bool> = true>
    [[nodiscard]] constexpr auto
    operator()(Ts&&... ts) const noexcept -> std::tuple<Ts&&...> {
        return std::forward_as_tuple<Ts...>(ts...);
    }
};

} // namespace muc
