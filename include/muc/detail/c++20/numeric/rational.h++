// -*- C++ -*-
//
// Copyright (C) 2021-2025  Shihan Zhao
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

#include "muc/detail/c++20/numeric/polynomial.h++"

#include <concepts>
#include <initializer_list>
#include <ranges>

namespace muc {

/// @brief Evaluates a rational function at a given point using polynomial
/// evaluation.
///
/// This function computes the value of a rational function defined by a
/// numerator and denominator, where both are represented as polynomial
/// coefficients. The function evaluates the numerator and denominator
/// polynomials at the specified floating-point value `x`, and returns the
/// result of the division.
///
/// @tparam T The type of the polynomial evaluation, constrained to
/// floating-point types.
/// @tparam A The type of the coefficients collection for the numerator, which
/// defaults to an
///            initializer_list. It must satisfy the requirements of a range.
/// @tparam B The type of the coefficients collection for the denominator, which
/// defaults to an
///            initializer_list. It must satisfy the requirements of a range.
/// @param numer A collection of coefficients representing the numerator
/// polynomial.
///              The coefficients should be specified in standard order,
///              starting from the constant term.
/// @param denom A collection of coefficients representing the denominator
/// polynomial.
///              The coefficients should also be specified in standard order,
///              starting from the constant term.
/// @param x The floating-point value at which the rational function is to be
/// evaluated.
/// @return The result of evaluating the rational function at the point `x`,
/// which is the
///         quotient of the evaluated numerator and denominator polynomials.
///         Note: The function does not check for division by zero.
template<std::floating_point T, std::ranges::range A = std::initializer_list<T>,
         std::ranges::range B = std::initializer_list<T>>
constexpr auto rational(A&& numer, B&& denom, T x) -> T {
    return polynomial(numer, x) / polynomial(denom, x);
}

/// @brief Evaluates a rational function at a given integral point using
/// polynomial evaluation.
///
/// This function serves as an overload for evaluating a rational function with
/// integral input. It takes a numerator and denominator represented as
/// polynomial coefficients and evaluates the rational function at the integral
/// point `x`. The integral `x` is implicitly converted to the specified
/// floating-point type `T` for evaluation.
///
/// @tparam T The type of the polynomial evaluation, constrained to
/// floating-point types.
///           The default type is double.
/// @tparam A The type of the coefficients collection for the numerator, which
/// defaults to an
///            initializer_list. It must satisfy the requirements of a range.
/// @tparam B The type of the coefficients collection for the denominator, which
/// defaults to an
///            initializer_list. It must satisfy the requirements of a range.
/// @param numer A collection of coefficients representing the numerator
/// polynomial.
///              The coefficients should be specified in standard order,
///              starting from the constant term.
/// @param denom A collection of coefficients representing the denominator
/// polynomial.
///              The coefficients should also be specified in standard order,
///              starting from the constant term.
/// @param x The integral value at which the rational function is to be
/// evaluated.
/// @return The result of evaluating the rational function at the point `x`,
/// which is the
///         quotient of the evaluated numerator and denominator polynomials.
///         Note: The function does not check for division by zero.
template<std::floating_point T = double,
         std::ranges::range A = std::initializer_list<T>,
         std::ranges::range B = std::initializer_list<T>>
constexpr auto rational(A&& numer, B&& denom, std::integral auto x) -> T {
    return rational<T>(numer, denom, x);
}

} // namespace muc
