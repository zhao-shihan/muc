#pragma once

// Linux/BSD implementation:
#if (defined linux or defined __linux__ or defined __linux) or             \
    (defined __DragonFly__ or defined __FreeBSD__ or defined __NetBSD__ or \
     defined __OpenBSD__)
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/linux_bsd_cpu_time_stopwatch.h++"
// Windows implementation:
#elif defined _WIN32
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/windows_cpu_time_stopwatch.h++"
// Fallback implementation:
#else
#include "muc/detail/c++17/time/impl/cpu_time_stopwatch/fallback_cpu_time_stopwatch.h++"
#endif

#include <limits>
#include <type_traits>

namespace muc {

/// @brief CPU stopwatch better than std::clock() when available.
template<typename Time = double>
class cpu_time_stopwatch final : protected impl::cpu_time_stopwatch<Time> {
    static_assert(std::is_floating_point_v<Time>,
                  "the value type for stopwatch should be a floating point");
    static_assert(std::numeric_limits<Time>::digits >=
                      std::numeric_limits<double>::digits,
                  "stopwatch value type should be at least as long as double");

public:
    using value_type = Time;

public:
    auto s_used() const noexcept -> value_type {
        return impl::cpu_time_stopwatch<Time>::s_used();
    }

    auto ms_used() const noexcept -> value_type {
        return impl::cpu_time_stopwatch<Time>::ms_used();
    }

    auto us_used() const noexcept -> value_type {
        return impl::cpu_time_stopwatch<Time>::us_used();
    }

    auto ns_used() const noexcept -> value_type {
        return impl::cpu_time_stopwatch<Time>::ns_used();
    }
};

} // namespace muc
