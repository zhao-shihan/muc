#include "muc/chrono"
#include "muc/time"

#include <iostream>
#include <random>
#include <thread>

auto task() -> void {
    std::mt19937_64 mt;
    volatile std::mt19937_64::result_type result;
    for (int i{}; i < 100'000'000; ++i) {
        result = mt();
    }
    std::cout << "Done (" << result << ").\n";
}

auto main() -> int {
    using namespace std::chrono_literals;

    {
        muc::chrono::stopwatch wts;
        std::cout << "Sleep 500 ms.\n";
        std::this_thread::sleep_for(500ms);
        std::cout << "Wall time elapsed: " << wts.read().count() << " ns\n";

        std::cout << "Reset wall time stopwatch.\n";
        wts.reset();

        std::cout << "Sleep 1 s.\n";
        std::this_thread::sleep_for(1s);
        std::cout << "Wall time elapsed: " << wts.read().count() << " ns\n";
    }
    {
        muc::cpu_time_stopwatch cts;
        std::cout << "Do some task.\n";
        task();
        std::cout << "CPU time used: " << cts.ns_used() << " ns\n"
                  << "CPU time used: " << cts.us_used() << " us\n"
                  << "CPU time used: " << cts.ms_used() << " ms\n"
                  << "CPU time used: " << cts.s_used() << " s\n";

        std::cout << "Reset CPU time stopwatch.\n";
        cts.reset();

        std::cout << "Do some task.\n";
        task();
        std::cout << "CPU time used: " << cts.ns_used() << " ns\n"
                  << "CPU time used: " << cts.us_used() << " us\n"
                  << "CPU time used: " << cts.ms_used() << " ms\n"
                  << "CPU time used: " << cts.s_used() << " s\n";
    }
}
