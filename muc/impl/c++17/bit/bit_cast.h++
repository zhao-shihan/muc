#pragma once

#include <cstring>
#include <type_traits>

namespace muc {

/// @brief Obtain a value of type To by reinterpreting the object representation
/// of From. Every bit in the value representation of the returned To object is
/// equal to the corresponding bit in the object representation of from. The
/// values of padding bits in the returned To object are unspecified.
/// @tparam To destination type
/// @tparam From source type
/// @param src the source of bits for the return value
/// @return An object of type To whose value representation is as described
/// above.
template<typename To, typename From>
auto bit_cast(const From& src) noexcept
    -> std::enable_if_t<sizeof(To) == sizeof(From) and
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
