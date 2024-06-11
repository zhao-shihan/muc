#pragma once

#include <utility>

namespace muc {

/// @brief `muc::identity` is a function object type whose operator() returns
/// its argument unchanged. The effect is the same as It is a backport of
/// `std::identity`.
struct identity {
    using is_transparent = struct {};

    template<typename T>
    [[nodiscard]] constexpr auto operator()(T&& t) const noexcept -> T&& {
        return std::forward<T>(t);
    }
};

} // namespace muc
