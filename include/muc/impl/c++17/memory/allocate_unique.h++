#pragma once

#include "muc/impl/c++17/memory/placement.h++"
#include "muc/impl/c++17/memory/to_address.h++"

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace muc {

template<typename Allocator>
struct allocator_delete : placement<Allocator> {
    allocator_delete() = default;

    explicit allocator_delete(const Allocator& alloc) :
        placement<Allocator>{alloc} {}

    explicit allocator_delete(Allocator&& alloc) :
        placement<Allocator>{std::move(alloc)} {}

    template<typename Alloc>
    allocator_delete(const allocator_delete<Alloc>& other) :
        Allocator{static_cast<const Alloc&>(other)} {}

    template<typename Alloc>
    allocator_delete(allocator_delete<Alloc>&& other) :
        Allocator{static_cast<Alloc&&>(other)} {}

    auto
    operator()(typename std::allocator_traits<Allocator>::pointer ptr) -> void {
        Allocator& alloc(*this);
        std::allocator_traits<Allocator>::destroy(alloc, muc::to_address(ptr));
        std::allocator_traits<Allocator>::deallocate(alloc, ptr, 1);
    }
};

template<typename T, typename Allocator, typename... Args>
auto allocate_unique(Allocator alloc, Args&&... args)
    -> std::unique_ptr<T, allocator_delete<Allocator>> {
    static_assert(
        std::is_same_v<typename std::allocator_traits<Allocator>::value_type,
                       std::remove_cv_t<T>>,
        "Allocator has the wrong value_type");
    const auto ptr{std::allocator_traits<Allocator>::allocate(alloc, 1)};
    const auto deallocate_when_failed{[&] {
        std::allocator_traits<Allocator>::deallocate(alloc, ptr, 1);
    }};
    try {
        std::allocator_traits<Allocator>::construct(
            alloc, muc::to_address(ptr), std::forward<Args>(args)...);
        return std::unique_ptr<T, allocator_delete<Allocator>>{
            ptr, allocator_delete<Allocator>{alloc}};
    } catch (const std::exception& e) {
        deallocate_when_failed();
        throw e;
    } catch (...) {
        deallocate_when_failed();
        throw;
    }
}

} // namespace muc
