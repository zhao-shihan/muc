#pragma once

#include <type_traits>

namespace muc {

template<typename T>
struct remove_vref : std::remove_volatile<std::remove_reference_t<T>> {};

template<typename T>
using remove_vref_t = typename remove_vref<T>::type;

} // namespace muc
