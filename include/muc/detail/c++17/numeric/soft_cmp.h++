// -*- C++ -*-
//
// Copyright 2021-2025  Shihan Zhao
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "muc/detail/c++17/math/sigmoid.h++"

#include <algorithm>
#include <type_traits>

namespace muc {

/// @brief A class to perform soft comparisons between floating-point values.
///
/// This class uses a softening factor to create a non-binary comparison
/// between two floating-point values, allowing for smooth transitions. The
/// comparisons are based on sigmoid functions to provide soft decision
/// boundaries.
///
/// @tparam T The type of the floating-point values. It should be a
/// floating-point type.
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
class soft_cmp {
private:
    class value;

    /// @brief Represents the result of a soft comparison operation.
    class result {
        friend class value;

    private:
        constexpr result(T result) :
            m_result{result} {}

    public:
        /// @brief Converts the result to the underlying type T.
        constexpr operator T() const {
            return m_result;
        }

        /// @brief Dereference operator to access the underlying result.
        /// @return The underlying result value.
        constexpr auto operator*() const -> T {
            return m_result;
        }

        /// @brief Negation operator to invert the result.
        /// @return A new result object representing 1 - m_result.
        constexpr auto operator not() const -> result {
            return 1 - m_result;
        }

        /// @brief Logical AND operation between two soft comparison results.
        /// @param other Another result to compare against.
        /// @return The result of the logical AND operation.
        constexpr auto operator and(result other) const -> result {
            return m_result * other.m_result;
        }

        /// @brief Logical OR operation between two soft comparison results.
        /// @param other Another result to compare against.
        /// @return The result of the logical OR operation.
        constexpr auto operator or(result other) const -> result {
            return std::min(m_result + other.m_result, static_cast<T>(1));
        }

    private:
        T m_result;
    };

    /// @brief Represents a value with an associated softening factor.
    class value {
        friend class soft_cmp;

    private:
        constexpr value(T value, T soft) :
            m_value{value},
            m_soft{soft} {}

    public:
        /// @brief Greater-than operator comparing two soft values.
        /// @param other The other value to compare against.
        /// @return The result of the soft comparison.
        auto operator>(value other) const -> result {
            return muc::sigmoid((m_value - other.m_value) / m_soft);
        }

        /// @brief Less-than operator comparing two soft values.
        /// @param other The other value to compare against.
        /// @return The result of the soft comparison.
        auto operator<(value other) const -> result {
            return muc::sigmoid((other.m_value - m_value) / m_soft);
        }

        /// @brief Equality operator comparing two soft values.
        /// @param other The other value to compare against.
        /// @return The result of the equality comparison.
        auto operator==(value other) const -> result {
            const auto x{muc::sigmoid((m_value - other.m_value) / m_soft)};
            return 4 * x * (1 - x);
        }

        /// @brief Inequality operator comparing two soft values.
        /// @param other The other value to compare against.
        /// @return The result of the inequality comparison.
        auto operator!=(value other) const -> result {
            const auto x{muc::sigmoid((m_value - other.m_value) / m_soft)};
            return 1 - 4 * x * (1 - x);
        }

        /// @brief Greater than or equal is same as greater-than.
        auto operator>=(value) const -> result = delete;
        /// @brief Less than or equal is same as less-than.
        auto operator<=(value) const -> result = delete;
#if __cplusplus >= 202002L
        /// @brief Three-way comparison is not well-defined.
        auto operator<=>(value) const -> result = delete;
#endif

    private:
        T m_value;
        T m_soft;
    };

public:
    /// @brief Constructs a soft_cmp object with a given softening factor.
    /// @param soft The softening factor to use in comparisons.
    constexpr explicit soft_cmp(T soft) :
        m_soft{soft} {}

    /// @brief Sets a new softening factor for the comparisons.
    /// @param soft The new softening factor.
    constexpr auto soft(T soft) -> void {
        m_soft = soft;
    }

    /// @brief Gets the current softening factor.
    /// @return The current softening factor.
    constexpr auto soft() const -> T {
        return m_soft;
    }

    /// @brief Creates a value for comparison using the current softening
    /// parameter.
    /// @param val The value to be wrapped with the current softening factor.
    /// @return A value object containing the given value and the softening
    /// parameter.
    constexpr auto operator()(T val) const -> value {
        return {val, m_soft};
    }

private:
    T m_soft;
};

} // namespace muc
