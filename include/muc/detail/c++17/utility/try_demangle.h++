// -*- C++ -*-
//
// Copyright 2024  Shihan Zhao
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

#include <cstdlib>
#include <stdexcept>
#include <string>
#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif

namespace muc {

inline auto try_demangle(const char* name, int& status) -> std::string {
    status = 0;
    if constexpr (requires(int status) {
                      abi::__cxa_demangle(name, nullptr, nullptr, &status);
                  }) {
        const auto c_result{
            abi::__cxa_demangle(name, nullptr, nullptr, &status)};
        const std::string result{c_result};
        std::free(c_result);
        return result;
    } else {
        return name;
    }
}

inline auto try_demangle(const char* name) -> std::string {
    int status;
    const auto result{try_demangle(name, status)};
    if constexpr (requires(int status) {
                      abi::__cxa_demangle(name, nullptr, nullptr, &status);
                  }) {
        switch (status) {
        case 0:
            break;
        case 1:
            throw std::runtime_error{
                "abi::__cxa_demangle: A memory allocation failure occurred"};
        case 2:
            throw std::runtime_error{
                "abi::__cxa_demangle: mangled_name is not a valid name under "
                "the C++ ABI mangling rules"};
        case 3:
            throw std::runtime_error{
                "abi::__cxa_demangle: One of the arguments is invalid"};
        }
    }
    return result;
}

} // namespace muc
