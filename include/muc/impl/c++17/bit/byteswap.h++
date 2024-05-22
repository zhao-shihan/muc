#pragma once

#include "muc/impl/c++17/bit/bit_cast.h++"

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace muc {

/// @brief Reverses the bytes in the given integer value.
/// @tparam T value type
/// @param value integer value
/// @return An integer value of type T whose object representation comprises the
/// bytes of that of value in reversed order.
template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
auto byteswap(T value) noexcept -> T {
    static_assert(std::has_unique_object_representations_v<T>,
                  "T may not have padding bits");
    auto bit{muc::bit_cast<std::array<std::byte, sizeof(T)>>(value)};
    std::reverse(bit.begin(), bit.end());
    return muc::bit_cast<T>(bit);
}

} // namespace muc
