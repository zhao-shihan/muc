#pragma once

#include <type_traits>

namespace muc {

template<typename T>
struct is_scoped_enum
    : std::bool_constant<
          std::is_enum_v<T> and
          not std::is_convertible_v<T, std::underlying_type_t<T>>> {};

template<typename T>
inline constexpr bool is_scoped_enum_v{is_scoped_enum<T>::value};

} // namespace muc
