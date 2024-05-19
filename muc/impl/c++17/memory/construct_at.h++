#pragma once

#include <new>
#include <utility>

namespace muc {

template<typename T, typename... Args>
auto construct_at(T* ptr, Args&&... args) -> T* {
    return ::new (static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
}

} // namespace muc
