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

#include <concepts>
#include <initializer_list>
#include <limits>
#include <ranges>

namespace muc {

/// @brief Evaluates a polynomial at a given value using Qin Jiushao's method.
///
/// This function takes a collection of coefficients representing a polynomial,
/// and evaluates it at the specified point `x`. The coefficients should be
/// provided in standard order (i.e., starting from the constant term).
///
/// The polynomial is evaluated using Qin Jiushao's method for improved
/// numerical stability and efficiency. If the coefficients collection is empty,
/// the function returns NaN.
///
/// @tparam T The type of the polynomial evaluation, constrained to
/// floating-point types.
/// @tparam C The type of the coefficients collection, which defaults to an
/// initializer_list.
///            It must satisfy the requirements of a range.
/// @param coeff A collection of coefficients representing the polynomial. The
/// coefficients
///              should be specified in standard order, where the first element
///              is the coefficient of the constant term.
/// @param x The value at which the polynomial is to be evaluated.
/// @return The result of evaluating the polynomial at the point `x`.
///         If the coefficients are empty, returns NaN or 0 depending on the
///         type.
template<std::floating_point T, std::ranges::range C = std::initializer_list<T>>
constexpr auto polynomial(C&& coeff, T x) -> T {
    auto c{std::ranges::crbegin(coeff)};
    const auto end{std::ranges::crend(coeff)};
    if (c == end) {
        return std::numeric_limits<T>::quiet_NaN();
    }
    T p{*c++};
    while (c != end) {
        p = p * x + *c++;
    }
    return p;
}

/// @brief Evaluates a polynomial at a given integral value using Horner's
/// method.
///
/// This function acts as an overload for evaluating a polynomial at an integral
/// input value. It ensures that the coefficient collection is passed in its
/// appropriate type while converting the integral `x` to the specified
/// floating-point type for evaluation. The coefficients should be defined in
/// standard order, starting from the constant term.
///
/// @tparam T The type of the polynomial evaluation, constrained to
/// floating-point types.
///           The default type is double.
/// @tparam C The type of the coefficients collection, which defaults to an
/// initializer_list.
///            It must satisfy the requirements of a range.
/// @param coeff A collection of coefficients representing the polynomial.
///              The coefficients should be specified in standard order,
///              where the first element is the coefficient of the constant
///              term.
/// @param x The integral value at which the polynomial is to be evaluated.
/// @return The result of evaluating the polynomial at the point `x`.
///         The integral `x` is implicitly converted to type `T` for the
///         evaluation.
template<std::floating_point T = double,
         std::ranges::range C = std::initializer_list<T>>
constexpr auto polynomial(C&& coeff, std::integral auto x) -> T {
    return polynomial<T>(coeff, x);
}

} // namespace muc
