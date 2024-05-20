#pragma once

#include <type_traits>

namespace muc {

/// @brief Calculate m^N, where both m and N are integers.
/// @tparam T integral type
/// @tparam N power, N>=0
/// @param m base
/// @return m^N of type T.
template<int N, typename T,
         std::enable_if_t<N >= 0 and std::is_integral_v<T>, bool> = true>
constexpr auto ipow(T m) -> T {
    if constexpr (N == 0) {
        return 1;
    }
    if constexpr (N == 1) {
        return m;
    }
    if constexpr (N % 2 == 0) {
        const auto k{muc::ipow<N / 2>(m)};
        return k * k;
    }
    if constexpr (N % 3 == 0) {
        const auto k{muc::ipow<N / 3>(m)};
        return k * k * k;
    }
    const auto k{muc::ipow<(N - 1) / 2>(m)};
    return k * m * k;
}

/// @brief Calculate x^N, where x is an floating point and N is an integer.
/// @tparam T floating point type
/// @tparam N power
/// @param x base
/// @return x^N
template<int N, typename T,
         std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto pow(T x) -> T {
    if constexpr (N < 0) {
        return muc::pow<-N>(1 / x);
    }
    if constexpr (N == 0) {
        return 1;
    }
    if constexpr (N == 1) {
        return x;
    }
    if constexpr (N % 2 == 0) {
        const auto u{muc::pow<N / 2>(x)};
        return u * u;
    }
    if constexpr (N % 3 == 0) {
        const auto u{muc::pow<N / 3>(x)};
        return u * u * u;
    }
    const auto u{muc::pow<(N - 1) / 2>(x)};
    return u * x * u;
}

/// @brief Calculate x^N, where x is an integral converted to floating point, N is an integer.
/// @tparam T floating point type
/// @tparam N power
/// @param x base
/// @return x^N
template<int N, typename T = double, typename U,
         std::enable_if_t<std::is_floating_point_v<T> and
                              std::is_integral_v<T>,
                          bool> = true>
constexpr auto pow(U x) -> T {
    return muc::pow<N>(static_cast<T>(x));
}

} // namespace muc
