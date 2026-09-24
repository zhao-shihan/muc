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

#include "muc/detail/c++20/random/urbg_ref.h++"

#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

namespace muc {

namespace impl {

/// @brief Type-erased dispatch and life-cycle table of basic_any_urbg.
///
/// The dispatch half is the table of `basic_urbg_ref`, shared with it
/// unchanged, so a view produced by `ref()` and a wrapper built from the
/// same generator type dispatch identically. The life-cycle half owns
/// and copies the erased generator.
template<typename UInt>
struct any_urbg_ops {
    urbg_ref_ops<UInt> m_ref;
    void* (*m_clone)(const void*);
    void (*m_destroy)(void*);
};

/// @brief Binds any_urbg_ops to the concrete generator type G.
template<typename G, typename UInt>
struct any_urbg_binding {
    static auto clone(const void* p) -> void* {
        return new G(*static_cast<const G*>(p));
    }

    static void destroy(void* p) {
        delete static_cast<G*>(p);
    }

    static constexpr any_urbg_ops<UInt> ops{
        urbg_ref_binding<G, UInt>::ops,
        &clone,
        &destroy,
    };
};

} // namespace impl

/// @brief Type-erased owning value wrapper of a UniformRandomBitGenerator.
///
/// Binds a copy of any object whose type satisfies
/// `std::uniform_random_bit_generator` and erases that type. Unlike
/// `basic_urbg_ref`, whose copies share one generator, a basic_any_urbg
/// owns its generator: copying duplicates the generator state, so equal
/// copies evolve independently. The wrapper is never empty: it is not
/// default constructible and always owns a generator.
///
/// @tparam UInt unsigned output type, aka `result_type`
///
/// @par Generation
/// The wrapper itself satisfies `std::uniform_random_bit_generator` with
/// the same semantics as `basic_urbg_ref`: `result_type` is UInt, `min()`
/// is 0, `max()` is the maximum of UInt, and outputs of the owned
/// generator are rebased to zero-based segments and concatenated into
/// UInt, first drawn segment most significant. A wrapper and a
/// `basic_urbg_ref` over generators of the same type and state generate
/// the same sequence.
///
/// @note Copying and moving both duplicate the generator state, like the
/// standard engines whose move is a memberwise copy; the source of a move
/// is left unchanged. Consequently a move is not O(1).
/// @warning A `basic_urbg_ref` obtained from `ref()` shares the owned
/// generator and must not outlive the wrapper or any copy of it.
/// @see std::uniform_random_bit_generator, muc::basic_any_rng,
/// muc::basic_urbg_ref
template<std::unsigned_integral UInt = std::uint64_t>
class basic_any_urbg {
public:
    /// @brief The type of the generated random values.
    using result_type = UInt;

    /// @brief Takes ownership of a copy of the generator g.
    /// @tparam G type of the generator to own a copy of
    /// @param g generator to copy; an rvalue is moved from
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                    (not std::is_same_v<std::remove_cvref_t<G>,
                                        basic_any_urbg>) and
                    std::uniform_random_bit_generator<
                        std::remove_cvref_t<G>> and
                    std::copy_constructible<std::remove_cvref_t<G>>
    basic_any_urbg(G&& g) :
        m_ptr{new std::remove_cvref_t<G>(std::forward<G>(g))},
        m_ops{&impl::any_urbg_binding<std::remove_cvref_t<G>, UInt>::ops} {}

    /// @brief Duplicates the generator owned by other.
    /// @param other wrapper to copy
    basic_any_urbg(const basic_any_urbg& other) :
        m_ptr{other.m_ops->m_clone(other.m_ptr)},
        m_ops{other.m_ops} {}

    /// @brief Duplicates the generator owned by other.
    /// @param other wrapper to copy; unchanged by the move
    basic_any_urbg(basic_any_urbg&& other) :
        basic_any_urbg(static_cast<const basic_any_urbg&>(other)) {}

    /// @brief Destroys the owned generator.
    ~basic_any_urbg() {
        m_ops->m_destroy(m_ptr);
    }

    /// @brief Duplicates the generator owned by other.
    /// @param other wrapper to copy
    /// @return *this
    auto operator=(const basic_any_urbg& other) -> basic_any_urbg& {
        auto* const p{other.m_ops->m_clone(other.m_ptr)};
        m_ops->m_destroy(m_ptr);
        m_ptr = p;
        m_ops = other.m_ops;
        return *this;
    }

    /// @brief Duplicates the generator owned by other.
    /// @param other wrapper to copy; unchanged by the move
    /// @return *this
    auto operator=(basic_any_urbg&& other) -> basic_any_urbg& {
        return *this = static_cast<const basic_any_urbg&>(other);
    }

    /// @brief Replaces the owned generator with a copy of g.
    /// @tparam G type of the generator to own a copy of
    /// @param g generator to copy; an rvalue is moved from
    /// @return *this
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                (not std::is_same_v<std::remove_cvref_t<G>, basic_any_urbg>) and
                std::uniform_random_bit_generator<std::remove_cvref_t<G>> and
                std::copy_constructible<std::remove_cvref_t<G>>
    auto operator=(G&& g) -> basic_any_urbg& {
        auto tmp{basic_any_urbg{std::forward<G>(g)}};
        std::swap(m_ptr, tmp.m_ptr);
        std::swap(m_ops, tmp.m_ops);
        return *this;
    }

    /// @brief Generates a random value.
    /// @return a value uniformly distributed over `[min(), max()]`
    MUC_ALWAYS_INLINE auto operator()() -> result_type {
        return m_ops->m_ref.m_call(m_ptr);
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

    /// @brief Recovers a pointer to the owned generator.
    /// @tparam E type to look up
    /// @return a pointer to the owned generator if it has type E,
    /// otherwise nullptr
    template<typename E>
    [[nodiscard]] auto target() noexcept -> E* {
        return m_ops->m_ref.m_token() ==
                       &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<E*>(m_ptr) :
                   nullptr;
    }

    /// @brief Recovers a pointer to the owned generator.
    /// @tparam E type to look up
    /// @return a pointer to the owned generator if it has type E,
    /// otherwise nullptr
    template<typename E>
    [[nodiscard]] auto target() const noexcept -> const E* {
        return m_ops->m_ref.m_token() ==
                       &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<const E*>(m_ptr) :
                   nullptr;
    }

    /// @brief Shares the owned generator as a `basic_urbg_ref`.
    /// @return a `basic_urbg_ref` bound to the owned generator
    [[nodiscard]] auto ref() & noexcept -> basic_urbg_ref<UInt> {
        return basic_urbg_ref<UInt>{m_ptr, &m_ops->m_ref};
    }

    /// @brief Exchanges the owned generators.
    /// @param x first wrapper
    /// @param y second wrapper
    friend void swap(basic_any_urbg& x, basic_any_urbg& y) noexcept {
        std::swap(x.m_ptr, y.m_ptr);
        std::swap(x.m_ops, y.m_ops);
    }

private:
    void* m_ptr;
    const impl::any_urbg_ops<UInt>* m_ops;
};

/// @brief `basic_any_urbg` with `std::uint32_t` output.
using any_urbg32 = basic_any_urbg<std::uint32_t>;

/// @brief `basic_any_urbg` with `std::uint64_t` output.
using any_urbg = basic_any_urbg<std::uint64_t>;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include "muc/detail/c++20/random/rng_ref.h++"

#include <random>
#include <utility>

static_assert(std::uniform_random_bit_generator<muc::any_urbg32>);
static_assert(std::uniform_random_bit_generator<muc::any_urbg>);
static_assert(std::same_as<muc::any_urbg32::result_type, std::uint32_t>);
static_assert(std::same_as<muc::any_urbg::result_type, std::uint64_t>);
static_assert(muc::any_urbg32::min() == 0);
static_assert(muc::any_urbg::max() ==
              std::numeric_limits<std::uint64_t>::max());
static_assert(not std::default_initializable<muc::any_urbg32>);
static_assert(not std::default_initializable<muc::any_urbg>);
static_assert(std::copyable<muc::any_urbg32>);
static_assert(std::copyable<muc::any_urbg>);
static_assert(not std::is_trivially_copyable_v<muc::any_urbg>);
static_assert(not std::is_trivially_destructible_v<muc::any_urbg>);
static_assert(std::constructible_from<muc::any_urbg32, std::mt19937&>);
static_assert(std::constructible_from<muc::any_urbg32, std::mt19937>);
static_assert(std::constructible_from<muc::any_urbg32, const std::mt19937&>);
static_assert(std::constructible_from<muc::any_urbg, std::mt19937_64&>);
static_assert(std::constructible_from<muc::any_urbg, std::minstd_rand&>);
static_assert(std::constructible_from<muc::any_urbg, std::ranlux48&>);
static_assert(std::is_convertible_v<std::mt19937, muc::any_urbg>);
static_assert(not std::constructible_from<muc::any_urbg, int&>);
static_assert(not std::constructible_from<muc::any_urbg, std::random_device&>);
static_assert(not std::constructible_from<muc::any_urbg, muc::urbg32_ref&>);
static_assert(not std::constructible_from<muc::any_urbg, muc::rng64_ref&>);
static_assert(std::same_as<decltype(std::declval<muc::any_urbg&>().ref()),
                           muc::urbg64_ref>);
static_assert(std::same_as<decltype(std::declval<muc::any_urbg32&>().ref()),
                           muc::urbg32_ref>);
static_assert(
    not std::is_invocable_v<decltype(&muc::any_urbg::ref), muc::any_urbg&&>);
static_assert(
    std::same_as<decltype(std::declval<const muc::any_urbg&>().target<int>()),
                 const int*>);

#endif
