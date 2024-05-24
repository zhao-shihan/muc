#pragma once

#include <type_traits>

namespace muc {

template<template<typename...> typename, typename>
struct is_template_of : std::false_type {};

template<template<typename...> typename Template, typename... Args>
struct is_template_of<Template, Template<Args...>> : std::true_type {};

template<template<typename...> typename Template, typename T>
inline constexpr bool is_template_of_v{is_template_of<Template, T>::value};

} // namespace muc
