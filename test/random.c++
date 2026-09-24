#include "muc/random"

#if __cplusplus >= 202002L

#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <utility>

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
        const muc::urbg_ref g{a};
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
        muc::urbg_ref g{a};
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

auto test_any_urbg() -> int {
    // The wrapper generates exactly what basic_urbg_ref generates.
    {
        std::mt19937 a{42}, b{42};
        muc::any_urbg32 g{a};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(b()));
        }
    }
    {
        std::mt19937 a{42}, b{42};
        muc::any_urbg g{a};
        for (int i{}; i < 8; ++i) {
            const auto hi{static_cast<std::uint64_t>(b())};
            const auto lo{static_cast<std::uint64_t>(b())};
            MUC_TEST_CHECK(g() == ((hi << 32) | lo));
        }
    }
    // A const source is copied by state.
    {
        const std::mt19937 a{3};
        std::mt19937 b{3};
        muc::any_urbg32 g{a};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(b()));
        }
    }
    // Non-power-of-two spans go through rejection sampling and still feed
    // standard distributions through the concept.
    {
        std::minstd_rand a{1};
        muc::any_urbg g{a};
        std::uniform_int_distribution<int> d{0, 9};
        for (int i{}; i < 100; ++i) {
            const auto v{d(g)};
            MUC_TEST_CHECK(v >= 0 and v <= 9);
        }
    }
    // Copies own independent generator states.
    {
        std::mt19937 a{7};
        muc::any_urbg32 g{a}, h{a};
        muc::any_urbg32 k{g};
        MUC_TEST_CHECK(k() == h());
        g();
        MUC_TEST_CHECK(k() == h());
        MUC_TEST_CHECK(g() != k());
    }
    // Moving duplicates the state and leaves the source unchanged.
    {
        std::mt19937 a{5}, b{5}, c{5};
        muc::any_urbg32 g{a};
        muc::any_urbg32 k{std::move(g)};
        MUC_TEST_CHECK(k() == static_cast<std::uint32_t>(b()));
        MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(c()));
    }
    // Rebinding assignment replaces the owned generator.
    {
        std::mt19937 a{1};
        std::minstd_rand b{2};
        muc::any_urbg32 g{a};
        g = b;
        MUC_TEST_CHECK(g.target<std::minstd_rand>() != nullptr);
        MUC_TEST_CHECK(g.target<std::mt19937>() == nullptr);
        g = std::mt19937{1};
        MUC_TEST_CHECK(g.target<std::mt19937>() != nullptr);
    }
    // target() recovers the owned generator; ref() shares it.
    {
        std::mt19937 a{3}, b{3};
        muc::any_urbg32 g{a};
        MUC_TEST_CHECK(g.target<std::mt19937>() != nullptr);
        MUC_TEST_CHECK(g.target<std::minstd_rand>() == nullptr);
        const auto& cg{g};
        MUC_TEST_CHECK(cg.target<std::mt19937>() == g.target<std::mt19937>());
        auto r{g.ref()};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(r() == static_cast<std::uint32_t>(b()));
            MUC_TEST_CHECK(g() == static_cast<std::uint32_t>(b()));
        }
    }
    // swap exchanges the owned generators.
    {
        std::mt19937 a{1}, b{2};
        std::mt19937 c{1}, d{2};
        muc::any_urbg32 g{a}, h{b};
        muc::any_urbg32 k{c}, l{d};
        swap(g, h);
        MUC_TEST_CHECK(g() == l());
        MUC_TEST_CHECK(h() == k());
    }
    return 0;
}

auto test_rng_ref() -> int {
    // seed() and discard() forward to the referenced engine.
    {
        std::mt19937 a{}, b{};
        muc::rng_ref r{a};
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
        muc::rng_ref ra{a}, rb{b};
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(not(ra != rb));
        ra.discard(1);
        MUC_TEST_CHECK(ra != rb);
        const muc::rng_ref ra2{a};
        MUC_TEST_CHECK(ra == ra2);
    }
    // Wrappers over different engine types never compare equal.
    {
        std::mt19937 a{3};
        std::minstd_rand b{3};
        const muc::rng_ref ra{a}, rb{b};
        MUC_TEST_CHECK(ra != rb);
    }
    // Narrow stream round-trip restores the engine state.
    {
        std::mt19937 a{9}, b{9};
        muc::rng_ref ra{a}, rb{b};
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
        muc::rng_ref ra{a}, rb{b};
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
        muc::rng_ref r{a};
        const muc::urbg_ref g{r};
        const auto x{g()};
        const auto hi{static_cast<std::uint64_t>(b())};
        const auto lo{static_cast<std::uint64_t>(b())};
        MUC_TEST_CHECK(x == ((hi << 32) | lo));
    }
    // target() recovers the referenced engine by type.
    {
        std::mt19937 a{8};
        const muc::rng_ref r{a};
        MUC_TEST_CHECK(r.target<std::mt19937>() == &a);
        MUC_TEST_CHECK(r.target<std::minstd_rand>() == nullptr);
    }
    return 0;
}

// Satisfies muc::random_number_generator but not muc::random_number_engine:
// it has no default constructor and cannot be constructed from its
// result_type.
class seeded_lcg {
public:
    using result_type = std::uint32_t;

    struct seed_token {};

    explicit seeded_lcg(seed_token, result_type s) noexcept :
        m_state{s} {}

    auto operator()() noexcept -> result_type {
        return m_state = m_state * 1664525u + 1013904223u;
    }

    static constexpr auto min() noexcept -> result_type {
        return 0;
    }

    static constexpr auto max() noexcept -> result_type {
        return std::numeric_limits<result_type>::max();
    }

    auto seed() noexcept -> void {
        m_state = 0;
    }

    auto seed(result_type s) noexcept -> void {
        m_state = s;
    }

    auto discard(unsigned long long z) noexcept -> void {
        while (z-- != 0) {
            (*this)();
        }
    }

    friend auto operator==(const seeded_lcg& x, const seeded_lcg& y) noexcept
        -> bool {
        return x.m_state == y.m_state;
    }

    friend auto operator!=(const seeded_lcg& x, const seeded_lcg& y) noexcept
        -> bool {
        return not(x == y);
    }

    friend auto operator<<(std::ostream& os, const seeded_lcg& x)
        -> std::ostream& {
        return os << x.m_state;
    }

    friend auto operator>>(std::istream& is, seeded_lcg& x) -> std::istream& {
        return is >> x.m_state;
    }

    friend auto operator<<(std::wostream& os, const seeded_lcg& x)
        -> std::wostream& {
        return os << x.m_state;
    }

    friend auto operator>>(std::wistream& is, seeded_lcg& x) -> std::wistream& {
        return is >> x.m_state;
    }

private:
    result_type m_state;
};

auto test_rng_ref_engine_like() -> int {
    // basic_rng_ref requires only the operations it forwards, so it binds
    // to a generator that is not an engine.
    static_assert(muc::random_number_generator<seeded_lcg>);
    static_assert(not muc::random_number_engine<seeded_lcg>);
    static_assert(std::constructible_from<muc::rng_ref, seeded_lcg&>);
    // seed(), seed(s), discard() and equality forward to the referenced
    // generator.
    {
        seeded_lcg a{seeded_lcg::seed_token{}, 42};
        seeded_lcg b{seeded_lcg::seed_token{}, 42};
        muc::rng_ref ra{a}, rb{b};
        MUC_TEST_CHECK(ra == rb);
        ra.discard(3);
        MUC_TEST_CHECK(ra != rb);
        ra.seed(7);
        rb.seed(7);
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
        ra.seed();
        rb.seed();
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra.target<seeded_lcg>() == &a);
    }
    // Narrow stream round-trip restores the referenced generator state.
    {
        seeded_lcg a{seeded_lcg::seed_token{}, 5};
        seeded_lcg b{seeded_lcg::seed_token{}, 5};
        muc::rng32_ref ra{a}, rb{b};
        ra.discard(3);
        std::ostringstream os;
        os << ra;
        std::istringstream is{os.str()};
        is >> rb;
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
    }
    return 0;
}

auto test_any_rng() -> int {
    // Default construction owns a default-constructed D, and seed()
    // restores it: the random_number_engine postcondition e == E().
    {
        muc::any_rng r{}, s{};
        MUC_TEST_CHECK(r == s);
        std::minstd_rand b{};
        muc::rng_ref t{b};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(r() == t());
        }
        MUC_TEST_CHECK(r != s);
        r.seed();
        MUC_TEST_CHECK(r == s);
    }
    // Seed construction and seeding reproduce states and keep the engine
    // type.
    {
        muc::any_rng r{42u}, s{42u};
        MUC_TEST_CHECK(r == s);
        r();
        MUC_TEST_CHECK(r != s);
        r.seed(42u);
        MUC_TEST_CHECK(r == s);
        MUC_TEST_CHECK(r.target<std::minstd_rand>() != nullptr);
    }
    {
        muc::any_rng r{std::mt19937{9}}, s{std::mt19937{1}};
        r.seed(7);
        s.seed(7);
        MUC_TEST_CHECK(r == s);
        MUC_TEST_CHECK(r.target<std::mt19937>() != nullptr);
    }
    // seed(), discard() and generation forward to the owned engine.
    {
        std::mt19937 b{};
        muc::any_rng r{std::mt19937{}};
        r.seed(42);
        b.seed(42);
        r.discard(5);
        b.discard(5);
        const auto hi{static_cast<std::uint64_t>(b())};
        const auto lo{static_cast<std::uint64_t>(b())};
        MUC_TEST_CHECK(r() == ((hi << 32) | lo));
    }
    // Equality compares engine type and state; wrappers over different
    // engine types never compare equal.
    {
        muc::any_rng ra{std::mt19937{3}}, rb{std::mt19937{3}};
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(not(ra != rb));
        ra.discard(1);
        MUC_TEST_CHECK(ra != rb);
        const muc::any_rng ra2{ra};
        MUC_TEST_CHECK(ra == ra2);
        muc::any_rng rc{std::minstd_rand{3}}, rd{std::mt19937{3}};
        MUC_TEST_CHECK(rc != rd);
    }
    // Copies own independent engine states (value semantics).
    {
        seeded_lcg a{seeded_lcg::seed_token{}, 42};
        muc::any_rng ra{a}, rb{a};
        MUC_TEST_CHECK(ra == rb);
        ra.discard(3);
        MUC_TEST_CHECK(ra != rb);
        rb.discard(3);
        MUC_TEST_CHECK(ra == rb);
    }
    // Narrow stream round-trip restores the engine state.
    {
        muc::any_rng ra{std::mt19937{9}}, rb{std::mt19937{9}};
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
        muc::any_rng ra{std::mt19937{9}}, rb{std::mt19937{9}};
        ra.discard(3);
        std::wostringstream os;
        os << ra;
        std::wistringstream is{os.str()};
        is >> rb;
        MUC_TEST_CHECK(ra == rb);
        MUC_TEST_CHECK(ra() == rb());
    }
    // ref() shares the owned engine; target() recovers it.
    {
        seeded_lcg a{seeded_lcg::seed_token{}, 42};
        muc::any_rng r{a}, s{a};
        auto v{r.ref()};
        v.discard(2);
        s.discard(2);
        MUC_TEST_CHECK(r == s);
        MUC_TEST_CHECK(r.target<seeded_lcg>() != nullptr);
    }
    // any_urbg can wrap any_rng, narrowing to a pure generator.
    {
        std::mt19937 b{5};
        muc::any_urbg g{muc::any_rng{std::mt19937{5}}};
        muc::rng_ref t{b};
        for (int i{}; i < 8; ++i) {
            MUC_TEST_CHECK(g() == t());
        }
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
    if (const auto ec{test_rng_ref_engine_like()}; ec != 0) {
        return ec;
    }
    if (const auto ec{test_any_urbg()}; ec != 0) {
        return ec;
    }
    if (const auto ec{test_any_rng()}; ec != 0) {
        return ec;
    }
#endif
}
