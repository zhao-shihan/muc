#include "muc/chrono"

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
        muc::chrono::stopwatch sw;
        std::cout << "Sleep 500 ms.\n";
        std::this_thread::sleep_for(500ms);
        std::cout << "Wall time elapsed: " << sw.read().count() << " ns\n";

        std::cout << "Reset wall time stopwatch.\n";
        sw.reset();

        std::cout << "Sleep 1 s.\n";
        std::this_thread::sleep_for(1s);
        std::cout << "Wall time elapsed: " << sw.read().count() << " ns\n";
    }
    {
        muc::chrono::processor_stopwatch psw;
        std::cout << "Do some task.\n";
        task();
        std::cout << "CPU time used: " << psw.read().count() << " ns\n";

        std::cout << "Reset CPU time stopwatch.\n";
        psw.reset();

        std::cout << "Do some task.\n";
        task();
        std::cout << "CPU time used: " << psw.read().count() << " ns\n";
    }
}
