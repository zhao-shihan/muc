#include "muc/ptr_span"
#include "muc/ptr_vector"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <utility>

template<typename T>
auto print_vector(const T& v) -> void {
    for (auto&& e : v) {
        std::cout << e << ' ';
    }
    std::cout << "\n";
}

template<typename T>
auto print_address(const T& v) -> void {
    for (auto&& e : v.underlying()) {
        std::cout << e.get() << ' ';
    }
    std::cout << "\n";
}

template<template<typename...> typename PtrVector>
auto test_ptr_vector() {
    {
        PtrVector<std::pmr::string> o;
        o.emplace_back("hello");
        o.emplace_back("world");
        o.emplace_back("!");
        muc::ptr_span v{std::as_const(o.underlying())};
        print_vector(v);
    }
    std::cout << '\n';
    {
        PtrVector<std::pmr::string> o;
        o.emplace_back("zoltraak");
        o.emplace_back("char");
        o.emplace_back("abandon");
        o.emplace_back("cat");
        o.emplace_back("note");
        o.emplace_back("bell");
        muc::ptr_span v{o.underlying()};
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
        PtrVector<std::pmr::string> o;
        o.emplace_back("zoltraak");
        o.emplace_back("char");
        o.emplace_back("abandon");
        o.emplace_back("cat");
        o.emplace_back("note");
        o.emplace_back("bell");
        muc::ptr_span v{o.underlying()};
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
        PtrVector<std::pmr::string> o;
        o.emplace_back("zoltraak");
        o.emplace_back("char");
        o.emplace_back("abandon");
        o.emplace_back("cat");
        o.emplace_back("note");
        o.emplace_back("bell");
        muc::ptr_span v{o.underlying()};
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
        PtrVector<std::pmr::string> o;
        o.emplace_back("zoltraak");
        o.emplace_back("char");
        o.emplace_back("abandon");
        o.emplace_back("cat");
        o.emplace_back("note");
        o.emplace_back("bell");
        muc::ptr_span v{o.underlying()};
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
        PtrVector<double> p;
        p.emplace_back(3);
        p.emplace_back(4);
        p.emplace_back(5);
        PtrVector<double> o;
        o.emplace_back(1);
        o.emplace_back(2);
        o.emplace_back(3);
        muc::ptr_span u{std::as_const(p.underlying())};
        muc::ptr_span v{std::as_const(o.underlying())};
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
        PtrVector<std::pmr::string> o;
        o.emplace_back("zoltraak");
        o.emplace_back("char");
        o.emplace_back("abandon");
        o.emplace_back("cat");
        o.emplace_back("note");
        o.emplace_back("bell");
        muc::ptr_span v{o.underlying()};
        print_vector(v);
        v.erase(v.begin(), v.begin() + 2);
        print_vector(v);
    }
    std::cout << '\n';
    {
        std::vector<std::pmr::string*> raw;
        raw.emplace_back(new std::pmr::string{"zoltraak"});
        raw.emplace_back(new std::pmr::string{"char"});
        raw.emplace_back(new std::pmr::string{"abandon"});
        raw.emplace_back(new std::pmr::string{"cat"});
        raw.emplace_back(new std::pmr::string{"note"});
        raw.emplace_back(new std::pmr::string{"bell"});
        PtrVector<std::pmr::string> o{raw};
        muc::ptr_span v{o.underlying()};
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
