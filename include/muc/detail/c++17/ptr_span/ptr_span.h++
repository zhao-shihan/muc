#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace muc {

template<typename RawPtrContainer>
class ptr_span {
public:
    using value_type = std::conditional_t<
        std::is_const_v<RawPtrContainer>,
        const typename std::pointer_traits<
            typename RawPtrContainer::value_type>::element_type,
        typename std::pointer_traits<
            typename RawPtrContainer::value_type>::element_type>;
    using size_type = typename RawPtrContainer::size_type;
    using difference_type = typename RawPtrContainer::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::pointer_traits<
        typename RawPtrContainer::value_type>::template rebind<value_type>;
    using const_pointer = typename std::pointer_traits<
        pointer>::template rebind<const value_type>;

private:
    template<typename RawIterator>
    class basic_iterator {
        friend class ptr_span;

    public:
        using difference_type = typename ptr_span::difference_type;
        using value_type = std::conditional_t<
            std::is_const_v<
                typename std::iterator_traits<RawIterator>::value_type>,
            const typename ptr_span::value_type, typename ptr_span::value_type>;
        using pointer = std::conditional_t<
            std::is_const_v<
                typename std::iterator_traits<RawIterator>::value_type>,
            typename ptr_span::const_pointer, typename ptr_span::pointer>;
        using reference = std::conditional_t<
            std::is_const_v<
                typename std::iterator_traits<RawIterator>::value_type>,
            typename ptr_span::const_reference, typename ptr_span::reference>;
        using iterator_category = std::random_access_iterator_tag;
// 2024-05-24 here
    public:
        basic_iterator() :
            m_iter{} {}

        explicit basic_iterator(RawIterator iter) :
            m_iter{iter} {}

        operator basic_iterator<typename RawPtrContainer::const_iterator>() {
            return basic_iterator<typename RawPtrContainer::const_iterator>{
                m_iter};
        }

        auto operator*() const -> reference {
            return **m_iter;
        }

        auto operator->() const -> pointer {
            return *m_iter;
        }

        auto operator[](std::size_t index) const -> reference {
            return *m_iter[index];
        }

        auto operator-(basic_iterator other) const -> difference_type {
            return m_iter - other.m_iter;
        }

        auto operator++() -> basic_iterator& {
            m_iter++;
            return *this;
        }

        auto operator++(int) -> basic_iterator {
            auto next{*this};
            ++(*this);
            return next;
        }

        auto operator--() -> basic_iterator& {
            m_iter--;
            return *this;
        }

        auto operator--(int) -> basic_iterator {
            auto prev{*this};
            --(*this);
            return prev;
        }

        auto operator+=(difference_type n) -> basic_iterator& {
            m_iter += n;
            return *this;
        }

        auto operator-=(difference_type n) -> basic_iterator& {
            m_iter -= n;
            return *this;
        }

        auto operator+(difference_type n) const -> basic_iterator {
            return basic_iterator{m_iter + n};
        }

        auto operator-(difference_type n) const -> basic_iterator {
            return basic_iterator{m_iter - n};
        }

        auto operator==(const basic_iterator& other) const -> bool {
            return m_iter == other.m_iter;
        }
#if __cplusplus >= 202002L
        auto operator<=>(const basic_iterator& other) const -> auto {
            return m_iter <=> other.m_iter;
        }
#else
        auto operator!=(const basic_iterator& other) const -> bool {
            return m_iter != other.m_iter;
        }

        auto operator<(const basic_iterator& other) const -> bool {
            return m_iter < other.m_iter;
        }

        auto operator<=(const basic_iterator& other) const -> bool {
            return m_iter <= other.m_iter;
        }

        auto operator>(const basic_iterator& other) const -> bool {
            return m_iter > other.m_iter;
        }

        auto operator>=(const basic_iterator& other) const -> bool {
            return m_iter >= other.m_iter;
        }
#endif
    private:
        RawIterator m_iter;
    };

protected:
    using iterator = basic_iterator<typename RawPtrContainer::iterator>;
    using const_iterator =
        basic_iterator<typename RawPtrContainer::const_iterator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

protected:
    ptr_span(const RawPtrContainer& ptr_vec) :
        m_ptr_vector{ptr_vec} {}

    ptr_span(RawPtrContainer&& ptr_vec) :
        m_ptr_vector{std::move(ptr_vec)} {}

    template<typename... Args>
    ptr_span(Args&&... args) :
        m_ptr_vector{std::forward<Args>(args)...} {}

    ptr_span(const ptr_span&) = default;

    ptr_span(ptr_span&&) = default;

public:
    //
    // Assignment
    //

    auto operator=(const ptr_span& other) -> ptr_span& = default;

    auto operator=(ptr_span&& other) noexcept(
        std::is_nothrow_move_constructible_v<RawPtrContainer>) -> ptr_span& =
                                                                      default;

    auto operator=(std::initializer_list<value_type> ilist) -> ptr_span& {
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

    auto ptr_vector() -> RawPtrContainer& {
        return m_ptr_vector;
    }

    auto ptr_vector() const -> const RawPtrContainer& {
        return m_ptr_vector;
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

    auto pbegin() noexcept -> typename RawPtrContainer::iterator {
        return m_ptr_vector->begin();
    }

    auto pbegin() const noexcept -> typename RawPtrContainer::const_iterator {
        return m_ptr_vector->begin();
    }

    auto pcbegin() const noexcept -> typename RawPtrContainer::const_iterator {
        return m_ptr_vector->cbegin();
    }

    auto pend() noexcept -> typename RawPtrContainer::iterator {
        return m_ptr_vector->end();
    }

    auto pend() const noexcept -> typename RawPtrContainer::const_iterator {
        return m_ptr_vector->end();
    }

    auto pcend() const noexcept -> typename RawPtrContainer::const_iterator {
        return m_ptr_vector->cend();
    }

    auto prbegin() noexcept -> typename RawPtrContainer::reverse_iterator {
        return m_ptr_vector->rbegin();
    }

    auto prbegin() const noexcept ->
        typename RawPtrContainer::const_reverse_iterator {
        return m_ptr_vector->rbegin();
    }

    auto pcrbegin() const noexcept ->
        typename RawPtrContainer::const_reverse_iterator {
        return m_ptr_vector->crbegin();
    }

    auto prend() noexcept -> typename RawPtrContainer::reverse_iterator {
        return m_ptr_vector->rend();
    }

    auto prend() const noexcept ->
        typename RawPtrContainer::const_reverse_iterator {
        return m_ptr_vector->rend();
    }

    auto pcrend() const noexcept ->
        typename RawPtrContainer::const_reverse_iterator {
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
        friend auto ptr_span::vrange() const noexcept -> vrange_type;

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
        RawPtrContainer* m_ptr_vector;
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

    auto swap(ptr_span& other) noexcept -> void {
        m_ptr_vector->swap(other.m_ptr_vector);
    }

    //
    // Compare
    //

    auto operator==(const ptr_span& other) -> bool {
        return m_ptr_vector == other.m_ptr_vector;
    }

    auto operator!=(const ptr_span& other) -> bool {
        return m_ptr_vector != other.m_ptr_vector;
    }

    auto operator<(const ptr_span& other) -> bool {
        return m_ptr_vector < other.m_ptr_vector;
    }

    auto operator<=(const ptr_span& other) -> bool {
        return m_ptr_vector <= other.m_ptr_vector;
    }

    auto operator>(const ptr_span& other) -> bool {
        return m_ptr_vector > other.m_ptr_vector;
    }

    auto operator>=(const ptr_span& other) -> bool {
        return m_ptr_vector >= other.m_ptr_vector;
    }

protected:
    auto range_check(size_type pos) const -> void {
        if (pos >= size()) {
            std::stringstream ss;
            ss << "muc::impl::ptr_span::range_check: pos >= size() " << '['
               << pos << " >= " << size() << ']';
            throw std::out_of_range{ss.str()};
        }
    }

protected:
    RawPtrContainer* m_ptr_vector;
};

} // namespace muc
