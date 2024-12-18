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

#include <cctype>

namespace muc {

inline auto isalnum(char c) -> bool {
    return std::isalnum(static_cast<unsigned char>(c));
}

inline auto isalpha(char c) -> bool {
    return std::isalpha(static_cast<unsigned char>(c));
}

inline auto iscntrl(char c) -> bool {
    return std::iscntrl(static_cast<unsigned char>(c));
}

inline auto isdigit(char c) -> bool {
    return std::isdigit(static_cast<unsigned char>(c));
}

inline auto isgraph(char c) -> bool {
    return std::isgraph(static_cast<unsigned char>(c));
}

inline auto islower(char c) -> bool {
    return std::islower(static_cast<unsigned char>(c));
}

inline auto isprint(char c) -> bool {
    return std::isprint(static_cast<unsigned char>(c));
}

inline auto ispunct(char c) -> bool {
    return std::ispunct(static_cast<unsigned char>(c));
}

inline auto isspace(char c) -> bool {
    return std::isspace(static_cast<unsigned char>(c));
}

inline auto isupper(char c) -> bool {
    return std::isupper(static_cast<unsigned char>(c));
}

inline auto isxdigit(char c) -> bool {
    return std::isxdigit(static_cast<unsigned char>(c));
}

inline auto tolower(char c) -> char {
    return std::tolower(static_cast<unsigned char>(c));
}

inline auto toupper(char c) -> char {
    return std::toupper(static_cast<unsigned char>(c));
}

} // namespace muc
