#pragma once

#include <ctime>
#include <sys/time.h>

namespace muc::impl {

template<typename Time>
class cpu_time_stopwatch {
public:
    cpu_time_stopwatch() noexcept :
        m_t0{} {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_t0);
    }

    auto s_used() const noexcept -> Time {
        return ns_used() / 1'000'000'000;
    }

    auto ms_used() const noexcept -> Time {
        return ns_used() / 1'000'000;
    }

    auto us_used() const noexcept -> Time {
        return ns_used() / 1'000;
    }

    auto ns_used() const noexcept -> Time {
        std::timespec t;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
        return static_cast<Time>(t.tv_sec - m_t0.tv_sec) * 1'000'000'000 +
               static_cast<Time>(t.tv_nsec - m_t0.tv_nsec);
    }

private:
    std::timespec m_t0;
};

} // namespace muc::impl
