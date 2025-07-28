// -*- C++ -*-
//
// Copyright 2021-2025  Shihan Zhao
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

#include "muc/detail/common/inline_macro.h++"

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace muc {

MUC_ALWAYS_INLINE auto cpu_relax() -> void {
#if defined __i386__ or defined __x86_64__ or defined _M_IX86 or defined _M_X64
    { // x86
#if defined _MSC_VER
        ::_mm_pause();
#else
        __builtin_ia32_pause();
#endif
    }
#elif defined __arm__ or defined __aarch64__ or defined _M_ARM or \
    defined _M_ARM64
    { // ARM
#if defined _MSC_VER
        __yield();
#else
        asm volatile("yield" ::: "memory");
#endif
    }
#elif defined __powerpc__ or defined __ppc__ or defined __PPC__ or \
    defined __powerpc64__
    { // PowerPC
        // TODO: nop as a workaround, need a real pause if possible
        asm volatile("nop" ::: "memory");
    }
#elif defined __mips__ && (__mips_isa_rev >= 2)
    { // MIPS (Release 2+)
        asm volatile("pause" ::: "memory");
    }
#elif defined __riscv
    { // RISC-V
        asm volatile("pause" ::: "memory");
    }
#elif defined __sparc__
    { // SPARC
        asm volatile("rd %%ccr, %%g0" ::: "memory");
    }
#elif defined __s390x__ or defined __zarch__
    { // IBM z/Architecture
        // TODO: nop as a workaround, need a real pause if possible
        asm volatile("nop" ::: "memory");
    }
#elif defined __loongarch__
    { // loongarch
        // TODO: nop as a workaround, need a real pause if possible
        asm volatile("nop" ::: "memory");
    }
#elif defined __alpha__
    { // Alpha
        // TODO: memory barrier as a workaround, need a real pause if possible
        asm volatile("mb" ::: "memory");
    }
#else
    { // fallback
        asm volatile("" ::: "memory");
    }
#endif
}

} // namespace muc
