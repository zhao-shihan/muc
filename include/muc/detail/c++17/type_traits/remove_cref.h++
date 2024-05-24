#pragma once

#include <type_traits>

namespace muc {

template<typename T>
struct remove_cref : std::remove_const<std::remove_reference_t<T>> {};

template<typename T>
using remove_cref_t = typename remove_cref<T>::type;

} // namespace muc
