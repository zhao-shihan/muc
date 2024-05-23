#pragma once

#include "muc/impl/c++17/memory/to_address.h++"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace muc::impl {

template<bool Const, typename Derived, typename RawPtrVector,
         typename Implementer>
class indirect_random_access_iterator {
    friend Implementer;

public:
    using difference_type = std::ptrdiff_t;
    using value_type =
        std::conditional_t<Const, const typename Derived::value_type,
                           typename Derived::value_type>;
    using pointer = std::conditional_t<Const, typename Derived::const_pointer,
                                       typename Derived::pointer>;
    using reference =
        std::conditional_t<Const, typename Derived::const_reference,
                           typename Derived::reference>;
    using iterator_category = std::random_access_iterator_tag;

private:
    using raw_iterator =
        std::conditional_t<Const, typename RawPtrVector::const_iterator,
                           typename RawPtrVector::iterator>;

public:
    indirect_random_access_iterator() :
        m_iter{} {}

    explicit indirect_random_access_iterator(raw_iterator iter) :
        m_iter{iter} {}

    operator indirect_random_access_iterator<true, Derived, RawPtrVector,
                                             Implementer>() {
        return indirect_random_access_iterator<true, Derived, RawPtrVector,
                                               Implementer>{m_iter};
    }

    auto operator*() const -> reference {
        return **m_iter;
    }

    auto operator->() const -> pointer {
        return muc::to_address(*m_iter);
    }

    auto operator[](std::size_t index) const -> reference {
        return *m_iter[index];
    }

    auto
    operator-(indirect_random_access_iterator other) const -> difference_type {
        return m_iter - other.m_iter;
    }

    auto operator++() -> indirect_random_access_iterator& {
        m_iter++;
        return *this;
    }

    auto operator++(int) -> indirect_random_access_iterator {
        auto next{*this};
        ++(*this);
        return next;
    }

    auto operator--() -> indirect_random_access_iterator& {
        m_iter--;
        return *this;
    }

    auto operator--(int) -> indirect_random_access_iterator {
        auto prev{*this};
        --(*this);
        return prev;
    }

    auto operator+=(difference_type n) -> indirect_random_access_iterator& {
        m_iter += n;
        return *this;
    }

    auto operator-=(difference_type n) -> indirect_random_access_iterator& {
        m_iter -= n;
        return *this;
    }

    auto operator+(difference_type n) const -> indirect_random_access_iterator {
        return indirect_random_access_iterator{m_iter + n};
    }

    auto operator-(difference_type n) const -> indirect_random_access_iterator {
        return indirect_random_access_iterator{m_iter - n};
    }

    auto
    operator==(const indirect_random_access_iterator& other) const -> bool {
        return m_iter == other.m_iter;
    }
#if __cplusplus >= 202002L
    auto
    operator<=>(const indirect_random_access_iterator& other) const -> auto {
        return m_iter <=> other.m_iter;
    }
#else
    auto
    operator!=(const indirect_random_access_iterator& other) const -> bool {
        return m_iter != other.m_iter;
    }

    auto operator<(const indirect_random_access_iterator& other) const -> bool {
        return m_iter < other.m_iter;
    }

    auto
    operator<=(const indirect_random_access_iterator& other) const -> bool {
        return m_iter <= other.m_iter;
    }

    auto operator>(const indirect_random_access_iterator& other) const -> bool {
        return m_iter > other.m_iter;
    }

    auto
    operator>=(const indirect_random_access_iterator& other) const -> bool {
        return m_iter >= other.m_iter;
    }
#endif
private:
    raw_iterator m_iter;
};

template<typename T>
struct obj_ptr_impl {
    struct type : T {
    public:
        using T::T;

        auto operator->() noexcept -> T* {
            return this;
        }

        auto operator->() const noexcept -> const T* {
            return this;
        }
    };
};

template<typename T>
struct obj_ptr_impl<T*> {
    using type = T*;
};

template<typename T>
using obj_ptr = typename obj_ptr_impl<T>::type;

template<typename Derived, typename RawPtrVectorObjOrPtr>
class ptr_vector_base {
private:
    using raw_ptr_vector = std::remove_pointer_t<RawPtrVectorObjOrPtr>;

protected:
    using value_type = typename std::pointer_traits<
        typename raw_ptr_vector::value_type>::element_type;
    using allocator_type = typename std::allocator_traits<
        typename raw_ptr_vector::allocator_type>::
        template rebind_alloc<value_type>;
    using size_type = typename raw_ptr_vector::size_type;
    using difference_type = typename raw_ptr_vector::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer =
        typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator =
        impl::indirect_random_access_iterator<false, Derived, raw_ptr_vector,
                                              ptr_vector_base>;
    using const_iterator =
        impl::indirect_random_access_iterator<true, Derived, raw_ptr_vector,
                                              ptr_vector_base>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

protected:
    ptr_vector_base(const RawPtrVectorObjOrPtr& ptr_vec) :
        m_ptr_vector{ptr_vec} {}

    ptr_vector_base(RawPtrVectorObjOrPtr&& ptr_vec) :
        m_ptr_vector{std::move(ptr_vec)} {}

    template<typename... Args>
    ptr_vector_base(Args&&... args) :
        m_ptr_vector{std::forward<Args>(args)...} {}

    ptr_vector_base(const ptr_vector_base&) = default;

    ptr_vector_base(ptr_vector_base&&) = default;

public:
    //
    // Assignment
    //

    auto operator=(const ptr_vector_base& other) -> ptr_vector_base& = default;

    auto operator=(ptr_vector_base&& other) noexcept(
        std::is_nothrow_move_constructible_v<raw_ptr_vector>)
        -> ptr_vector_base& = default;

    auto
    operator=(std::initializer_list<value_type> ilist) -> ptr_vector_base& {
        m_ptr_vector->clear();
        insert(cend(), std::move(ilist));
        return *this;
    }

    auto assign(size_type count, const value_type& value) -> void {
        m_ptr_vector->clear();
        insert(cend(), count, value);
    }

    template<typename InputIt>
    auto assign(InputIt first, InputIt last) -> void {
        m_ptr_vector->clear();
        insert(cend(), first, last);
    }

    auto assign(std::initializer_list<value_type> ilist) -> void {
        m_ptr_vector->clear();
        insert(cend(), std::move(ilist));
    }

    auto get_allocator() const -> allocator_type {
        return allocator_type{m_ptr_vector->get_allocator()};
    }

    //
    // Element access
    //

    auto at(size_type pos) -> reference {
        range_check(pos);
        return (*this)[pos];
    }

    auto at(size_type pos) const -> const_reference {
        range_check(pos);
        return (*this)[pos];
    }

    auto operator[](size_type pos) -> reference {
        assert(pos < size());
        return *m_ptr_vector[pos];
    }

    auto operator[](size_type pos) const -> const_reference {
        assert(pos < size());
        return *m_ptr_vector[pos];
    }

    auto front() -> reference {
        assert(not empty());
        return *m_ptr_vector->front();
    }

    auto front() const -> const_reference {
        assert(not empty());
        return *m_ptr_vector->front();
    }

    auto back() -> reference {
        assert(not empty());
        return *m_ptr_vector->back();
    }

    auto back() const -> const_reference {
        assert(not empty());
        return *m_ptr_vector->back();
    }

    auto ptr_vector() -> raw_ptr_vector& {
        if constexpr (std::is_pointer_v<RawPtrVectorObjOrPtr>) {
            return *m_ptr_vector;
        } else {
            return m_ptr_vector;
        }
    }

    auto ptr_vector() const -> const raw_ptr_vector& {
        if constexpr (std::is_pointer_v<RawPtrVectorObjOrPtr>) {
            return *m_ptr_vector;
        } else {
            return m_ptr_vector;
        }
    }

    //
    // Iterators
    //

    auto begin() const noexcept -> const_iterator {
        return const_iterator{m_ptr_vector->begin()};
    }

    auto cbegin() const noexcept -> const_iterator {
        return begin();
    }

    auto end() const noexcept -> const_iterator {
        return const_iterator{m_ptr_vector->end()};
    }

    auto cend() const noexcept -> const_iterator {
        return end();
    }

    auto rbegin() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator{end()};
    }

    auto crbegin() const noexcept -> const_reverse_iterator {
        return rbegin();
    }

    auto rend() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator{begin()};
    }

    auto crend() const noexcept -> const_reverse_iterator {
        return rend();
    }

    //
    // Pointer vector iterator
    //

    auto pbegin() noexcept -> typename raw_ptr_vector::iterator {
        return m_ptr_vector->begin();
    }

    auto pbegin() const noexcept -> typename raw_ptr_vector::const_iterator {
        return m_ptr_vector->begin();
    }

    auto pcbegin() const noexcept -> typename raw_ptr_vector::const_iterator {
        return m_ptr_vector->cbegin();
    }

    auto pend() noexcept -> typename raw_ptr_vector::iterator {
        return m_ptr_vector->end();
    }

    auto pend() const noexcept -> typename raw_ptr_vector::const_iterator {
        return m_ptr_vector->end();
    }

    auto pcend() const noexcept -> typename raw_ptr_vector::const_iterator {
        return m_ptr_vector->cend();
    }

    auto prbegin() noexcept -> typename raw_ptr_vector::reverse_iterator {
        return m_ptr_vector->rbegin();
    }

    auto prbegin() const noexcept ->
        typename raw_ptr_vector::const_reverse_iterator {
        return m_ptr_vector->rbegin();
    }

    auto pcrbegin() const noexcept ->
        typename raw_ptr_vector::const_reverse_iterator {
        return m_ptr_vector->crbegin();
    }

    auto prend() noexcept -> typename raw_ptr_vector::reverse_iterator {
        return m_ptr_vector->rend();
    }

    auto prend() const noexcept ->
        typename raw_ptr_vector::const_reverse_iterator {
        return m_ptr_vector->rend();
    }

    auto pcrend() const noexcept ->
        typename raw_ptr_vector::const_reverse_iterator {
        return m_ptr_vector->crend();
    }

    //
    // Writable iterators & range
    //

    auto vbegin() noexcept -> iterator {
        return iterator{m_ptr_vector->begin()};
    }

    auto vend() noexcept -> iterator {
        return iterator{m_ptr_vector->end()};
    }

    auto vrbegin() noexcept -> reverse_iterator {
        return reverse_iterator{vend()};
    }

    auto vrend() noexcept -> reverse_iterator {
        return reverse_iterator{vbegin()};
    }

    class vrange_type {
        friend auto
        ptr_vector_base::writable_range() const noexcept -> vrange_type;

    public:
        auto begin() const noexcept -> iterator {
            return iterator{m_ptr_vector->begin()};
        }

        auto end() const noexcept -> iterator {
            return iterator{m_ptr_vector->end()};
        }

        auto rbegin() const noexcept -> reverse_iterator {
            return reverse_iterator{end()};
        }

        auto rend() const noexcept -> reverse_iterator {
            return reverse_iterator{begin()};
        }

    private:
        raw_ptr_vector* m_ptr_vector;
    };

    auto vrange() const noexcept -> vrange_type {
        return m_ptr_vector;
    }

    //
    // Capacity
    //

    [[nodiscard]] auto empty() const noexcept -> bool {
        return m_ptr_vector->empty();
    }

    auto size() const noexcept -> size_type {
        return m_ptr_vector->size();
    }

    auto max_size() const noexcept -> size_type {
        return m_ptr_vector->max_size();
    }

    auto reserve() -> void {
        return m_ptr_vector->reserve();
    }

    auto capacity() const noexcept -> size_type {
        return m_ptr_vector->capacity();
    }

    auto shrink_to_fit() -> void {
        return m_ptr_vector->shrink_to_fit();
    }

    //
    // Modifier
    //

    auto clear() -> void {
        m_ptr_vector->clear();
    }

    auto insert(const_iterator pos, const value_type& value) -> iterator {
        return emplace(pos, value);
    }

    auto insert(const_iterator pos, value_type&& value) -> iterator {
        return emplace(pos, std::move(value));
    }

    auto insert(const_iterator pos, size_type count,
                const value_type& value) -> iterator {
        const auto i_pos{pos - cbegin()};

        m_ptr_vector->resize(size() + count);
        const auto first{vbegin() + i_pos};
        const auto last{first + count};

        std::move_backward(first.m_iter, last.m_iter, vend().m_iter);
        std::generate(first.m_iter, last.m_iter, [&]() {
            return allocate_ptr();
        });

        return first;
    }

    template<typename InputIt>
    auto insert(const_iterator pos, InputIt first, InputIt last) -> iterator {
        const auto i_pos{pos - cbegin()};
        const auto count{std::distance(first, last)};

        m_ptr_vector->resize(size() + count);
        const auto dst_first{vbegin() + i_pos};
        const auto dst_last{dst_first + count};

        std::move_backward(dst_first.m_iter, dst_last.m_iter, vend().m_iter);
        std::generate(dst_first.m_iter, dst_last.m_iter, [&]() {
            return allocate_ptr(*first++);
        });

        return dst_first;
    }

    auto insert(const_iterator pos,
                std::initializer_list<value_type> ilist) -> iterator {
        return insert(pos, ilist.begin(), ilist.end());
    }

    // #if __cplusplus>=

    // // TODO: insert_range

    // #endif

    template<typename... Args>
    auto emplace(const_iterator pos, Args&&... args) -> iterator {
        return iterator{m_ptr_vector->emplace(
            pos.m_iter, allocate_ptr(std::forward<Args>(args)...))};
    }

    auto erase(const_iterator pos) -> iterator {
        return iterator{m_ptr_vector->erase(pos.m_iter)};
    }

    auto erase(const_iterator first, const_iterator last) -> iterator {
        return iterator{m_ptr_vector->erase(first.m_iter, last.m_iter)};
    }

    auto push_back(const value_type& value) -> void {
        emplace_back(value);
    }

    auto push_back(value_type&& value) -> void {
        emplace_back(std::move(value));
    }

    template<typename... Args>
    auto emplace_back(Args&&... args) -> reference {
        return *emplace_back(allocate_ptr(std::forward<Args>(args)...));
    }

    auto pop_back() -> void {
        m_ptr_vector->pop_back();
    }

    auto resize(size_type count) -> void {
        if (size() >= count) {
            erase(cbegin() + count, cend());
        } else {
            const auto first{
                m_ptr_vector->insert(m_ptr_vector->cend(), count - size(), {})};
            const auto last{m_ptr_vector->end()};
            for (auto i{first}; i != last; ++i) {
                *i = allocate_ptr();
            }
        }
    }

    auto resize(size_type count, const value_type& value) -> void {
        if (size() >= count) {
            erase(cbegin() + count, cend());
        } else {
            const auto first{
                m_ptr_vector->insert(m_ptr_vector->cend(), count - size(), {})};
            const auto last{m_ptr_vector->end()};
            for (auto i{first}; i != last; ++i) {
                *i = allocate_ptr(value);
            }
        }
    }

    auto swap(ptr_vector_base& other) noexcept -> void {
        m_ptr_vector->swap(other.m_ptr_vector);
    }

    //
    // Compare
    //

    auto operator==(const ptr_vector_base& other) -> bool {
        return m_ptr_vector == other.m_ptr_vector;
    }

    auto operator!=(const ptr_vector_base& other) -> bool {
        return m_ptr_vector != other.m_ptr_vector;
    }

    auto operator<(const ptr_vector_base& other) -> bool {
        return m_ptr_vector < other.m_ptr_vector;
    }

    auto operator<=(const ptr_vector_base& other) -> bool {
        return m_ptr_vector <= other.m_ptr_vector;
    }

    auto operator>(const ptr_vector_base& other) -> bool {
        return m_ptr_vector > other.m_ptr_vector;
    }

    auto operator>=(const ptr_vector_base& other) -> bool {
        return m_ptr_vector >= other.m_ptr_vector;
    }

protected:
    auto range_check(size_type pos) const -> void {
        if (pos >= size()) {
            throw std::out_of_range{""};
        }
    }

private:
    template<typename... Args>
    auto allocate_ptr(Args&&... args) const ->
        typename raw_ptr_vector::value_type {
        return static_cast<const Derived*>(this)->allocate_ptr(
            std::forward<Args>(args)...);
    }

protected:
    obj_ptr<RawPtrVectorObjOrPtr> m_ptr_vector;
};

} // namespace muc::impl
