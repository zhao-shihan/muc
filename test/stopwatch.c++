#ifndef MUC_TEST_UNIVERSE
#include "muc/time"
#else
#include "muc-universe"
#endif

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
        muc::wall_time_stopwatch wts;
        std::cout << "Sleep 500 ms.\n";
        std::this_thread::sleep_for(500ms);
        std::cout << "Wall time elapsed: " << wts.ns_elapsed() << " ns\n"
                  << "Wall time elapsed: " << wts.us_elapsed() << " us\n"
                  << "Wall time elapsed: " << wts.ms_elapsed() << " ms\n"
                  << "Wall time elapsed: " << wts.s_elapsed() << " s\n";

        std::cout << "Reset wall time stopwatch.\n";
        wts = {};

        std::cout << "Sleep 1 s.\n";
        std::this_thread::sleep_for(1s);
        std::cout << "Wall time elapsed: " << wts.ns_elapsed() << " ns\n"
                  << "Wall time elapsed: " << wts.us_elapsed() << " us\n"
                  << "Wall time elapsed: " << wts.ms_elapsed() << " ms\n"
                  << "Wall time elapsed: " << wts.s_elapsed() << " s\n";
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
        cts = {};

        std::cout << "Do some task.\n";
        task();
        std::cout << "CPU time used: " << cts.ns_used() << " ns\n"
                  << "CPU time used: " << cts.us_used() << " us\n"
                  << "CPU time used: " << cts.ms_used() << " ms\n"
                  << "CPU time used: " << cts.s_used() << " s\n";
    }
}
