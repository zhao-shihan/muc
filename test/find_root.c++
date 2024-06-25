#ifndef MUC_TEST_UNIVERSE
#include "muc/numeric"
#else
#include "muc-universe"
#endif

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

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

#define MUC_TEST_ZBRENT(f, x1, x2)                                         \
    {                                                                      \
        const auto [x, converged]{muc::find_root::zbrent(                  \
            [](auto x) {                                                   \
                return f;                                                  \
            },                                                             \
            x1, x2)};                                                      \
        std::cout << #f " = 0  (x1 = " #x1 ", x2 = " #x2 ") ->  x = " << x \
                  << " (" << converged << ")\n";                           \
    }

auto main() -> int {
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);

    MUC_TEST_NEWTON(std::log(x) - 1, 1 / x, 0.5)
    MUC_TEST_NEWTON(std::log(x) - 1, 1 / x, 5.)
    MUC_TEST_NEWTON(std::log(x) - 1, 1 / x, 100.)

    MUC_TEST_SECANT(std::log(x) - 1, 0.5)
    MUC_TEST_SECANT(std::log(x) - 1, 5.)
    MUC_TEST_SECANT(std::log(x) - 1, 100.)

    MUC_TEST_ZBRENT(std::log(x) - 1, 0.1, 0.5)
    MUC_TEST_ZBRENT(std::log(x) - 1, 0.1, 5.)
    MUC_TEST_ZBRENT(std::log(x) - 1, 0.1, 100.)
}
