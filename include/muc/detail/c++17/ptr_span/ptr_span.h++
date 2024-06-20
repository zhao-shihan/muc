#pragma once

#include "muc/detail/c++17/memory/to_address.h++"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace muc {

template<typename T,
         typename =
             std::void_t<decltype(*std::declval<typename T::value_type>())>>
class ptr_span {
private:
    using raw_ptr_vector = T;

public:
    using value_type = typename std::pointer_traits<
        typename raw_ptr_vector::value_type>::element_type;
    using size_type = typename raw_ptr_vector::size_type;
    using difference_type = typename raw_ptr_vector::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;             // TODO: Do we need traits?
    using const_pointer = const value_type*; // TODO: Do we need traits?

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

    public:
        basic_iterator() :
            m_iter{} {}

    private:
        explicit basic_iterator(RawIterator iter) :
            m_iter{iter} {}

    public:
        operator basic_iterator<typename raw_ptr_vector::const_iterator>() {
            return basic_iterator<typename raw_ptr_vector::const_iterator>{
                m_iter};
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

public:
    using iterator = basic_iterator<typename raw_ptr_vector::iterator>;
    using const_iterator =
        basic_iterator<typename raw_ptr_vector::const_iterator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    ptr_span() noexcept :
        m_ptr_vector{} {}

    ptr_span(T& ptr_vector) noexcept :
        m_ptr_vector{&ptr_vector} {}

    ptr_span(T&& ptr_vector) = delete;

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

    auto underlying() -> raw_ptr_vector& {
        return *m_ptr_vector;
    }

    auto underlying() const -> const raw_ptr_vector& {
        return *m_ptr_vector;
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
        raw_ptr_vector* m_ptr_vector;
    };

    auto vrange() const noexcept -> vrange_type {
        return m_ptr_vector;
    }

    //
    // Capacity
    //

    [[nodiscard]] auto empty() const noexcept -> bool {
        return m_ptr_vector ? m_ptr_vector->empty() : true;
    }

    auto size() const noexcept -> size_type {
        return m_ptr_vector ? m_ptr_vector->size() : 0;
    }

    auto max_size() const noexcept -> size_type {
        return m_ptr_vector->max_size();
    }

    auto reserve(size_type new_capacity) -> void {
        return m_ptr_vector->reserve(new_capacity);
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

    auto erase(const_iterator pos) -> iterator {
        return iterator{m_ptr_vector->erase(pos.m_iter)};
    }

    auto erase(const_iterator first, const_iterator last) -> iterator {
        return iterator{m_ptr_vector->erase(first.m_iter, last.m_iter)};
    }

    auto pop_back() -> void {
        m_ptr_vector->pop_back();
    }

    auto swap(ptr_span& other) noexcept -> void {
        std::swap(m_ptr_vector, other.m_ptr_vector);
    }

    //
    // Compare
    //

    auto operator==(const ptr_span& other) const -> bool {
        return *m_ptr_vector == *other.m_ptr_vector;
    }
#if __cplusplus >= 202002L
    auto operator<=>(const ptr_span& other) const -> auto {
        return *m_ptr_vector <=> *other.m_ptr_vector;
    }
#else
    auto operator!=(const ptr_span& other) const -> bool {
        return *m_ptr_vector != *other.m_ptr_vector;
    }

    auto operator<(const ptr_span& other) const -> bool {
        return *m_ptr_vector < *other.m_ptr_vector;
    }

    auto operator<=(const ptr_span& other) const -> bool {
        return *m_ptr_vector <= *other.m_ptr_vector;
    }

    auto operator>(const ptr_span& other) const -> bool {
        return *m_ptr_vector > *other.m_ptr_vector;
    }

    auto operator>=(const ptr_span& other) const -> bool {
        return *m_ptr_vector >= *other.m_ptr_vector;
    }
#endif

private:
    auto range_check(size_type pos) const -> void {
        if (pos >= size()) {
            std::stringstream ss;
            ss << "muc::ptr_span::range_check: pos >= size() [" << pos
               << " >= " << size() << ']';
            throw std::out_of_range{ss.str()};
        }
    }

private:
    raw_ptr_vector* m_ptr_vector;
};

template<typename T>
auto swap(ptr_span<T>& lhs,
          ptr_span<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
    lhs.swap(rhs);
}

} // namespace muc
