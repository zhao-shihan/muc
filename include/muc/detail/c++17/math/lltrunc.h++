#pragma once

#include <cassert>
#include <limits>
#include <type_traits>

namespace muc {

/// @brief A fast implementation of std::trunc but never raise floating point
/// exceptions and returns a long long int. Computes the nearest integer not
/// greater in magnitude than x.
/// @tparam T floating-point type
/// @param x floating-point value
/// @return the nearest integer value not greater in magnitude than x
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
constexpr auto lltrunc(T x) -> long long {
    assert(static_cast<T>(std::numeric_limits<long long>::min()) < x and
           x < static_cast<T>(std::numeric_limits<long long>::max()));
    return x;
}

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::lltrunc(-3.) == -3);
static_assert(muc::lltrunc(-2.) == -2);
static_assert(muc::lltrunc(-1.) == -1);
static_assert(muc::lltrunc(+0.) == +0);
static_assert(muc::lltrunc(+1.) == +1);
static_assert(muc::lltrunc(+2.) == +2);
static_assert(muc::lltrunc(+3.) == +3);

static_assert(muc::lltrunc(-3.3) == -3);
static_assert(muc::lltrunc(-2.3) == -2);
static_assert(muc::lltrunc(-1.3) == -1);
static_assert(muc::lltrunc(+0.3) == +0);
static_assert(muc::lltrunc(+1.3) == +1);
static_assert(muc::lltrunc(+2.3) == +2);
static_assert(muc::lltrunc(+3.3) == +3);

static_assert(muc::lltrunc(-3.5) == -3);
static_assert(muc::lltrunc(-2.5) == -2);
static_assert(muc::lltrunc(-1.5) == -1);
static_assert(muc::lltrunc(+0.5) == +0);
static_assert(muc::lltrunc(+1.5) == +1);
static_assert(muc::lltrunc(+2.5) == +2);
static_assert(muc::lltrunc(+3.5) == +3);

static_assert(muc::lltrunc(-3.7) == -3);
static_assert(muc::lltrunc(-2.7) == -2);
static_assert(muc::lltrunc(-1.7) == -1);
static_assert(muc::lltrunc(+0.7) == +0);
static_assert(muc::lltrunc(+1.7) == +1);
static_assert(muc::lltrunc(+2.7) == +2);
static_assert(muc::lltrunc(+3.7) == +3);

#endif
