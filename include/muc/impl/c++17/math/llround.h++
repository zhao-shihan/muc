#pragma once

#include <cassert>
#include <limits>
#include <type_traits>

namespace muc {

/// @brief A fast implementation of std::llround but never raise floating point
/// exceptions. Computes the nearest integer value to x (in floating-point
/// format), rounding halfway cases away from zero.
/// @tparam T floating-point type
/// @param x floating-point value
/// @return the nearest integer value to x, rounding halfway cases away from
/// zero
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto llround(T x) -> long long {
    assert(static_cast<T>(std::numeric_limits<long long>::min()) < x and
           x < static_cast<T>(std::numeric_limits<long long>::max()));
    return x + (x >= 0 ? 0.5 : -0.5);
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::llround(-3.) == -3);
static_assert(muc::llround(-2.) == -2);
static_assert(muc::llround(-1.) == -1);
static_assert(muc::llround(+0.) == +0);
static_assert(muc::llround(+1.) == +1);
static_assert(muc::llround(+2.) == +2);
static_assert(muc::llround(+3.) == +3);

static_assert(muc::llround(-3.3) == -3);
static_assert(muc::llround(-2.3) == -2);
static_assert(muc::llround(-1.3) == -1);
static_assert(muc::llround(+0.3) == +0);
static_assert(muc::llround(+1.3) == +1);
static_assert(muc::llround(+2.3) == +2);
static_assert(muc::llround(+3.3) == +3);

static_assert(muc::llround(-3.5) == -4);
static_assert(muc::llround(-2.5) == -3);
static_assert(muc::llround(-1.5) == -2);
static_assert(muc::llround(+0.5) == +1);
static_assert(muc::llround(+1.5) == +2);
static_assert(muc::llround(+2.5) == +3);
static_assert(muc::llround(+3.5) == +4);

static_assert(muc::llround(-3.7) == -4);
static_assert(muc::llround(-2.7) == -3);
static_assert(muc::llround(-1.7) == -2);
static_assert(muc::llround(+0.7) == +1);
static_assert(muc::llround(+1.7) == +2);
static_assert(muc::llround(+2.7) == +3);
static_assert(muc::llround(+3.7) == +4);

#endif
