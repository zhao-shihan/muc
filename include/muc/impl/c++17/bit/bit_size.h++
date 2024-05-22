#pragma once

#include <climits>
#include <cstddef>

namespace muc {

template<typename T>
std::size_t bit_size{CHAR_BIT * sizeof(T)};

} // namespace muc
