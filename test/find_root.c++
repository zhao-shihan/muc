#include "muc/math"

#include <cmath>
#include <iostream>

#define MUC_TEST_NEWTON(f, df, x0)                                   \
    {                                                                \
        const auto [x, converged]{muc::find_root::newton_raphson(    \
            [](auto x) {                                             \
                return f;                                            \
            },                                                       \
            [](auto x) {                                             \
                return df;                                           \
            },                                                       \
            x0)};                                                    \
        std::cout << #f " = 0  (x0 = " #x0 ") ->  x = " << x << " (" \
                  << converged << ")\n";                             \
    }

#define MUC_TEST_SECANT(f, x0)                                       \
    {                                                                \
        const auto [x, converged]{muc::find_root::secant(            \
            [](auto x) {                                             \
                return f;                                            \
            },                                                       \
            x0)};                                                    \
        std::cout << #f " = 0  (x0 = " #x0 ") ->  x = " << x << " (" \
                  << converged << ")\n";                             \
    }

auto main() -> int {
    MUC_TEST_NEWTON(x * x - 1, 2 * x, 0.5)
    MUC_TEST_NEWTON(x * x - 1, 2 * x, 5.)
    MUC_TEST_NEWTON(x * x - 1, 2 * x, 100.)

    MUC_TEST_SECANT(x * x - 1, 0.5)
    MUC_TEST_SECANT(x * x - 1, 5.)
    MUC_TEST_SECANT(x * x - 1, 100.)
}
