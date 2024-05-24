#pragma once

#include "muc/detail/c++17/type_traits/is_template_of.h++"

namespace muc {

template<typename T, template<typename...> typename Template>
concept InstantiatedFrom = is_template_of_v<Template, T>;

} // namespace muc
