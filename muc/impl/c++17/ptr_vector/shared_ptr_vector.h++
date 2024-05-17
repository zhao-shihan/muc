#pragma once

#include "ptr_vector_base.h++"

#include <initializer_list>
#include <memory>
#include <memory_resource>
#include <utility>
#include <vector>

namespace muc {

template<typename T, typename Allocator = typename std::allocator<T>>
class shared_ptr_vector
    : public impl::ptr_vector_base<shared_ptr_vector<T, Allocator>,
                                   std::vector<std::shared_ptr<T>,
                                               typename std::allocator_traits<Allocator>::template rebind_alloc<std::shared_ptr<T>>>> {
    static_assert(std::is_same_v<typename Allocator::value_type, T>,
                  "muc::shared_ptr_vector must have the same value_type as its allocator");
    static_assert(not std::is_array_v<T>,
                  "value type of muc::shared_ptr_vector cannot be an array");

private:
    using base = impl::ptr_vector_base<shared_ptr_vector<T, Allocator>,
                                       std::vector<std::shared_ptr<T>,
                                                   typename std::allocator_traits<Allocator>::template rebind_alloc<std::shared_ptr<T>>>>;

    friend base; // for access allocate_value

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
    shared_ptr_vector() noexcept(noexcept(Allocator{})) = default;

    explicit shared_ptr_vector(const Allocator& alloc) noexcept :
        base{alloc} {}

    shared_ptr_vector(size_type count, const T& value, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vec) {
            ptr = allocate_value(value);
        }
    }

    shared_ptr_vector(size_type count, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vec) {
            ptr = allocate_value();
        }
    }

    template<typename InputIt>
    shared_ptr_vector(InputIt first, InputIt last, const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), first, last);
    }

    shared_ptr_vector(const shared_ptr_vector& other, const Allocator& alloc) :
        base{other, alloc} {}

    shared_ptr_vector(shared_ptr_vector&& other, const Allocator& alloc) :
        base{std::move(other), alloc} {}

    shared_ptr_vector(std::initializer_list<T> init, const Allocator& alloc = {}) :
        base{alloc} {
        this->insert(this->cend(), std::move(init));
    }

protected:
    template<typename... Args>
    auto allocate_value(Args&&... args) const -> std::shared_ptr<T> {
        return std::allocate_shared<T>(this->get_allocator(), std::forward<Args>(args)...);
    }
};

template<typename Pointer, typename Allocator>
auto swap(shared_ptr_vector<Pointer, Allocator>& lhs,
          shared_ptr_vector<Pointer, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
    lhs.swap(rhs);
}

namespace pmr {

template<typename T>
using shared_ptr_vector = ::muc::shared_ptr_vector<T, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr

} // namespace muc
