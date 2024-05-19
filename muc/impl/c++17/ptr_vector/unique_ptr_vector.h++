#pragma once

#include "muc/impl/c++17/memory/allocate_unique.h++"
#include "muc/impl/c++17/ptr_vector/impl/ptr_vector_base.h++"

#include <initializer_list>
#include <memory>
#include <memory_resource>
#include <utility>
#include <vector>

namespace muc {

template<typename T, typename Allocator = typename std::allocator<T>>
class unique_ptr_vector
    : public impl::ptr_vector_base<unique_ptr_vector<T, Allocator>,
                                   std::vector<std::unique_ptr<T, allocator_delete<Allocator>>,
                                               typename std::allocator_traits<Allocator>::template rebind_alloc<std::unique_ptr<T, allocator_delete<Allocator>>>>> {
    static_assert(std::is_same_v<typename Allocator::value_type, T>,
                  "muc::shared_ptr_vector must have the same value_type as its allocator");
    static_assert(not std::is_array_v<T>,
                  "value type of muc::unique_ptr_vector cannot be an array");

private:
    using base = impl::ptr_vector_base<unique_ptr_vector<T, Allocator>,
                                       std::vector<std::unique_ptr<T, allocator_delete<Allocator>>,
                                                   typename std::allocator_traits<Allocator>::template rebind_alloc<std::unique_ptr<T, allocator_delete<Allocator>>>>>;

    friend base; // for access allocate_ptr

public:
    using value_type = typename base::value_type;
    using allocator_type = typename base::allocator_type;
    using size_type = typename base::size_type;
    using difference_type = typename base::difference_type;
    using reference = typename base::reference;
    using const_reference = typename base::const_reference;
    using pointer = typename base::pointer;
    using const_pointer = typename base::const_pointer;
    using iterator = typename base::iterator;
    using const_iterator = typename base::const_iterator;
    using reverse_iterator = typename base::reverse_iterator;
    using const_reverse_iterator = typename base::const_reverse_iterator;

public:
    unique_ptr_vector() noexcept(noexcept(Allocator{})) = default;

    explicit unique_ptr_vector(const Allocator& alloc) noexcept :
        base{alloc} {}

    unique_ptr_vector(size_type count, const T& value, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vector) {
            ptr = allocate_ptr(value);
        }
    }

    explicit unique_ptr_vector(size_type count, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vector) {
            ptr = allocate_ptr();
        }
    }

    template<typename InputIt>
    unique_ptr_vector(InputIt first, InputIt last, const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), first, last);
    }

    unique_ptr_vector(const unique_ptr_vector& other, const Allocator& alloc) :
        base{other, alloc} {}

    unique_ptr_vector(unique_ptr_vector&& other, const Allocator& alloc) :
        base{std::move(other), alloc} {}

    unique_ptr_vector(std::initializer_list<T> init, const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), std::move(init));
    }

protected:
    template<typename... Args>
    auto allocate_ptr(Args&&... args) const -> std::unique_ptr<T, allocator_delete<Allocator>> {
        return allocate_unique<T>(this->get_allocator(), std::forward<Args>(args)...);
    }
};

template<typename Pointer, typename Allocator>
auto swap(unique_ptr_vector<Pointer, Allocator>& lhs,
          unique_ptr_vector<Pointer, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
    lhs.swap(rhs);
}

namespace pmr {

template<typename T>
using unique_ptr_vector = ::muc::unique_ptr_vector<T, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr

} // namespace muc
