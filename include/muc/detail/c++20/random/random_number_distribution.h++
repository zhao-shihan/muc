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

/// @brief Sub-requirements on D::param_type of RandomNumberDistribution (a C++
/// named requirements).
/// See also:
/// https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution
template<typename P>
concept random_number_distribution_parameter = requires {
    // 1. They said: "D::param_type (aka P) satisfies CopyConstructible."
    // 2. They said: "D::param_type (aka P) satisfies CopyAssignable."
    requires std::copyable<P>;
    // 3. They said: "D::param_type (aka P) satisfies EqualityComparable."
    requires std::equality_comparable<P>;
    // 4. They said: "D::param_type (aka P) has a constructor taking identical
    // arguments as each of the constructors of D that take arguments
    // corresponding to the distribution parameters."
    // -- Semantic only
    // 5. They said: "D::param_type (aka P) has a member function with the
    // identical name, type, and semantics, as every member function of D that
    // returns a parameter of the distribution."
    // -- Semantic only
    // 6. They said: "D::param_type (aka P) declares a member typedef:
    // using distribution_type = D;"
    typename P::distribution_type;
};

/// @brief Sub-requirements on D::param_type of RandomNumberDistribution (a C++
/// named requirements).
/// See also:
/// https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution
template<typename P, typename D>
concept random_number_distribution_parameter_of = requires {
    // 1. Previous requirements.
    requires random_number_distribution_parameter<P>;
    // 2. The member typedef distribution_type is same as D.
    requires std::same_as<typename P::distribution_type, D>;
    // 3. D satisfies RandomNumberDistribution, where they said: "D::param_type
    // must be valid.". It's clear that D::param_type should be the same as P.
    typename D::param_type;
    requires std::same_as<P, typename D::param_type>;
};

/// @brief C++ named requirements: RandomNumberDistribution.
/// See also:
/// https://en.cppreference.com/w/cpp/named_req/RandomNumberDistribution
template<typename D>
concept random_number_distribution = requires(D d, const D x) {
    // 1. They said: "D satisfies CopyConstructible."
    // 2. They said: "D satisfies CopyAssignable."
    requires std::copyable<D>;
    // 3. They said: "D::result_type must be valid, it is an arithmetic type."
    // -- Weakened, here we allow D::result_type to be something else
    typename D::result_type;
    // 4. They said: "D::param_type must be valid, it satisfies 'some
    // requirements'."
    typename D::param_type;
    requires random_number_distribution_parameter_of<typename D::param_type, D>;
    // 5. They said: "D() must be valid and creates a distribution
    // indistinguishable from any other default-constructed D."
    requires std::default_initializable<D>;
    // 6. They said: "Given p, a (possibly const) value of type D::param_type,
    // D(p) must be valid and creates a distribution indistinguishable from D
    // constructed directly from the values used to construct p."
    requires std::constructible_from<D, const typename D::param_type>;
    // 7. They said: "Given d, a value of D, expression d.reset() must be
    // valid and its value type is void. Expression d.reset() resets the
    // internal state of the distribution. The next call to operator() on
    // d will not depend on values produced by any engine prior to reset()."
    { d.reset() } -> std::same_as<void>;
    // 8. They said: "Given x, a (possibly const) value of D, expression
    // x.param() must be valid and its value type is D::param_type. Expression
    // x.param() returns p such that D(p).param() == p.
    { x.param() } -> std::same_as<typename D::param_type>;
    // 9. They said: "Given d, a value of D, and given p, a (possibly const)
    // value of type D::param_type, expression d.param(p) must be
    // valid and its value type is void. The postcondition of d.param(p) is
    // d.param() == p.
    requires requires(const typename D::param_type p) {
        { d.param(p) } -> std::same_as<void>;
    };
    // 10. They said: "Given g, lvalues of a type satisfying
    // UniformRandomBitGenerator, expression d(g) must be valid. The result of
    // expression d(g) has type of D::result_type. The sequence of numbers
    // returned by successive invocations of expression d(g) with the same g
    // are randomly distributed according to the distribution parametrized by
    // 11. They said: "Given g, lvalues of a type satisfying
    // UniformRandomBitGenerator, and given p, a (possibly const) value of type
    // D::param_type, expression d(g, p) must be valid. The sequence of numbers
    // returned by successive invocations of d(g, p) with the same g are
    // randomly distributed according to the distribution parametrized by p.
    // d.param().
    requires requires(std::random_device g, const typename D::param_type p) {
        { d(g) } -> std::same_as<typename D::result_type>;
        { d(g, p) } -> std::same_as<typename D::result_type>;
    };
    // 12. They said: "Given x, a (possibly const) value of D, expression
    // x.min() must be valid. It returns the greatest lower bound on the values
    // potentially returned by x’s operator(), as determined by the current
    // values of x’s parameters.
    { x.min() } -> std::same_as<typename D::result_type>;
    // 13. They said: "Given x, a (possibly const) value of D, expression
    // x.max() must be valid. It returns the least upper bound on the values
    // potentially returned by x’s operator(), as determined by the current
    // values of x’s parameters.
    { x.max() } -> std::same_as<typename D::result_type>;
    // 14. They said: "Given x and y, (possibly const) values of type D, given
    // g1 and g2, lvalues of a type satisfying UniformRandomBitGenerator,
    // expression x == y must be valid. It Establishes an equivalence relation.
    // It returns true if x.param() == y.param() and future infinite sequences
    // of values that would be generated by repeated invocations of x(g1) and
    // y(g2) would be equal as long as g1 == g2. Furthermore, they said: "Given
    // x and y, (possibly const) values of type D, expression x != y must be
    // valid. It returns a bool such that (x == y) == !(x != y)."
    requires std::equality_comparable<D>;
    // 15. They said: "Given x, a (possibly const) value of D, given os, a
    // lvalue of a specialization of std::basic_ostream, expression os << x
    // must be valid. Expression os << x returns a reference to the type of
    // os. Expression os << x writes a textual representation of the
    // distribution parameters and internal state to os. The formatting flags
    // and fill character of os are unchanged."
    // 16. They said: "Given d, a value of type D, given is, a lvalue of a
    // specialization of std::basic_istream, expression is >> d must be valid.
    // Expression is >> d returns a reference to the type of is. Expression
    // is >> d restores the distribution parameters and internal state with
    // data read from is. The formatting flags of is are unchanged. The data
    // must have been written using a stream with the same locale, CharT and
    // Traits stream template parameters, otherwise the behavior is undefined.
    // If bad input is encountered, is.setstate(std::ios::failbit) is called,
    // which may throw std::ios_base::failure. d is unchanged in that case."
    requires muc::stream_ioable<D>;
};

} // namespace muc

#ifdef MUC_STATIC_TEST

static_assert(muc::random_number_distribution<std::uniform_int_distribution<int>>);
static_assert(muc::random_number_distribution<std::uniform_real_distribution<double>>);
static_assert(muc::random_number_distribution<std::bernoulli_distribution>);
static_assert(muc::random_number_distribution<std::binomial_distribution<int>>);
static_assert(muc::random_number_distribution<std::negative_binomial_distribution<int>>);
static_assert(muc::random_number_distribution<std::geometric_distribution<int>>);
static_assert(muc::random_number_distribution<std::poisson_distribution<int>>);
static_assert(muc::random_number_distribution<std::exponential_distribution<double>>);
static_assert(muc::random_number_distribution<std::gamma_distribution<double>>);
static_assert(muc::random_number_distribution<std::weibull_distribution<double>>);
static_assert(muc::random_number_distribution<std::extreme_value_distribution<double>>);
static_assert(muc::random_number_distribution<std::normal_distribution<double>>);
static_assert(muc::random_number_distribution<std::lognormal_distribution<double>>);
static_assert(muc::random_number_distribution<std::chi_squared_distribution<double>>);
static_assert(muc::random_number_distribution<std::cauchy_distribution<double>>);
static_assert(muc::random_number_distribution<std::fisher_f_distribution<double>>);
static_assert(muc::random_number_distribution<std::student_t_distribution<double>>);
static_assert(muc::random_number_distribution<std::discrete_distribution<int>>);
static_assert(muc::random_number_distribution<std::piecewise_constant_distribution<double>>);
static_assert(muc::random_number_distribution<std::piecewise_linear_distribution<double>>);

#endif
