#pragma once

#include "muc/detail/c++20/concepts/boolean_testable.h++"
#include "muc/detail/c++20/tuple/apply.h++"
#include "muc/detail/c++20/tuple/tuple_like.h++"

#include <utility>

namespace muc {

template<typename F, typename T>
concept applicable_on = requires(F&& Func, T&& t) {
    requires tuple_like<T>;
    { apply(std::forward<F>(Func), std::forward<T>(t)) };
};

template<typename F, typename T>
concept predicate_applicable_on = requires(F&& Func, T&& t) {
    requires tuple_like<T>;
    { apply(std::forward<F>(Func), std::forward<T>(t)) } -> boolean_testable;
};

} // namespace muc
