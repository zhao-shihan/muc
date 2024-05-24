#pragma once

#include "muc/detail/c++20/tuple/tuple_like.h++"

namespace muc {

template<typename T>
concept pair_like = requires {
    requires tuple_like<T>;
    requires std::tuple_size_v<T> == 2;
};

} // namespace muc
