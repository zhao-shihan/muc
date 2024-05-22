#pragma once

#include <cinttypes>
#include <cstdlib>
#include <limits>
#include <type_traits>

#if __cplusplus >= 202002L // >= C++20
#    include <version>
#    ifdef __cpp_lib_constexpr_cmath // >= C++23
#        define MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
#    endif
#endif

#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
#    include <cmath>
#endif

namespace muc {

///////////////////////////////////////////////////////////////////////////////
// div
///////////////////////////////////////////////////////////////////////////////

constexpr auto div(int x, int y) -> std::div_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::div(x, y);
#else // backport
    std::div_t result{};
    result.quot = x / y;
    result.rem = x % y;
    return result;
#endif
}

constexpr auto div(long x, long y) -> std::ldiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::div(x, y);
#else // backport
    std::ldiv_t result{};
    result.quot = x / y;
    result.rem = x % y;
    return result;
#endif
}

constexpr auto div(long long x, long long y) -> std::lldiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::div(x, y);
#else // backport
    std::lldiv_t result{};
    result.quot = x / y;
    result.rem = x % y;
    return result;
#endif
}

constexpr auto ldiv(long x, long y) -> std::ldiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::ldiv(x, y);
#else // backport
    return muc::div(x, y);
#endif
}

constexpr auto lldiv(long long x, long long y) -> std::lldiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::lldiv(x, y);
#else // backport
    return muc::div(x, y);
#endif
}

constexpr auto imaxdiv(std::intmax_t x, std::intmax_t y) -> std::imaxdiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::imaxdiv(x, y);
#else // backport
    std::imaxdiv_t result{};
    result.quot = x / y;
    result.rem = x % y;
    return result;
#endif
}

template<typename IntMax,
         std::enable_if_t<std::is_same_v<IntMax, std::intmax_t> and
                              (sizeof(IntMax) > sizeof(long long)),
                          bool> = true>
constexpr auto div(IntMax x, IntMax y) -> std::imaxdiv_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::div(x, y);
#else // backport
    return muc::imaxdiv(x, y);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// floating point abs
///////////////////////////////////////////////////////////////////////////////

template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto abs(T x) -> T {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::abs(x);
#else // backport
#    if defined __clang__ or defined __GNUC__
    if constexpr (std::is_same_v<T, double>) {
        return __builtin_fabs(x);
    } else if constexpr (std::is_same_v<T, float>) {
        return __builtin_fabsf(x);
    } else if constexpr (std::is_same_v<T, long double>) {
        return __builtin_fabsl(x);
    }
#    else
    return x >= 0 ? x : -x;
#    endif
#endif
}

template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto fabs(T x) -> T {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::fabs(x);
#else // backport
    return muc::abs(x);
#endif
}

constexpr auto fabsf(float x) -> float {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::fabsf(x);
#else // backport
    return muc::abs(x);
#endif
}

constexpr auto fabsl(long double x) -> long double {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::fabsl(x);
#else // backport
    return muc::abs(x);
#endif
}

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr auto fabs(T n) -> double {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::fabs(n);
#else // backport
    return muc::abs(static_cast<double>(n));
#endif
}

///////////////////////////////////////////////////////////////////////////////
// integral abs
///////////////////////////////////////////////////////////////////////////////

constexpr auto abs(int n) -> int {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::abs(n);
#else // backport
    return n >= 0 ? n : -n;
#endif
}

constexpr auto abs(long n) -> long {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::abs(n);
#else // backport
    return n >= 0 ? n : -n;
#endif
}

constexpr auto abs(long long n) -> long long {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::abs(n);
#else // backport
    return n >= 0 ? n : -n;
#endif
}

constexpr auto labs(long n) -> long {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::labs(n);
#else // backport
    return muc::abs(n);
#endif
}

constexpr auto llabs(long long n) -> long long {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::llabs(n);
#else // backport
    return muc::abs(n);
#endif
}

constexpr auto imaxabs(std::intmax_t n) -> std::intmax_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::imaxabs(n);
#else // backport
    return n >= 0 ? n : -n;
#endif
}

template<typename IntMax,
         std::enable_if_t<std::is_same_v<IntMax, std::intmax_t> and
                              (sizeof(IntMax) > sizeof(long long)),
                          bool> = true>
constexpr auto abs(IntMax n) -> std::intmax_t {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::abs(n);
#else // backport
    return muc::imaxabs(n);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// isnan
///////////////////////////////////////////////////////////////////////////////

template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto isnan(T x) -> bool {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::isnan(x);
#else // backport
    return x != x;
#endif
}

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr auto isnan(T n) -> bool {
#ifdef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
    return std::isnan(n);
#else // backport
    if constexpr (std::numeric_limits<double>::is_iec559) {
        return false;
    } else {
        return muc::isnan(static_cast<double>(n));
    }
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_HAS_CONSTEXPR_CMATH
