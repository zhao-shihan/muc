#pragma once

#include "muc/impl/c++20/concepts/move_assignable.h++"

#include <concepts>

namespace muc {

/// @brief C++ named requirements: CopyAssignable. This is more strict than
/// std::is_copy_assignable because it does check the type of the result of the
/// assignment (which, for a CopyAssignable type, must be an lvalue of type T).
/// See also: https://en.cppreference.com/w/cpp/named_req/copy_assignable
template<typename T>
concept copy_assignable =
    move_assignable<T> and std::assignable_from<T&, T&> and
    std::assignable_from<T&, const T&> and std::assignable_from<T&, const T&&>;

template<typename T>
concept noexcept_copy_assignable =
    noexcept_move_assignable<T> and std::assignable_from<T&, T&> and
    std::assignable_from<T&, const T&> and
    std::assignable_from<T&, const T&&> and
    std::is_nothrow_move_assignable_v<T>;

} // namespace muc
