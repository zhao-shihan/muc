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

#include "muc/detail/c++17/utility/cpu_relax.h++"
#include "muc/detail/common/inline_macro.h++"

#include <atomic>

namespace muc {

/// @brief A lightweight spin-based mutex for low-latency synchronization.
///        This mutex uses `std::atomic_flag` for efficient spinning.
/// @warning It is NOT fair - thread acquisition order is not guaranteed.
class spin_mutex {
public:
    /// @brief Constructs the mutex in an unlocked state
    constexpr spin_mutex() noexcept = default;

    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator=(const spin_mutex&) = delete;

    /// @brief Acquires the lock through spinning
    /// @details Continuously tries to set the atomic flag until successful.
    ///          Uses architecture-specific pause/yield instructions to
    ///          reduce contention and improve power efficiency.
    MUC_ALWAYS_INLINE auto lock() noexcept -> void {
        while (m_flag.test_and_set(std::memory_order::acquire)) {
            cpu_relax();
        }
    }

    /// @brief Attempts to acquire the lock without blocking
    /// @return true if lock was acquired, false if already locked
    MUC_ALWAYS_INLINE auto try_lock() noexcept -> bool {
        return not m_flag.test_and_set(std::memory_order::acquire);
    }

    /// @brief Releases the lock
    /// @pre Must be called by the current lock owner
    MUC_ALWAYS_INLINE auto unlock() noexcept -> void {
        m_flag.clear(std::memory_order::release);
    }

private:
    std::atomic_flag m_flag; ///< set = locked, clear = unlocked
};

} // namespace muc
