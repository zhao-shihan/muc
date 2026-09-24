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

#include "muc/detail/c++20/random/random_number_engine.h++"
#include "muc/detail/c++20/random/rng_ref.h++"

#include <concepts>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <random>
#include <type_traits>
#include <utility>

namespace muc {

namespace impl {

/// @brief Type-erased dispatch and life-cycle table of basic_any_rng.
///
/// The dispatch half is the table of `basic_rng_ref`, shared with it
/// unchanged, so a view produced by `ref()` and a wrapper built from the
/// same generator type dispatch identically. The life-cycle half owns
/// and copies the erased generator.
template<typename UInt>
struct any_rng_ops {
    rng_ref_ops<UInt> m_ref;
    void* (*m_clone)(const void*);
    void (*m_destroy)(void*);
};

/// @brief Binds any_rng_ops to the concrete engine type G.
template<typename G, typename UInt>
struct any_rng_binding {
    static auto clone(const void* p) -> void* {
        return new G(*static_cast<const G*>(p));
    }

    static void destroy(void* p) {
        delete static_cast<G*>(p);
    }

    static constexpr any_rng_ops<UInt> ops{
        rng_ref_binding<G, UInt>::ops,
        &clone,
        &destroy,
    };
};

} // namespace impl

/// @brief Type-erased owning value wrapper of a random number engine.
///
/// Binds a copy of any object whose type satisfies
/// `random_number_generator` and erases that type; every type satisfying
/// `random_number_engine` qualifies. Unlike `basic_rng_ref`, whose copies
/// share one engine, a basic_any_rng owns its engine: copying duplicates
/// the engine state, so equal copies evolve independently. With the
/// default-constructed and seed-constructed states that D provides, the
/// wrapper itself satisfies `random_number_engine`. The wrapper is never
/// empty: it always owns an engine.
///
/// @tparam UInt unsigned output type, aka `result_type`
/// @tparam D engine behind the default-constructed and seed-constructed
/// states, satisfying `random_number_engine`
///
/// @par Generation
/// The wrapper itself satisfies `std::uniform_random_bit_generator`, see
/// `basic_urbg_ref` for the generation semantics.
///
/// @par Engine operations
/// `seed()` and `seed(s)` reseed the owned engine in place, keeping its
/// type and truncating s to `G::result_type` by unsigned conversion; they
/// therefore reproduce states of the owned engine, so two wrappers seeded
/// alike compare equal and generate the same sequence as long as they own
/// engines of the same type. Wrappers over different engine types never
/// compare equal, not even after `seed()`, which is where the
/// `random_number_engine` postconditions `e == E()` and `e == E(s)` read
/// most literally with E the default payload D. `discard(z)` advances the
/// owned engine by z of its own outputs, not z wrapper calls (one wrapper
/// call may consume several engine outputs). Two wrappers compare equal
/// when they own engines of the same type with equal state. The stream
/// operators serialize the state of the owned engine for both narrow and
/// wide streams. `basic_any_rng()` owns a default-constructed D and
/// `basic_any_rng(s)` owns a D seeded with s truncated to
/// `D::result_type`.
///
/// @note Copying and moving both duplicate the engine state, like the
/// standard engines whose move is a memberwise copy; the source of a move
/// is left unchanged. Consequently a move is not O(1).
/// @warning A `basic_rng_ref` obtained from `ref()` shares the owned
/// engine and must not outlive the wrapper or any copy of it.
/// @see muc::random_number_engine, muc::random_number_generator,
/// muc::basic_any_urbg, muc::basic_rng_ref
template<std::unsigned_integral UInt = std::uint64_t,
         random_number_engine D = std::minstd_rand>
class basic_any_rng {
public:
    /// @brief The type of the generated random values.
    using result_type = UInt;

    /// @brief Owns a default-constructed D.
    basic_any_rng() :
        m_ptr{new D{}},
        m_ops{&impl::any_rng_binding<D, UInt>::ops} {}

    /// @brief Owns a D seeded with the value s.
    /// @param s seed value; truncated to `D::result_type` by unsigned
    /// conversion
    explicit basic_any_rng(result_type s) :
        m_ptr{new D(static_cast<typename D::result_type>(s))},
        m_ops{&impl::any_rng_binding<D, UInt>::ops} {}

    /// @brief Takes ownership of a copy of the engine g.
    /// @tparam G type of the engine to own a copy of
    /// @param g engine to copy; an rvalue is moved from
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                    (not std::is_same_v<std::remove_cvref_t<G>,
                                        basic_any_rng>) and
                    random_number_generator<std::remove_cvref_t<G>>
    basic_any_rng(G&& g) :
        m_ptr{new std::remove_cvref_t<G>(std::forward<G>(g))},
        m_ops{&impl::any_rng_binding<std::remove_cvref_t<G>, UInt>::ops} {}

    /// @brief Duplicates the engine owned by other.
    /// @param other wrapper to copy
    basic_any_rng(const basic_any_rng& other) :
        m_ptr{other.m_ops->m_clone(other.m_ptr)},
        m_ops{other.m_ops} {}

    /// @brief Duplicates the engine owned by other.
    /// @param other wrapper to copy; unchanged by the move
    basic_any_rng(basic_any_rng&& other) :
        basic_any_rng(static_cast<const basic_any_rng&>(other)) {}

    /// @brief Destroys the owned engine.
    ~basic_any_rng() {
        m_ops->m_destroy(m_ptr);
    }

    /// @brief Duplicates the engine owned by other.
    /// @param other wrapper to copy
    /// @return *this
    auto operator=(const basic_any_rng& other) -> basic_any_rng& {
        auto* const p{other.m_ops->m_clone(other.m_ptr)};
        m_ops->m_destroy(m_ptr);
        m_ptr = p;
        m_ops = other.m_ops;
        return *this;
    }

    /// @brief Duplicates the engine owned by other.
    /// @param other wrapper to copy; unchanged by the move
    /// @return *this
    auto operator=(basic_any_rng&& other) -> basic_any_rng& {
        return *this = static_cast<const basic_any_rng&>(other);
    }

    /// @brief Replaces the owned engine with a copy of g.
    /// @tparam G type of the engine to own a copy of
    /// @param g engine to copy; an rvalue is moved from
    /// @return *this
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                (not std::is_same_v<std::remove_cvref_t<G>, basic_any_rng>) and
                random_number_generator<std::remove_cvref_t<G>>
    auto operator=(G&& g) -> basic_any_rng& {
        auto tmp{basic_any_rng{std::forward<G>(g)}};
        std::swap(m_ptr, tmp.m_ptr);
        std::swap(m_ops, tmp.m_ops);
        return *this;
    }

    /// @brief Generates a random value.
    /// @return a value uniformly distributed over `[min(), max()]`
    MUC_ALWAYS_INLINE auto operator()() -> result_type {
        return m_ops->m_ref.m_gen.m_call(m_ptr);
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

    /// @brief Reseeds the owned engine with its default seed, keeping its
    /// type.
    auto seed() -> void {
        m_ops->m_ref.m_seed_default(m_ptr);
    }

    /// @brief Reseeds the owned engine with the value s, keeping its type.
    /// @param s seed value; truncated to `G::result_type` by unsigned
    /// conversion
    auto seed(result_type s) -> void {
        m_ops->m_ref.m_seed_value(m_ptr, s);
    }

    /// @brief Advances the owned engine by z of its outputs.
    /// @param z number of engine outputs to skip
    auto discard(unsigned long long z) -> void {
        m_ops->m_ref.m_discard(m_ptr, z);
    }

    /// @brief Recovers a pointer to the owned engine.
    /// @tparam E type to look up
    /// @return a pointer to the owned engine if it has type E, otherwise
    /// nullptr
    template<typename E>
    [[nodiscard]] auto target() noexcept -> E* {
        return m_ops->m_ref.m_gen.m_token() ==
                       &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<E*>(m_ptr) :
                   nullptr;
    }

    /// @brief Recovers a pointer to the owned engine.
    /// @tparam E type to look up
    /// @return a pointer to the owned engine if it has type E, otherwise
    /// nullptr
    template<typename E>
    [[nodiscard]] auto target() const noexcept -> const E* {
        return m_ops->m_ref.m_gen.m_token() ==
                       &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<const E*>(m_ptr) :
                   nullptr;
    }

    /// @brief Shares the owned engine as a `basic_rng_ref`.
    /// @return a `basic_rng_ref` bound to the owned engine
    [[nodiscard]] auto ref() & noexcept -> basic_rng_ref<UInt> {
        return basic_rng_ref<UInt>{m_ptr, &m_ops->m_ref};
    }

    /// @brief Compares the state of the owned engines.
    /// @param x first wrapper
    /// @param y second wrapper
    /// @return true if both own engines of the same type with equal state
    friend auto operator==(const basic_any_rng& x, const basic_any_rng& y)
        -> bool {
        return x.m_ops->m_ref.m_gen.m_token() ==
                   y.m_ops->m_ref.m_gen.m_token() and
               x.m_ops->m_ref.m_equal(x.m_ptr, y.m_ptr);
    }

    /// @brief Compares the state of the owned engines.
    /// @param x first wrapper
    /// @param y second wrapper
    /// @return true if x and y do not compare equal
    friend auto operator!=(const basic_any_rng& x, const basic_any_rng& y)
        -> bool {
        return not(x == y);
    }

    /// @brief Writes the state of the owned engine to a narrow stream.
    /// @param os output stream
    /// @param x wrapper to serialize
    /// @return os
    friend auto operator<<(std::ostream& os, const basic_any_rng& x)
        -> std::ostream& {
        return x.m_ops->m_ref.m_output_narrow(x.m_ptr, os);
    }

    /// @brief Writes the state of the owned engine to a wide stream.
    /// @param os output stream
    /// @param x wrapper to serialize
    /// @return os
    friend auto operator<<(std::wostream& os, const basic_any_rng& x)
        -> std::wostream& {
        return x.m_ops->m_ref.m_output_wide(x.m_ptr, os);
    }

    /// @brief Restores the state of the owned engine from a narrow stream.
    /// @param is input stream
    /// @param x wrapper to restore
    /// @return is
    friend auto operator>>(std::istream& is, basic_any_rng& x)
        -> std::istream& {
        return x.m_ops->m_ref.m_input_narrow(x.m_ptr, is);
    }

    /// @brief Restores the state of the owned engine from a wide stream.
    /// @param is input stream
    /// @param x wrapper to restore
    /// @return is
    friend auto operator>>(std::wistream& is, basic_any_rng& x)
        -> std::wistream& {
        return x.m_ops->m_ref.m_input_wide(x.m_ptr, is);
    }

    /// @brief Exchanges the owned engines.
    /// @param x first wrapper
    /// @param y second wrapper
    friend void swap(basic_any_rng& x, basic_any_rng& y) noexcept {
        std::swap(x.m_ptr, y.m_ptr);
        std::swap(x.m_ops, y.m_ops);
    }

private:
    void* m_ptr;
    const impl::any_rng_ops<UInt>* m_ops;
};

/// @brief `basic_any_rng` with `std::uint32_t` output.
using any_rng32 = basic_any_rng<std::uint32_t>;

/// @brief `basic_any_rng` with `std::uint64_t` output.
using any_rng = basic_any_rng<std::uint64_t>;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include "muc/detail/c++20/random/any_urbg.h++"

#include <random>
#include <utility>

static_assert(muc::random_number_generator<muc::any_rng32>);
static_assert(muc::random_number_generator<muc::any_rng>);
static_assert(muc::random_number_engine<muc::any_rng32>);
static_assert(muc::random_number_engine<muc::any_rng>);
static_assert(std::uniform_random_bit_generator<muc::any_rng>);
static_assert(not muc::random_number_generator<muc::any_urbg>);
static_assert(std::same_as<muc::any_rng32::result_type, std::uint32_t>);
static_assert(std::same_as<muc::any_rng::result_type, std::uint64_t>);
static_assert(muc::any_rng32::min() == 0);
static_assert(muc::any_rng::max() == std::numeric_limits<std::uint64_t>::max());
static_assert(std::default_initializable<muc::any_rng>);
static_assert(std::constructible_from<muc::any_rng, muc::any_rng::result_type>);
static_assert(std::copyable<muc::any_rng>);
static_assert(not std::is_trivially_copyable_v<muc::any_rng>);
static_assert(not std::is_trivially_destructible_v<muc::any_rng>);
static_assert(std::equality_comparable<muc::any_rng>);
static_assert(muc::stream_ioable<muc::any_rng>);
static_assert(std::constructible_from<muc::any_rng32, std::mt19937&>);
static_assert(std::constructible_from<muc::any_rng32, std::mt19937>);
static_assert(std::constructible_from<muc::any_rng32, const std::mt19937&>);
static_assert(std::constructible_from<muc::any_rng, std::mt19937_64&>);
static_assert(std::constructible_from<muc::any_rng, std::minstd_rand&>);
static_assert(std::constructible_from<muc::any_rng, std::ranlux24&>);
static_assert(std::is_convertible_v<std::mt19937, muc::any_rng>);
static_assert(not std::is_convertible_v<std::uint64_t, muc::any_rng>);
static_assert(not std::is_convertible_v<int, muc::any_rng>);
static_assert(not std::constructible_from<muc::any_rng, std::random_device&>);
static_assert(not std::constructible_from<muc::any_rng, muc::rng64_ref&>);
static_assert(not std::constructible_from<muc::any_rng, muc::any_urbg&>);
static_assert(std::constructible_from<muc::any_urbg, muc::any_rng&>);
static_assert(std::constructible_from<muc::any_urbg32, muc::any_rng&>);
static_assert(std::same_as<decltype(std::declval<muc::any_rng&>().ref()),
                           muc::rng64_ref>);
static_assert(std::same_as<decltype(std::declval<muc::any_rng32&>().ref()),
                           muc::rng32_ref>);
static_assert(
    not std::is_invocable_v<decltype(&muc::any_rng::ref), muc::any_rng&&>);
static_assert(
    std::same_as<decltype(std::declval<const muc::any_rng&>().target<int>()),
                 const int*>);

#endif
