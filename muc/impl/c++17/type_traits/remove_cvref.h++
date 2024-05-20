#pragma once

#include <type_traits>

namespace muc {

template<typename T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = remove_cvref<T>::type;

} // namespace muc
