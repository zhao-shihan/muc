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

#include "muc/detail/c++20/concepts/stream_ioable.h++"

#include <concepts>
#include <random>

namespace muc {

/// @brief C++ named requirements: RandomNumberEngine, except that E need not
/// be default constructible nor constructible from a seed value.
///
/// A random_number_generator is a UniformRandomBitGenerator that can also be
/// reseeded, skipped ahead, compared by state and serialized, which is
/// everything a random number engine does apart from creating engines: a type
/// satisfies this concept even if it cannot be created from scratch or from a
/// seed value, as a type-erased reference such as `basic_rng_ref` cannot.
/// Every type satisfying `random_number_engine` satisfies this concept.
///
/// The named requirement states the postconditions `e == E()` for `seed()` and
/// `e == E(s)` for `seed(s)`, naming constructors this concept does not
/// require. Restated without them, seeding is reproducible: after `seed()`, or
/// after `seed(s)` with the same s, any two objects of type E compare equal,
/// as long as neither generated a value or was advanced by `discard` in
/// between. For an engine this is exactly the standard postcondition, because
/// `E()` and `E(s)` are the objects that seeding produces.
///
/// See also:
/// https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
template<typename E>
concept random_number_generator = requires(E e, const E x) {
    // 1. They said: "A random number engine is a function object returning
    // unsigned integer values such that each value in the range of possible
    // results has (ideally) equal probability. Any random number engine is
    // also a UniformRandomBitGenerator."
    requires std::uniform_random_bit_generator<E>;
    // 2. They said: "E satisfies CopyConstructible."
    // 3. They said: "E satisfies CopyAssignable."
    requires std::copyable<E>;
    // 4. They said: "E() must be valid and creates an engine with the same
    // initial state as all other default-constructed engines of type E."
    // 5. They said: "E(x) creates an engine that compares equal to x."
    // -- Satisfied by CopyConstructible.
    // 6. They said: "E(s) creates an engine whose initial state is determined
    // by s."
    // -- Requirements 4 to 6 are construction requirements and are not
    // imposed here, see random_number_engine.
    // 7. They said: "e.seed() must be valid and its value type is void.
    // Postcondition: e == E()." Since E() is not required to exist, the
    // postcondition is restated in the documentation above.
    { e.seed() } -> std::same_as<void>;
    // 8. They said: "e.seed(s) must be valid and its value type is void.
    // Postcondition: e == E(s)." Since E(s) is not required to exist, the
    // postcondition is restated in the documentation above.
    requires requires(typename E::result_type s) {
        { e.seed(s) } -> std::same_as<void>;
    };
    // 9. They said: "e.discard(z) must be valid and its value type is void.
    // Expression e.discard(z) advances e's state from e_i to e_{i+z} by any
    // means equivalent to z consecutive calls of e()."
    requires requires(unsigned long long z) {
        { e.discard(z) } -> std::same_as<void>;
    };
    // 10. They said: "Given x and y, (possibly const) values of type E, given
    // g1 and g2, lvalues of a type satisfying UniformRandomBitGenerator,
    // expression x == y must be valid. It returns true if for all positive
    // integer i, the ith consecutive calls of x(g1) and y(g2) return the
    // same value." Furthermore, they said: "Expression x != y must be
    // valid. It returns a bool such that (x == y) == !(x != y)."
    requires std::equality_comparable<E>;
    // 11. They said: "Given x, a (possibly const) value of E, given os, a
    // lvalue of a specialization of std::basic_ostream, expression os << x
    // must be valid. Expression os << x returns a reference to the type of
    // os. Expression os << x writes a textual representation of the engine's
    // current state to os. The formatting flags and fill character of os are
    // unchanged."
    // 12. They said: "Given v, a value of type E, given is, a lvalue of a
    // specialization of std::basic_istream, expression is >> v must be valid.
    // Expression is >> v returns a reference to the type of is. Expression
    // is >> v restores the engine's state with data read from is. The
    // formatting flags of is are unchanged. The data must have been written
    // using a stream with the same locale, CharT and Traits stream template
    // parameters, otherwise the behavior is undefined. If bad input is
    // encountered, is.setstate(std::ios::failbit) is called, which may throw
    // std::ios_base::failure. v is unchanged in that case."
    requires stream_ioable<E>;
};

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::random_number_generator<std::minstd_rand0>);
static_assert(muc::random_number_generator<std::minstd_rand>);
static_assert(muc::random_number_generator<std::mt19937>);
static_assert(muc::random_number_generator<std::mt19937_64>);
static_assert(muc::random_number_generator<std::ranlux24_base>);
static_assert(muc::random_number_generator<std::ranlux48_base>);
static_assert(muc::random_number_generator<std::ranlux24>);
static_assert(muc::random_number_generator<std::ranlux48>);
static_assert(muc::random_number_generator<std::knuth_b>);
static_assert(not muc::random_number_generator<std::random_device>);

#endif
