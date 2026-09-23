#include "muc/random"

#if __cplusplus >= 202002L

#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>

#define MUC_TEST_CHECK(expr)                                            \
    {                                                                   \
        if (not(expr)) {                                                \
            std::cerr << "Check failed: " #expr " at line " << __LINE__ \
                      << '\n';                                          \
            return 1;                                                   \
        }                                                               \
    }

auto test_urbg_ref() -> int {
    // Power-of-two span with segment width equal to the output width
    // consumes exactly one generator call per wrapper call.
    {
        std::mt19937 a{42}, b{42};
        const muc::urbg32_ref g{a};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(b()));
        }
    }
    // Power-of-two span composed from two segments, first drawn segment
    // most significant.
    {
        std::mt19937 a{42}, b{42};
        const muc::urbg64_ref g{a};
        for (int i{}; i < 8; ++i) {
            const auto hi{static_cast<std::uint64_t>(b())};
            const auto lo{static_cast<std::uint64_t>(b())};
            MUC_TEST_CHECK(g() == ((hi << 32) | lo));
        }
    }
    // Segments wider than the output type keep their high bits.
    {
        std::mt19937_64 a{7}, b{7};
        const muc::urbg32_ref g{a};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(b() >> 32));
        }
    }
    // Non-power-of-two spans go through rejection sampling and still feed
    // standard distributions through the concept.
    {
        std::minstd_rand a{1};
        muc::urbg64_ref g{a};
        std::uniform_int_distribution<int> d{0, 9};
        for (int i{}; i < 100; ++i) {
            const auto v{d(g)};
            MUC_TEST_CHECK(v >= 0 and v <= 9);
        }
        std::uint64_t acc{};
        for (int i{}; i < 64; ++i) {
            acc ^= g();
        }
        MUC_TEST_CHECK(acc != 0);
    }
    // target() recovers the referenced generator by type.
    {
        std::mt19937 a{3};
        const muc::urbg32_ref g{a};
        MUC_TEST_CHECK(g.target<std::mt19937>() == &a);
        MUC_TEST_CHECK(g.target<std::minstd_rand>() == nullptr);
    }
    return 0;
}

auto test_rng_ref() -> int {
    // seed() and discard() forward to the referenced engine.
    {
        std::mt19937 a{}, b{};
        muc::rng64_ref r{a};
        r.seed(42);
        b.seed(42);
        r.discard(5);
        b.discard(5);
        const auto hi{static_cast<std::uint64_t>(b())};
        const auto lo{static_cast<std::uint64_t>(b())};
        MUC_TEST_CHECK(r() == ((hi << 32) | lo));
    }
    // Equality compares the state of the referenced engines.
    {
        std::mt19937 a{3}, b{3};
        muc::rng64_ref ra{a}, rb{b};
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(not(ra != rb));
        ra.discard(1);
        MUC_TEST_CHECK(ra != rb);
        const muc::rng64_ref ra2{a};
        MUC_TEST_CHECK(ra == ra2);
    }
    // Wrappers over different engine types never compare equal.
    {
        std::mt19937 a{3};
        std::minstd_rand b{3};
        const muc::rng64_ref ra{a}, rb{b};
        MUC_TEST_CHECK(ra != rb);
    }
    // Narrow stream round-trip restores the engine state.
    {
        std::mt19937 a{9}, b{9};
        muc::rng64_ref ra{a}, rb{b};
        ra.discard(3);
        std::ostringstream os;
        os << ra;
        std::istringstream is{os.str()};
        is >> rb;
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
    }
    // Wide stream round-trip restores the engine state.
    {
        std::mt19937 a{9}, b{9};
        muc::rng64_ref ra{a}, rb{b};
        ra.discard(3);
        std::wostringstream os;
        os << ra;
        std::wistringstream is{os.str()};
        is >> rb;
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
    }
    // Rejection-sampling engines support the engine operations as well.
    {
        std::minstd_rand a{11}, b{11};
        muc::rng32_ref ra{a}, rb{b};
        ra.discard(6);
        std::ostringstream os;
        os << ra;
        std::istringstream is{os.str()};
        is >> rb;
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
    }
    // Implicit conversion to basic_urbg_ref shares the referenced engine.
    {
        std::mt19937 a{5}, b{5};
        muc::rng64_ref r{a};
        const muc::urbg64_ref g{r};
        const auto x{g()};
        const auto hi{static_cast<std::uint64_t>(b())};
        const auto lo{static_cast<std::uint64_t>(b())};
        MUC_TEST_CHECK(x == ((hi << 32) | lo));
    }
    // target() recovers the referenced engine by type.
    {
        std::mt19937 a{8};
        const muc::rng64_ref r{a};
        MUC_TEST_CHECK(r.target<std::mt19937>() == &a);
        MUC_TEST_CHECK(r.target<std::minstd_rand>() == nullptr);
    }
    return 0;
}

#endif

auto main() -> int {
#if __cplusplus >= 202002L
    if (const auto ec{test_urbg_ref()}; ec != 0) {
        return ec;
    }
    if (const auto ec{test_rng_ref()}; ec != 0) {
        return ec;
    }
#endif
}
