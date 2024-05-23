#ifndef MUC_TEST_UNIVERSE
#include "muc/math"
#include "muc/numeric"
#else
#include "muc-universe"
#endif

#include <iomanip>
#include <iostream>
#include <iterator>
#include <valarray>

#define MUC_TEST_MATH_SCALAR(expr) \
    { std::cout << #expr " = " << expr << '\n'; }

#define MUC_TEST_MATH_VECTOR(expr)              \
    {                                           \
        const auto result{expr};                \
        std::cout << #expr " = [ ";             \
        for (int i{}; i < result.size(); ++i) { \
            std::cout << result[i] << ' ';      \
        }                                       \
        std::cout << "]\n";                     \
    }

auto main() -> int {
    std::cout << std::setprecision(17);

#if __cplusplus >= 202002L
    MUC_TEST_MATH_SCALAR(muc::clamp<"()">(0., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"()">(1., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"()">(2., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"()">(3., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"()">(4., 1., 3.))

    MUC_TEST_MATH_SCALAR(muc::clamp<"(]">(0., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"(]">(1., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"(]">(2., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"(]">(3., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"(]">(4., 1., 3.))

    MUC_TEST_MATH_SCALAR(muc::clamp<"[)">(0., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[)">(1., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[)">(2., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[)">(3., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[)">(4., 1., 3.))

    MUC_TEST_MATH_SCALAR(muc::clamp<"[]">(0., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[]">(1., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[]">(2., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[]">(3., 1., 3.))
    MUC_TEST_MATH_SCALAR(muc::clamp<"[]">(4., 1., 3.))
#endif

    MUC_TEST_MATH_SCALAR(muc::midpoint(2, 5))
    MUC_TEST_MATH_SCALAR(muc::midpoint(0, 2))
    MUC_TEST_MATH_SCALAR(muc::midpoint(0, -4))
    MUC_TEST_MATH_SCALAR(muc::midpoint(2, -5))
    MUC_TEST_MATH_SCALAR(muc::midpoint(0., 1.))
    MUC_TEST_MATH_SCALAR(muc::midpoint(-42., 42.))

    MUC_TEST_MATH_SCALAR(muc::lerp(5u, 2u, -0.5))
    MUC_TEST_MATH_SCALAR(muc::lerp(5u, 2u, 0))
    MUC_TEST_MATH_SCALAR(muc::lerp(5u, 2u, 0.5))
    MUC_TEST_MATH_SCALAR(muc::lerp(5u, 2u, 1))
    MUC_TEST_MATH_SCALAR(muc::lerp(5u, 2u, 1.5))

    MUC_TEST_MATH_SCALAR(muc::lerp(0, 0, -0.5))
    MUC_TEST_MATH_SCALAR(muc::lerp(0, 0, 0))
    MUC_TEST_MATH_SCALAR(muc::lerp(0, 0, 0.5))
    MUC_TEST_MATH_SCALAR(muc::lerp(0, 0, 1))
    MUC_TEST_MATH_SCALAR(muc::lerp(0, 0, 1.5))

    const std::valarray u{0., 1.};
    const std::valarray v{1., 0.};

    MUC_TEST_MATH_VECTOR(muc::midpoint(u, v))
    MUC_TEST_MATH_VECTOR(muc::lerp(u, v, -0.5))
    MUC_TEST_MATH_VECTOR(muc::lerp(u, v, 0))
    MUC_TEST_MATH_VECTOR(muc::lerp(u, v, 0.5))
    MUC_TEST_MATH_VECTOR(muc::lerp(u, v, 1))
    MUC_TEST_MATH_VECTOR(muc::lerp(u, v, 1.5))
}
