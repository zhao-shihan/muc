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

#include "muc/detail/c++20/random/random_number_generator.h++"

#include <concepts>

namespace muc {

/// @brief C++ named requirements: RandomNumberEngine.
///
/// A random_number_engine is a `muc::random_number_generator` that adds the
/// construction requirements of the named requirement: an engine can be
/// created from scratch and from a seed value, while a random number
/// generator need not be, as a type-erased reference such as
/// `muc::basic_rng_ref` cannot. Owning standard engines satisfy this concept,
/// and everything satisfying this concept satisfies
/// `muc::random_number_generator` as well.
///
/// @note Satisfying this concept requires value semantics, not just the member
/// operations. The named requirement states that `E(x)` creates an engine that
/// compares equal to x and whose ith consecutive call returns the same value
/// as x's ith call, and that `E()` and `E(s)` create engines with an initial
/// state of their own; this holds only when an engine owns its state, so that
/// equal copies evolve independently. A non-owning reference such as
/// `muc::basic_rng_ref` therefore deliberately does not satisfy this concept
/// even though it provides every operation: copies of it share the referenced
/// engine, so equal copies would not generate the same sequence. Interfaces
/// that must also accept such references are constrained on
/// `muc::random_number_generator` instead; erasing the type of an engine while
/// keeping value semantics requires a wrapper that owns the engine state.
///
/// See also:
/// https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
template<typename E>
concept random_number_engine =
    random_number_generator<E> and
    // 4. They said: "E() must be valid and creates an engine with the same
    // initial state as all other default-constructed engines of type E."
    std::default_initializable<E> and
    // 5. They said: "E(x) creates an engine that compares equal to x."
    // -- Satisfied by the CopyConstructible requirement of
    // random_number_generator.
    // 6. They said: "E(s) creates an engine whose initial state is determined
    // by s."
    std::constructible_from<E, typename E::result_type>;

} // namespace muc

#ifdef MUC_STATIC_TEST

#include <random>

static_assert(muc::random_number_engine<std::minstd_rand0>);
static_assert(muc::random_number_engine<std::minstd_rand>);
static_assert(muc::random_number_engine<std::mt19937>);
static_assert(muc::random_number_engine<std::mt19937_64>);
static_assert(muc::random_number_engine<std::ranlux24_base>);
static_assert(muc::random_number_engine<std::ranlux48_base>);
static_assert(muc::random_number_engine<std::ranlux24>);
static_assert(muc::random_number_engine<std::ranlux48>);
static_assert(muc::random_number_engine<std::knuth_b>);

#endif
