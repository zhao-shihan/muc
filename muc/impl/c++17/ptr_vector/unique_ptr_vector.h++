#pragma once

#include "ptr_vector_base.h++"

#include <initializer_list>
#include <memory>
#include <memory_resource>
#include <utility>
#include <vector>

namespace muc {

namespace impl {

template<typename Allocator>
struct delete_by_allocator : private Allocator {
    delete_by_allocator() = default;

    explicit delete_by_allocator(const Allocator& alloc) :
        Allocator{alloc} {}

    explicit delete_by_allocator(Allocator&& alloc) :
        Allocator{std::move(alloc)} {}

    auto operator()(typename std::allocator_traits<Allocator>::pointer ptr) noexcept -> void {
        Allocator& alloc{*this};
        std::allocator_traits<Allocator>::destory(alloc, &*ptr);
        std::allocator_traits<Allocator>::deallocate(alloc, ptr, 1);
    }
};

} // namespace impl

template<typename T, typename Allocator = typename std::allocator<T>>
class unique_ptr_vector
    : public impl::ptr_vector_base<unique_ptr_vector<T, Allocator>,
                                   std::vector<std::unique_ptr<T, impl::delete_by_allocator<Allocator>>,
                                               typename std::allocator_traits<Allocator>::template rebind_alloc<std::unique_ptr<T, impl::delete_by_allocator<Allocator>>>>> {
    static_assert(std::is_same_v<typename Allocator::value_type, T>,
                  "muc::shared_ptr_vector must have the same value_type as its allocator");
    static_assert(not std::is_array_v<T>,
                  "value type of muc::unique_ptr_vector cannot be an array");

private:
    using base = impl::ptr_vector_base<unique_ptr_vector<T, Allocator>,
                                       std::vector<std::unique_ptr<T, impl::delete_by_allocator<Allocator>>,
                                                   typename std::allocator_traits<Allocator>::template rebind_alloc<std::unique_ptr<T, impl::delete_by_allocator<Allocator>>>>>;

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
    unique_ptr_vector() noexcept(noexcept(Allocator{})) = default;

    explicit unique_ptr_vector(const Allocator& alloc) noexcept :
        base{alloc} {}

    unique_ptr_vector(size_type count, const T& value, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vec) {
            ptr = allocate_value(value);
        }
    }

    unique_ptr_vector(size_type count, const Allocator& alloc = {}) :
        base{count, alloc} {
        for (auto&& ptr : this->m_ptr_vec) {
            ptr = allocate_value();
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
    auto allocate_value(Args&&... args) const -> std::unique_ptr<T, impl::delete_by_allocator<Allocator>> {
        auto alloc{this->get_allocator()};
        const auto ptr{std::allocator_traits<Allocator>::allocate(alloc, 1)};
        std::allocator_traits<Allocator>::construct(alloc, &*ptr, std::forward<Args>(args)...);
        return std::unique_ptr<T, impl::delete_by_allocator<Allocator>>{&*ptr, impl::delete_by_allocator{alloc}};
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
