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
#include "muc/detail/c++20/random/random_number_generator.h++"
#include "muc/detail/c++20/random/urbg_ref.h++"

#include <concepts>
#include <cstdint>
#include <istream>
#include <ostream>
#include <type_traits>

namespace muc {

namespace impl {

/// @brief Type-erased dispatch table of basic_rng_ref.
template<typename UInt>
struct rng_ref_ops {
    urbg_ref_ops<UInt> m_gen;
    void (*m_seed_default)(void*);
    void (*m_seed_value)(void*, UInt);
    void (*m_discard)(void*, unsigned long long);
    bool (*m_equal)(const void*, const void*);
    auto (*m_output_narrow)(const void*, std::ostream&) -> std::ostream&;
    auto (*m_output_wide)(const void*, std::wostream&) -> std::wostream&;
    auto (*m_input_narrow)(void*, std::istream&) -> std::istream&;
    auto (*m_input_wide)(void*, std::wistream&) -> std::wistream&;
};

/// @brief Binds rng_ref_ops to the concrete engine type G.
template<typename G, typename UInt>
struct rng_ref_binding {
    static void seed_default(void* p) {
        static_cast<G*>(p)->seed();
    }

    static void seed_value(void* p, UInt s) {
        static_cast<G*>(p)->seed(static_cast<typename G::result_type>(s));
    }

    static void discard(void* p, unsigned long long z) {
        static_cast<G*>(p)->discard(z);
    }

    static bool equal(const void* a, const void* b) {
        return *static_cast<const G*>(a) == *static_cast<const G*>(b);
    }

    static auto output_narrow(const void* p, std::ostream& os)
        -> std::ostream& {
        return os << *static_cast<const G*>(p);
    }

    static auto output_wide(const void* p, std::wostream& os)
        -> std::wostream& {
        return os << *static_cast<const G*>(p);
    }

    static auto input_narrow(void* p, std::istream& is) -> std::istream& {
        return is >> *static_cast<G*>(p);
    }

    static auto input_wide(void* p, std::wistream& is) -> std::wistream& {
        return is >> *static_cast<G*>(p);
    }

    static constexpr rng_ref_ops<UInt> ops{
        urbg_ref_binding<G, UInt>::ops,
        &seed_default,
        &seed_value,
        &discard,
        &equal,
        &output_narrow,
        &output_wide,
        &input_narrow,
        &input_wide,
    };
};

} // namespace impl

/// @brief Type-erased non-owning reference to a random number engine.
///
/// Binds a reference to any object whose type satisfies
/// `random_number_generator` and erases that type; every type satisfying
/// `random_number_engine` qualifies. Copies of a basic_rng_ref share the
/// referenced engine. The wrapper is never empty: it is not default
/// constructible and always holds a reference. Besides generating values like
/// `basic_urbg_ref`, it forwards the engine operations `seed`, `discard`,
/// equality and stream serialization to the referenced engine.
///
/// @tparam UInt unsigned output type, aka `result_type`
///
/// @par Generation
/// The wrapper itself satisfies `std::uniform_random_bit_generator`, see
/// `basic_urbg_ref` for the generation semantics. It satisfies
/// `random_number_generator`, since it forwards every engine operation
/// that concept requires, but not `random_number_engine`: as a reference
/// it can neither be default constructed nor be constructed from a seed.
/// The owning `basic_any_rng` can do both and therefore satisfies
/// `random_number_engine`.
///
/// @par Engine operations
/// `seed()` and `seed(s)` reseed the referenced engine, truncating s to
/// `G::result_type` by unsigned conversion. `discard(z)` advances the
/// referenced engine by z of its own outputs, not z wrapper calls (one
/// wrapper call may consume several engine outputs). Two wrappers compare
/// equal when they reference engines of the same type with equal state;
/// wrappers over different engine types never compare equal. The stream
/// operators serialize the state of the referenced engine for both narrow
/// and wide streams.
///
/// @warning The referenced engine must outlive the wrapper and every copy
/// of it.
/// @see muc::random_number_engine, muc::random_number_generator,
/// muc::basic_any_rng, muc::basic_urbg_ref
template<std::unsigned_integral UInt = std::uint64_t>
class basic_rng_ref : impl::random_ref_tag {
public:
    /// @brief The type of the generated random values.
    using result_type = UInt;

    /// @brief Binds the wrapper to the generator g.
    /// @tparam G type of the referenced generator
    /// @param g generator to reference; must outlive the wrapper
    template<typename G>
        requires(not std::is_base_of_v<impl::random_ref_tag,
                                       std::remove_cvref_t<G>>) and
                    random_number_generator<G>
    constexpr basic_rng_ref(G& g) noexcept :
        m_ptr{&g},
        m_ops{&impl::rng_ref_binding<G, UInt>::ops} {}

    /// @brief Generates a random value.
    /// @return a value uniformly distributed over `[min(), max()]`
    MUC_ALWAYS_INLINE auto operator()() const -> result_type {
        return m_ops->m_gen.m_call(m_ptr);
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

    /// @brief Reseeds the referenced engine with its default seed.
    auto seed() const -> void {
        m_ops->m_seed_default(m_ptr);
    }

    /// @brief Reseeds the referenced engine with the value s.
    /// @param s seed value; truncated to `G::result_type` by unsigned
    /// conversion
    auto seed(result_type s) const -> void {
        m_ops->m_seed_value(m_ptr, s);
    }

    /// @brief Advances the referenced engine by z of its outputs.
    /// @param z number of engine outputs to skip
    auto discard(unsigned long long z) const -> void {
        m_ops->m_discard(m_ptr, z);
    }

    /// @brief Recovers a pointer to the referenced engine.
    /// @tparam E type to look up
    /// @return a pointer to the referenced object if it has type E,
    /// otherwise nullptr
    template<typename E>
    [[nodiscard]] auto target() const noexcept -> E* {
        return m_ops->m_gen.m_token() ==
                       &impl::type_token_v<std::remove_cvref_t<E>> ?
                   static_cast<E*>(m_ptr) :
                   nullptr;
    }

    /// @brief Shares the referenced generator as a `basic_urbg_ref`.
    /// @return a `basic_urbg_ref` bound to the same referenced engine
    operator basic_urbg_ref<UInt>() const noexcept {
        return basic_urbg_ref<UInt>{m_ptr, &m_ops->m_gen};
    }

    /// @brief Compares the state of the referenced engines.
    /// @param x first wrapper
    /// @param y second wrapper
    /// @return true if both reference engines of the same type with equal
    /// state
    friend auto operator==(const basic_rng_ref& x, const basic_rng_ref& y)
        -> bool {
        return x.m_ops->m_gen.m_token() == y.m_ops->m_gen.m_token() and
               x.m_ops->m_equal(x.m_ptr, y.m_ptr);
    }

    /// @brief Compares the state of the referenced engines.
    /// @param x first wrapper
    /// @param y second wrapper
    /// @return true if x and y do not compare equal
    friend auto operator!=(const basic_rng_ref& x, const basic_rng_ref& y)
        -> bool {
        return not(x == y);
    }

    /// @brief Writes the state of the referenced engine to a narrow stream.
    /// @param os output stream
    /// @param x wrapper to serialize
    /// @return os
    friend auto operator<<(std::ostream& os, const basic_rng_ref& x)
        -> std::ostream& {
        return x.m_ops->m_output_narrow(x.m_ptr, os);
    }

    /// @brief Writes the state of the referenced engine to a wide stream.
    /// @param os output stream
    /// @param x wrapper to serialize
    /// @return os
    friend auto operator<<(std::wostream& os, const basic_rng_ref& x)
        -> std::wostream& {
        return x.m_ops->m_output_wide(x.m_ptr, os);
    }

    /// @brief Restores the state of the referenced engine from a narrow
    /// stream.
    /// @param is input stream
    /// @param x wrapper to restore
    /// @return is
    friend auto operator>>(std::istream& is, basic_rng_ref& x)
        -> std::istream& {
        return x.m_ops->m_input_narrow(x.m_ptr, is);
    }

    /// @brief Restores the state of the referenced engine from a wide
    /// stream.
    /// @param is input stream
    /// @param x wrapper to restore
    /// @return is
    friend auto operator>>(std::wistream& is, basic_rng_ref& x)
        -> std::wistream& {
        return x.m_ops->m_input_wide(x.m_ptr, is);
    }

private:
    template<std::unsigned_integral, random_number_engine>
    friend class basic_any_rng;

    constexpr basic_rng_ref(void* ptr,
                            const impl::rng_ref_ops<UInt>* ops) noexcept :
        m_ptr{ptr},
        m_ops{ops} {}

    void* m_ptr;
    const impl::rng_ref_ops<UInt>* m_ops;
};

/// @brief `basic_rng_ref` with `std::uint32_t` output.
using rng32_ref = basic_rng_ref<std::uint32_t>;

/// @brief `basic_rng_ref` with `std::uint64_t` output.
using rng_ref = basic_rng_ref<std::uint64_t>;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include "muc/detail/c++20/concepts/stream_ioable.h++"

#include <random>

static_assert(muc::random_number_generator<muc::rng32_ref>);
static_assert(muc::random_number_generator<muc::rng_ref>);
static_assert(not muc::random_number_engine<muc::rng32_ref>);
static_assert(not muc::random_number_engine<muc::rng_ref>);
static_assert(std::uniform_random_bit_generator<muc::rng32_ref>);
static_assert(std::uniform_random_bit_generator<muc::rng_ref>);
static_assert(std::same_as<muc::rng32_ref::result_type, std::uint32_t>);
static_assert(std::same_as<muc::rng_ref::result_type, std::uint64_t>);
static_assert(muc::rng32_ref::min() == 0);
static_assert(muc::rng32_ref::max() ==
              std::numeric_limits<std::uint32_t>::max());
static_assert(muc::rng_ref::min() == 0);
static_assert(muc::rng_ref::max() == std::numeric_limits<std::uint64_t>::max());
static_assert(not std::default_initializable<muc::rng32_ref>);
static_assert(not std::default_initializable<muc::rng_ref>);
static_assert(std::copyable<muc::rng32_ref>);
static_assert(std::copyable<muc::rng_ref>);
static_assert(std::is_trivially_copyable_v<muc::rng_ref>);
static_assert(std::is_trivially_destructible_v<muc::rng_ref>);
static_assert(std::equality_comparable<muc::rng_ref>);
static_assert(muc::stream_ioable<muc::rng_ref>);
static_assert(std::constructible_from<muc::rng32_ref, std::mt19937&>);
static_assert(std::constructible_from<muc::rng_ref, std::mt19937&>);
static_assert(std::constructible_from<muc::rng_ref, std::mt19937_64&>);
static_assert(std::constructible_from<muc::rng_ref, std::minstd_rand&>);
static_assert(std::constructible_from<muc::rng_ref, std::ranlux24&>);
static_assert(not std::constructible_from<muc::rng_ref, std::mt19937>);
static_assert(not std::constructible_from<muc::rng_ref, const std::mt19937&>);
static_assert(std::convertible_to<muc::rng32_ref, muc::urbg32_ref>);
static_assert(std::convertible_to<muc::rng_ref, muc::urbg_ref>);
static_assert(not std::convertible_to<muc::rng_ref, muc::urbg32_ref>);
static_assert(std::constructible_from<muc::urbg32_ref, muc::rng32_ref&>);
static_assert(std::constructible_from<muc::urbg_ref, muc::rng_ref&>);
static_assert(not std::constructible_from<muc::urbg_ref, muc::rng32_ref&>);

#endif
