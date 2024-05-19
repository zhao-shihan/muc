#pragma once

#include <cstring>
#include <type_traits>

namespace muc {

template<typename To, typename From>
auto bit_cast(const From& src) noexcept -> std::enable_if_t<
                                            sizeof(To) == sizeof(From) and
                                                std::is_trivially_copyable_v<From> and
                                                std::is_trivially_copyable_v<To>,
                                            To> {
    static_assert(std::is_trivially_constructible_v<To>,
                  "This implementation additionally requires "
                  "destination type to be trivially constructible");
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace muc
