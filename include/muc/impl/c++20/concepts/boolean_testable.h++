#pragma once

#include <concepts>
#include <utility>

namespace muc {

template<typename B>
concept boolean_testable = requires(B&& b) {
    requires std::convertible_to<B, bool>;
    { not std::forward<B>(b) } -> std::convertible_to<bool>;
};

} // namespace muc
