#pragma once

#if __cplusplus >= 202002L // >= C++20
#include <version>
#ifdef __cpp_lib_forward_like // >= C++23
#define MUC_CPP_LIB_FORWARD_LIKE
#endif
#endif

#include <utility>

#ifndef MUC_CPP_LIB_FORWARD_LIKE // backport
#include <type_traits>
#endif

namespace muc {

template<typename T, typename U>
[[nodiscard]] constexpr auto forward_like(U&& x) noexcept -> auto&& {
#ifdef MUC_CPP_LIB_FORWARD_LIKE // C++23
    return std::forward_like<T, U>(x);
#else // backport
    constexpr bool is_adding_const =
        std::is_const_v<std::remove_reference_t<T>>;
    if constexpr (std::is_lvalue_reference_v<T&&>) {
        if constexpr (is_adding_const) {
            return std::as_const(x);
        } else {
            return static_cast<U&>(x);
        }
    } else {
        if constexpr (is_adding_const) {
            return std::move(std::as_const(x));
        } else {
            return std::move(x);
        }
    }
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_FORWARD_LIKE
