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

#include <complex>
#include <type_traits>
#include <utility>

namespace muc::impl {

#define MUC_IMPL_HAS_SCALAR_MUL_IMPL_IMPL(SCALAR)                             \
    std::conjunction</* std::is_convertible<decltype(+std::declval<const      \
                        T&>()), T>,*/                                         \
                     std::is_convertible<decltype(-std::declval<const T&>()), \
                                         T>,                                  \
                     std::is_convertible<                                     \
                         decltype(SCALAR * std::declval<const T&>()), T>,     \
                     std::is_convertible<                                     \
                         decltype(std::declval<const T&>() * SCALAR), T>,     \
                     std::is_convertible<                                     \
                         decltype(std::declval<const T&>() / SCALAR), T>,     \
                     std::is_convertible<                                     \
                         decltype(std::declval<T&>() *= SCALAR), T>,          \
                     std::is_convertible<                                     \
                         decltype(std::declval<T&>() /= SCALAR), T>>

#define MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(signature, scalar_type) \
    template<typename, typename = void>                                 \
    struct has_scalar_mul_##signature##_impl : std::false_type {};      \
                                                                        \
    template<typename T>                                                \
    struct has_scalar_mul_##signature##                                 \
        _impl<T, std::void_t<MUC_IMPL_HAS_SCALAR_MUL_IMPL_IMPL(         \
                     std::declval<scalar_type>())>> :                   \
        MUC_IMPL_HAS_SCALAR_MUL_IMPL_IMPL(std::declval<scalar_type>()) {};

MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(f, float)
MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(d, double)
MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(ld, long double)
MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(cf, std::complex<float>)
MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(cd, std::complex<double>)
MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION(cld, std::complex<long double>)

#undef MUC_IMPL_HAS_SCALAR_MUL_IMPL_DEFINITION
#undef MUC_IMPL_HAS_SCALAR_MUL_IMPL_IMPL

template<typename T>
struct has_scalar_mul :
    std::disjunction<has_scalar_mul_f_impl<T>, has_scalar_mul_d_impl<T>,
                     has_scalar_mul_ld_impl<T>, has_scalar_mul_cf_impl<T>,
                     has_scalar_mul_cd_impl<T>, has_scalar_mul_cld_impl<T>> {};

#define MUC_IMPL_HAS_ADD_SUB_IMPL                                              \
    std::conjunction<                                                          \
        std::is_convertible<                                                   \
            decltype(std::declval<const T&>() + std::declval<const T&>()), T>, \
        std::is_convertible<                                                   \
            decltype(std::declval<const T&>() - std::declval<const T&>()), T>, \
        std::is_convertible<                                                   \
            decltype(std::declval<const T&>() + std::declval<const U&>()), T>, \
        std::is_convertible<                                                   \
            decltype(std::declval<const T&>() - std::declval<const U&>()), T>, \
        std::is_convertible<                                                   \
            decltype(std::declval<T&>() += std::declval<const U&>()), T>,      \
        std::is_convertible<                                                   \
            decltype(std::declval<T&>() -= std::declval<const U&>()), T>>

template<typename, typename, typename = void>
struct has_add_sub : std::false_type {};

template<typename T, typename U>
struct has_add_sub<T, U, std::void_t<MUC_IMPL_HAS_ADD_SUB_IMPL>> :
    MUC_IMPL_HAS_ADD_SUB_IMPL {};

} // namespace muc::impl
