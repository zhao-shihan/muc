#pragma once

#include "muc/ceta_string"

#include <cassert>
#include <cmath>
#include <concepts>
#include <type_traits>

namespace muc {

template<ceta_string<3> Mode, std::floating_point T1, std::floating_point T2, std::floating_point T3>
    requires(Mode == "[]" or
             Mode == "[)" or
             Mode == "(]" or
             Mode == "()")
constexpr auto clamp(T1 value, T2 low, T3 high) -> std::common_type_t<T1, T2, T3> {
    const std::common_type_t<T1, T2, T3> v{value};
    const std::common_type_t<T1, T2, T3> lo{low};
    const std::common_type_t<T1, T2, T3> hi{high};
    assert(lo < hi);
    if constexpr (Mode == "[]") {
        return v < lo ? lo :
               hi < v ? hi :
                        v;
    } else if constexpr (Mode == "[)") {
        return v < lo  ? lo :
               hi <= v ? std::nexttoward(hi, lo) :
                         v;
    } else if constexpr (Mode == "(]") {
        return v <= lo ? std::nexttoward(lo, hi) :
               hi < v  ? hi :
                         v;
    } else if constexpr (Mode == "()") {
        return v <= lo ? std::nexttoward(lo, hi) :
               hi <= v ? std::nexttoward(hi, lo) :
                         v;
    }
}

} // namespace muc
