#pragma once

#if __cplusplus >= 202002L // >= C++20
#include <version>
#ifdef __cpp_lib_unreachable // >= C++23
#define MUC_CPP_LIB_UNREACHABLE
#endif
#endif

#ifdef MUC_CPP_LIB_UNREACHABLE
#include <utility>
#endif

namespace muc {

[[noreturn]] inline auto unreachable() -> void {
#ifdef MUC_CPP_LIB_UNREACHABLE
    std::unreachable(); // C++23
#elif defined __clang__ or defined __GNUC__
    __builtin_unreachable();
#elif defined _MSC_VER
    __assume(false);
#endif
}

} // namespace muc

#undef MUC_CPP_LIB_UNREACHABLE
