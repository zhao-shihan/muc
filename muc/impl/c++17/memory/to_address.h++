#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace muc {

template<typename T>
constexpr auto to_address(T* ptr) noexcept -> T* {
    static_assert(not std::is_function_v<T>);
    return ptr;
}

namespace impl {

template<typename, typename = void>
struct has_pointer_traits_to_address
    : std::false_type {};

template<typename T>
struct has_pointer_traits_to_address<
    T, std::void_t<decltype(std::pointer_traits<T>::to_address(std::declval<const T&>()))>>
    : std::true_type {};

} // namespace impl

template<typename T>
constexpr auto to_address(const T& ptr) noexcept -> auto {
    if constexpr (impl::has_pointer_traits_to_address<T>::value) {
        return std::pointer_traits<T>::to_address(ptr);
    } else {
        return muc::to_address(ptr.operator->());
    }
}

} // namespace muc
