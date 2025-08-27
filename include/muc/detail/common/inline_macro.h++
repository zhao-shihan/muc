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

#if defined _MSC_VER
#define MUC_STRONG_INLINE __forceinline
#else
#define MUC_STRONG_INLINE inline
#endif

#if defined __clang__
#define MUC_ALWAYS_INLINE [[clang::always_inline]] inline
#elif defined __GNUC__
#define MUC_ALWAYS_INLINE [[gnu::always_inline]] inline
#else
#define MUC_ALWAYS_INLINE MUC_STRONG_INLINE
#endif

#if defined __clang__
#define MUC_NOINLINE [[clang::noinline]]
#elif defined __GNUC__
#define MUC_NOINLINE [[gnu::noinline]]
#elif defined _MSC_VER
#define MUC_NOINLINE __declspec(noinline)
#endif
