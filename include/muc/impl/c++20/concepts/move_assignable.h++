#pragma once

#include <concepts>

namespace muc {

/// @brief C++ named requirements: MoveAssignable. This is more strict than
/// std::is_move_assignable because it does check the type of the result of the
/// assignment (which, for a CopyAssignable type, must be an lvalue of type T).
/// See also: https://en.cppreference.com/w/cpp/named_req/move_assignable
template<typename T>
concept move_assignable = std::assignable_from<T&, T&&>;

template<typename T>
concept noexcept_move_assignable =
    std::assignable_from<T&, T&&> and std::is_nothrow_move_assignable_v<T>;

} // namespace muc
