#include "muc/ceta_string.h++"

#include <iostream>

template<muc::ceta_string... S>
auto print() -> void {
    (std::cout << ... << S.c_str()) << '\n';
    (std::cout << ... << S.sv()) << '\n';
    (std::cout << ... << S.s()) << '\n';
}

auto main() -> int {
    print<"hello", ", ", "world!">();
    // print<0>();
}
