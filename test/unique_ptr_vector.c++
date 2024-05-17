#include "muc/ptr_vector.h++"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>

template<typename T>
auto print_vector(const T& v) -> void {
    for (auto&& e : v) {
        std::cout << e << ' ';
    }
    std::cout << "\n";
}

auto main() -> int {
    {
        muc::unique_ptr_vector<std::string> v{"hello", "world", "!"};
        print_vector(v);
    }
    std::cout << '\n';
    /* {
        // muc::pmr::unique_ptr_vector<int> v(6, 6);
        muc::pmr::unique_ptr_vector<float> v(6, 6.);
        print_vector(v);
    }
    std::cout << '\n';
    {
        muc::pmr::unique_ptr_vector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.begin(), v.end());
        print_vector(v);
        std::shuffle(v.begin(), v.end(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.begin(), v.end());
        print_vector(v);
    }
    std::cout << '\n';
    {
        muc::pmr::unique_ptr_vector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.rbegin(), v.rend());
        print_vector(v);
        std::shuffle(v.rbegin(), v.rend(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.rbegin(), v.rend());
        print_vector(v);
    }
    std::cout << '\n';
    {
        muc::pmr::unique_ptr_vector<double> v{1, 2, 3, 4, 5};
        v.insert(v.begin() + 2, 42);
        print_vector(v);
    }
    std::cout << '\n'; */
}
