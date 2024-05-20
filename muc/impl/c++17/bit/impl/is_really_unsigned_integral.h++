#pragma once

#include <type_traits>

namespace muc::impl {

template<typename T>
struct is_really_unsigned_integral
    : std::bool_constant<
          std::is_unsigned_v<T> and not std::is_same_v<T, bool> and
          not std::is_same_v<T, char> and not std::is_same_v<T, char8_t> and
          not std::is_same_v<T, char16_t> and
          not std::is_same_v<T, char32_t> and not std::is_same_v<T, wchar_t>> {
};

template<typename T>
inline constexpr auto is_really_unsigned_integral_v{
    is_really_unsigned_integral<T>::value};

} // namespace muc::impl
