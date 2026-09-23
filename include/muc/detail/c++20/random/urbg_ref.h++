// -*- C++ -*-
//
// Copyright (C) 2021-2026  Shihan Zhao
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "muc/detail/common/inline_macro.h++"

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <random>
#include <type_traits>

namespace muc {

namespace impl {

/// @brief Empty base class marking the muc random reference wrappers.
///
/// Used to keep `basic_urbg_ref` from wrapping another reference wrapper
/// instead of binding to its referenced generator.
struct random_ref_tag {};

/// @brief Type identity token, unique per T and requiring no RTTI.
///
/// Its address identifies T in `target()` and is a constant expression,
/// so type recovery needs neither RTTI nor function-local state.
template<typename T>
inline constexpr unsigned char type_token_v{0};

/// @brief Draws one uniformly distributed segment of n bits from g.
///
/// A segment is the zero-based value `g() - G::min()`, restricted by
/// rejection sampling to `[0, 2^n)` so that every n-bit pattern is equally
/// likely. When Pow2 is true, a segment already covers exactly n bits and
/// no draw is ever rejected.
/// @tparam Pow2 whether the span of g is a power of two
/// @tparam G type of the wrapped generator
/// @param g wrapped generator
/// @param lo `G::min()` widened to `std::uintmax_t`
/// @param n number of uniform bits provided by one accepted draw
/// @return a value uniformly distributed over `[0, 2^n)`
template<bool Pow2, typename G>
[[nodiscard]] MUC_ALWAYS_INLINE constexpr auto
draw_segment(G& g, std::uintmax_t lo, int n) -> std::uintmax_t {
    if constexpr (Pow2) {
        return static_cast<std::uintmax_t>(g()) - lo;
    } else {
        while (true) {
            const auto seg{static_cast<std::uintmax_t>(g()) - lo};
            if (seg < (std::uintmax_t{1} << n)) {
                return seg;
            }
        }
    }
}

/// @brief Generates one unbiased UInt value from the wrapped generator g.
///
/// Segments are concatenated into UInt with the first drawn segment placed
/// most significant; excess low bits of the last segment are dropped. The
/// result is uniformly distributed over `[0, 2^digits(UInt))`. Power-of-two
/// spans consume `ceil(digits(UInt) / n)` generator calls with n =
/// `bit_width(G::max() - G::min())`; other spans extract segments with
/// rejection sampling, consuming a variable number of calls.
///
/// `G::min()` and `G::max()` are constant expressions for every type
/// satisfying `std::uniform_random_bit_generator`, so every segment
/// parameter is a compile-time constant and the power-of-two and
/// wide-segment paths are selected with `if constexpr`.
/// @tparam G type of the wrapped generator
/// @tparam UInt unsigned output type
/// @param g wrapped generator
/// @return a value uniformly distributed over the full range of UInt
template<typename G, typename UInt>
[[nodiscard]] MUC_ALWAYS_INLINE constexpr auto generate(G& g) -> UInt {
    constexpr auto w{std::numeric_limits<UInt>::digits};
    constexpr auto lo{static_cast<std::uintmax_t>(G::min())};
    constexpr auto span_1{static_cast<std::uintmax_t>(G::max()) - lo};
    // (max - min + 1) is a power of two iff x & (x + 1) == 0 for
    // x = max - min; this includes the full range, where x + 1 wraps.
    constexpr auto pow2{(span_1 & (span_1 + 1)) == 0};
    // A power-of-two span contributes bit_width(max - min) uniform bits
    // per draw. Any other span contributes one bit less, so that the
    // rejection in draw_segment can trim it to a power-of-two size.
    constexpr auto n{std::bit_width(span_1) - (pow2 ? 0 : 1)};
    if constexpr (n >= w) {
        // One wide segment suffices; keep its high w bits.
        return static_cast<UInt>(draw_segment<pow2>(g, lo, n) >> (n - w));
    } else {
        auto acc{UInt{0}};
        auto filled{0};
        while (filled < w) {
            const auto seg{draw_segment<pow2>(g, lo, n)};
            const auto take{n < w - filled ? n : w - filled};
            acc |= static_cast<UInt>(seg >> (n - take)) << (w - filled - take);
            filled += take;
        }
        return acc;
    }
}

/// @brief Type-erased dispatch table of basic_urbg_ref.
template<typename UInt>
struct urbg_ref_ops {
    auto (*m_call)(void*) -> UInt;
    auto (*m_token)() -> const void*;
};

/// @brief Binds urbg_ref_ops to the concrete generator type G.
template<typename G, typename UInt>
struct urbg_ref_binding {
    static auto call(void* p) -> UInt {
        return generate<G, UInt>(*static_cast<G*>(p));
    }

    static constexpr auto token() -> const void* {
        return &type_token_v<std::remove_cvref_t<G>>;
    }

    static constexpr urbg_ref_ops<UInt> ops{&call, &token};
};

} // namespace impl

/// @brief Type-erased non-owning reference to a UniformRandomBitGenerator.
///
/// Binds a reference to any object whose type satisfies
/// `std::uniform_random_bit_generator` and erases that type. Copies of a
/// basic_urbg_ref share the referenced generator. The wrapper is never
/// empty: it is not default constructible and always holds a reference.
///
/// @tparam UInt unsigned output type, aka `result_type`
///
/// @par Generation
/// The wrapper itself satisfies `std::uniform_random_bit_generator`:
/// `result_type` is UInt, `min()` is 0 and `max()` is the maximum of
/// UInt, and each call returns a value uniformly distributed over that
/// full range. Outputs of the referenced generator are rebased to
/// zero-based segments and concatenated into UInt, first drawn segment
/// most significant; when one segment is wider than UInt, only its high
/// bits are kept. Generators whose span is not a power of two produce
/// segments with rejection sampling (expected two generator calls per
/// segment), so one call may consume several generator outputs.
///
/// @warning The referenced generator must outlive the wrapper and every
/// copy of it.
/// @see std::uniform_random_bit_generator, muc::basic_rng_ref
template<std::unsigned_integral UInt = std::uint64_t>
class basic_urbg_ref : impl::random_ref_tag {
public:
    /// @brief The type of the generated random values.
    using result_type = UInt;

    /// @brief Binds the wrapper to the generator g.
    /// @tparam G type of the referenced generator
    /// @param g generator to reference; must outlive the wrapper
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                    std::uniform_random_bit_generator<G>
    constexpr basic_urbg_ref(G& g) noexcept :
        m_ptr{&g},
        m_ops{&impl::urbg_ref_binding<G, UInt>::ops} {}

    /// @brief Generates a random value.
    /// @return a value uniformly distributed over `[min(), max()]`
    MUC_ALWAYS_INLINE auto operator()() const -> result_type {
        return m_ops->m_call(m_ptr);
    }

    /// @brief Returns the smallest value that can be generated.
    /// @return 0
    static constexpr auto min() noexcept -> result_type {
        return 0;
    }

    /// @brief Returns the largest value that can be generated.
    /// @return the maximum of result_type
    static constexpr auto max() noexcept -> result_type {
        return std::numeric_limits<result_type>::max();
    }

    /// @brief Recovers a pointer to the referenced generator.
    /// @tparam E type to look up
    /// @return a pointer to the referenced object if it has type E,
    /// otherwise nullptr
    template<typename E>
    [[nodiscard]] auto target() const noexcept -> E* {
        return m_ops->m_token() == &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<E*>(m_ptr) :
                   nullptr;
    }

private:
    template<std::unsigned_integral>
    friend class basic_rng_ref;

    constexpr basic_urbg_ref(void* ptr,
                             const impl::urbg_ref_ops<UInt>* ops) noexcept :
        m_ptr{ptr},
        m_ops{ops} {}

    void* m_ptr;
    const impl::urbg_ref_ops<UInt>* m_ops;
};

/// @brief `basic_urbg_ref` with `std::uint32_t` output.
using urbg32_ref = basic_urbg_ref<std::uint32_t>;

/// @brief `basic_urbg_ref` with `std::uint64_t` output.
using urbg64_ref = basic_urbg_ref<std::uint64_t>;

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(std::uniform_random_bit_generator<muc::urbg32_ref>);
static_assert(std::uniform_random_bit_generator<muc::urbg64_ref>);
static_assert(std::same_as<muc::urbg32_ref::result_type, std::uint32_t>);
static_assert(std::same_as<muc::urbg64_ref::result_type, std::uint64_t>);
static_assert(muc::urbg32_ref::min() == 0);
static_assert(muc::urbg32_ref::max() ==
              std::numeric_limits<std::uint32_t>::max());
static_assert(muc::urbg64_ref::min() == 0);
static_assert(muc::urbg64_ref::max() ==
              std::numeric_limits<std::uint64_t>::max());
static_assert(not std::default_initializable<muc::urbg32_ref>);
static_assert(not std::default_initializable<muc::urbg64_ref>);
static_assert(std::copyable<muc::urbg32_ref>);
static_assert(std::copyable<muc::urbg64_ref>);
static_assert(std::is_trivially_copyable_v<muc::urbg64_ref>);
static_assert(std::is_trivially_destructible_v<muc::urbg64_ref>);
static_assert(std::constructible_from<muc::urbg32_ref, std::mt19937&>);
static_assert(std::constructible_from<muc::urbg64_ref, std::mt19937&>);
static_assert(std::constructible_from<muc::urbg64_ref, std::mt19937_64&>);
static_assert(std::constructible_from<muc::urbg32_ref, std::minstd_rand&>);
static_assert(std::constructible_from<muc::urbg64_ref, std::ranlux48&>);
static_assert(std::constructible_from<muc::urbg64_ref, std::random_device&>);
static_assert(not std::constructible_from<muc::urbg64_ref, std::mt19937>);
static_assert(
    not std::constructible_from<muc::urbg64_ref, const std::mt19937&>);
static_assert(not std::constructible_from<muc::urbg64_ref, int&>);

#endif
