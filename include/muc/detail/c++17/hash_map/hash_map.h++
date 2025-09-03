// -*- C++ -*-
//
// Copyright (C) 2021-2025  Shihan Zhao
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

#include "muc/detail/external/c++17/parallel_hashmap/phmap.h"

namespace muc {

template<typename K, typename V,
         typename Hash = impl::phmap::priv::hash_default_hash<K>,
         typename Eq = impl::phmap::priv::hash_default_eq<K>,
         typename Alloc = impl::phmap::priv::Allocator<
             impl::phmap::priv::Pair<const K, V>>> // alias for std::allocator
using flat_hash_map = impl::phmap::flat_hash_map<K, V, Hash, Eq, Alloc>;

template<typename Key, typename Value,
         typename Hash = impl::phmap::priv::hash_default_hash<Key>,
         typename Eq = impl::phmap::priv::hash_default_eq<Key>,
         typename Alloc = impl::phmap::priv::Allocator<impl::phmap::priv::Pair<
             const Key, Value>>> // alias for std::allocator
using node_hash_map = impl::phmap::node_hash_map<Key, Value, Hash, Eq, Alloc>;

template<typename K, typename V,
         typename Hash = impl::phmap::priv::hash_default_hash<K>,
         typename Eq = impl::phmap::priv::hash_default_eq<K>,
         typename Alloc = impl::phmap::priv::Allocator<
             impl::phmap::priv::Pair<const K, V>>, // alias for std::allocator
         std::size_t N = 4,                        // 2**N submaps
         typename Mutex =
             impl::phmap::NullMutex> // use std::mutex to enable internal locks
using parallel_flat_hash_map =
    impl::phmap::parallel_flat_hash_map<K, V, Hash, Eq, Alloc, N, Mutex>;

template<typename Key, typename Value,
         typename Hash = impl::phmap::priv::hash_default_hash<Key>,
         typename Eq = impl::phmap::priv::hash_default_eq<Key>,
         typename Alloc = impl::phmap::priv::Allocator<impl::phmap::priv::Pair<
             const Key, Value>>, // alias for std::allocator
         std::size_t N = 4,      // 2**N submaps
         typename Mutex =
             impl::phmap::NullMutex> // use std::mutex to enable internal locks
using parallel_node_hash_map =
    impl::phmap::parallel_node_hash_map<Key, Value, Hash, Eq, Alloc, N, Mutex>;

} // namespace muc
