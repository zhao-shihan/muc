#pragma once

#if not __has_cpp_attribute(assume) and \
    not(defined __clang__ or defined __GNUC__ or defined _MSC_VER)
#include "muc/detail/c++17/utility/unreachable.h++"
#endif

namespace muc {

constexpr auto assume(bool condition) noexcept -> void {
#if __has_cpp_attribute(assume)
    [[assume(condition)]]; // C++23
#elif defined __clang__
    __builtin_assume(condition);
#elif defined __GNUC__
    if (not condition) {
        __builtin_unreachable();
    }
#elif defined _MSC_VER
    __assume(condition);
#else
    if (not condition) {
        unreachable();
    }
#endif
}

} // namespace muc
