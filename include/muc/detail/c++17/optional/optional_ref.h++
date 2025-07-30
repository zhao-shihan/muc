// -*- C++ -*-
//
// Copyright 2021-2025  Shihan Zhao
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <functional>
#include <optional>

namespace muc {

/// @brief Alias for an optional reference wrapper
///
/// Represents a potentially absent reference to an object of type T. Combines
/// the behavior of:
///   - `std::optional`: May or may not contain a value
///   - `std::reference_wrapper`: Contains a reference when present
///
/// @tparam T Type of the referenced object
///
/// @par Usage examples:
/// @code
/// int value = 42;
///
/// // Create engaged optional reference
/// optional_ref<int> opt_ref = value;
///
/// // Create empty optional reference
/// optional_ref<int> empty_ref = std::nullopt;
///
/// // Access referenced value
/// if (opt_ref) {
///     opt_ref.value().get() = 43;  // Modifies original 'value'
/// }
/// @endcode
///
/// @warning Storing references requires ensuring the referenced object outlives
/// the optional_ref
/// @see std::optional, std::reference_wrapper
template<typename T>
using optional_ref = std::optional<std::reference_wrapper<T>>;

} // namespace muc
