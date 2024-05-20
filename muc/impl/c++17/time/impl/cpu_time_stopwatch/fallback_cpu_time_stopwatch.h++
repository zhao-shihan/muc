#pragma once

#include <ctime>

namespace muc::impl {

template<typename Time>
class cpu_time_stopwatch {
public:
    cpu_time_stopwatch() noexcept :
        m_t0{std::clock()} {}

    auto s_used() const noexcept -> Time {
        static_cast<Time>(std::clock() - m_t0) / CLOCKS_PER_SEC;
    }

    auto ms_used() const noexcept -> Time {
        return s_used() * 1'000;
    }

    auto us_used() const noexcept -> Time {
        return s_used() * 1'000'000;
    }

    auto ns_used() const noexcept -> Time {
        return s_used() * 1'000'000'000;
    }

private:
    std::clock_t m_t0;
};

} // namespace muc::impl
