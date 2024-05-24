#ifndef MUC_TEST_UNIVERSE
#include "muc/ptr_vector"
#else
#include "muc-universe"
#endif

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

template<typename T>
auto print_address(const T& v) -> void {
    for (auto&& e : v.ptr_vector()) {
        std::cout << e.get() << ' ';
    }
    std::cout << "\n";
}

template<template<typename...> typename PtrVector>
auto test_ptr_vector() {
    {
        PtrVector<std::pmr::string> v{"hello", "world", "!"};
        print_vector(v);
    }
    std::cout << '\n';
    {
        // PtrVector<int> v(6, 6);
        PtrVector<float> v(6, 6.);
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.vbegin(), v.vend());
        print_vector(v);
        std::shuffle(v.pbegin(), v.pend(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.vbegin(), v.vend());
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.pbegin(), v.pend(), [](auto&& a, auto&& b) {
            return *a < *b;
        });
        print_vector(v);
        std::shuffle(v.pbegin(), v.pend(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.pbegin(), v.pend(), [](auto&& a, auto&& b) {
            return *a < *b;
        });
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.vrbegin(), v.vrend());
        print_vector(v);
        std::shuffle(v.prbegin(), v.prend(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.vrbegin(), v.vrend());
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        std::sort(v.prbegin(), v.prend(), [](auto&& a, auto&& b) {
            return *a < *b;
        });
        print_vector(v);
        std::shuffle(v.prbegin(), v.prend(), std::minstd_rand{});
        print_vector(v);
        std::stable_sort(v.prbegin(), v.prend(), [](auto&& a, auto&& b) {
            return *a < *b;
        });
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<double> v{1, 2, 3, 4, 5};
        v.insert(v.begin() + 2, 42);
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<double> u{6, 7, 8, 9, 0};
        PtrVector<double> v{1, 2, 3, 4, 5};
        print_vector(u);
        print_address(u);
        print_vector(v);
        print_address(v);
        swap(u, v);
        print_vector(u);
        print_address(u);
        print_vector(v);
        print_address(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> v;
        v = {"zoltraak", "char", "abandon", "cat", "note", "bell"};
        print_vector(v);
        v.erase(v.begin(), v.begin() + 2);
        print_vector(v);
    }
    std::cout << '\n';
}

auto main() -> int {
    std::cout << "================================\n"
                 "muc::pmr::unique_ptr_vector\n"
                 "================================\n";
    test_ptr_vector<muc::pmr::unique_ptr_vector>();
    std::cout << "================================\n"
                 "muc::pmr::shared_ptr_vector\n"
                 "================================\n";
    test_ptr_vector<muc::pmr::shared_ptr_vector>();
}
