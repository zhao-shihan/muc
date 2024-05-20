# MUC

MUC is a C++ STL-style header-only library, requires C++17 or higher. Some utilities requires C++20.

# Public headers

- `muc/bit`: bit manipulations.
- `muc/ceta_string` (C++20): includes `muc::ceta_string`, a string type for template parameters.
- `muc/graph`: an simple undirectional weighted/unweighted graph.
- `muc/math` (partially C++20): mathematical functions that are useful or high-performance.
- `muc/memory`: basic memory management and pointer operations.
- `muc/ptr_vector`: include linear sequence containers like `std::vector<T>` but stores as `std::vector<smart-ptr<T>>`. There is also a view `muc::ptr_vector_view` for access `std::vector<smart-ptr<T>>` as `std::vector<T>`.
- `muc/time`: timing and high-resolution stopwatch.
- `muc/type_traits`: template metaprogramming library.
