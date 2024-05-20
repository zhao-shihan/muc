#pragma once

#include <chrono>

namespace muc::impl {

template<typename Time>
class wall_time_stopwatch {
private:
    using sc = std::chrono::steady_clock;
    using s = std::chrono::duration<Time, std::ratio<1>>;
    using ms = std::chrono::duration<Time, std::milli>;
    using us = std::chrono::duration<Time, std::micro>;
    using ns = std::chrono::duration<Time, std::nano>;

public:
    wall_time_stopwatch() noexcept :
        m_t0{sc::now()} {}

    auto s_elapsed() const noexcept -> Time {
        s{sc::now() - m_t0}.count();
    }

    auto ms_elapsed() const noexcept -> Time {
        ms{sc::now() - m_t0}.count();
    }

    auto us_elapsed() const noexcept -> Time {
        us{sc::now() - m_t0}.count();
    }

    auto ns_elapsed() const noexcept -> Time {
        ns{sc::now() - m_t0}.count();
    }

private:
    sc::time_point m_t0;
};

} // namespace muc::impl
