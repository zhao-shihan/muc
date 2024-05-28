#pragma once

#if __cplusplus >= 202002L // >= C++20
#include <version>
#ifdef __cpp_lib_to_underlying // >= C++23
#define MUC_CPP_LIB_TO_UNDERLYING
#endif
#endif

#ifdef MUC_CPP_LIB_TO_UNDERLYING
#include <utility>
#else // backport
#include <type_traits>
#endif

namespace muc {

template<typename E>
constexpr auto to_underlying(E value) noexcept -> std::underlying_type_t<E> {
#ifdef MUC_CPP_LIB_TO_UNDERLYING
    return std::to_underlying<E>(value);
#else // backport
    return static_cast<typename std::underlying_type_t<E>>(value);
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_TO_UNDERLYING
