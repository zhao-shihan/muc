# AGENTS.md

Instructions for AI coding agents working in the MUC repository.

## Ask the user about every decision — no exceptions

This repository is owned and maintained by one human. Agents must not make assumptions: **every
decision point is raised with the user before you act**. Do not pick a default, do not do "the
obviously reasonable thing", and do not implement a guess so the user "has something to review".
There is no penalty for asking too many questions — under-asking is the failure mode here, not
over-asking. If you are unsure whether something is a decision point, it is: ask. Ask well: state
the decision, offer 2–4 concrete options with trade-offs, mark your recommendation, then wait.

Ask before (non-exhaustive):

- public API design: naming, signature, semantics, `constexpr`/`noexcept`, value categories
- adding, removing or renaming a public header or public symbol, and any breaking change or
  deprecation
- implementation trade-offs (algorithm choice, performance vs. readability, reuse of existing
  code) and large refactors, cross-file renames, file deletions
- version bumps and anything release-related
- new third-party dependencies, or raising/lowering the required C++ standard
- build system, warning flags, or the test and verification strategy (`CMakeLists.txt`,
  `test/CMakeLists.txt`)
- unclear or incomplete requirements, or code contradicting documentation
- **any git write operation**: `commit`, `push`, `branch`, `tag`, `reset`, `rebase`, `stash`, …

## Project overview

MUC is a header-only, STL-style C++17-and-later library, distributed as the CMake INTERFACE target
`muc::muc`. There is no compiled artifact and no third-party dependency.

- `include/muc/<name>` — public headers, no extension; users write `#include "muc/<name>"`.
- `include/muc/detail/c++17/<name>/`, `detail/c++20/<name>/` — the real implementations (`.h++`),
  split by the standard they need; `detail/common/` holds shared macros.
- `test/<name>.c++` — one test source per public header, built for every standard it supports
  (`ceta_string` is C++20-and-later only).
- `README.md` — the hand-maintained public-header list; `.clang-format` is authoritative; `build/`
  is a local, gitignored build tree that is never committed.

## Build, test and verification gate

Generic commands, choosing generator and compiler for your environment:

```sh
cmake -S . -B build -DMUC_TEST=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

Reuse an existing `build/` tree when it works (locally: Ninja, clang++, Debug,
UndefinedBehaviorSanitizer); create a separate directory only if it is unusable, and report the
directory and exact commands you used.

`test/CMakeLists.txt` registers no `add_test()`, so `ctest` reports zero tests — running the
binaries is the real check:

```sh
build/test/{find_root,math,random,stopwatch,type_traits}_cxx_std_{17,20,23}
build/test/ceta_string_cxx_std_{20,23}
```

All 17 must exit 0. Enumerate by name as above rather than globbing `build/test/*`, which also
holds stale binaries from removed modules; a full run takes about half a minute.

Building every target and running all 17 executables to a zero exit status is required before
reporting a task as done. A compile-only check, a partial build, or "the change is trivial" is not
enough. If a build or test fails, report the exact failure instead of calling the change complete,
and never claim a verification you did not run.

## Code style and formatting

- Run `clang-format -i` (repo `.clang-format`, LLVM base) on every file you touch; do not
  hand-format. Do not reformat files you did not otherwise change, and ask before making a
  formatting-only commit (`Format source`) or editing `.clang-format` itself.
- The configuration is mechanical, so let it decide rather than arguing style: 80-column limit,
  4-space indent, no tabs, pointers and references aligned left (`int* p`, `const T& x`), `{...}`
  braced lists, no namespace indentation, a blank line before each access specifier, and a
  `} // namespace …` comment closing every namespace.
- Include order inside a header: project includes (`"muc/..."`), blank line, then standard library
  includes (`<...>`); alphabetical within each group.
- A public header includes `detail` headers in descending-standard blocks — the
  `#if __cplusplus >= 202002L` block, then `#if __cplusplus >= 201703L` — alphabetical inside each
  one, each closed by its own `#endif` before the include guard's `#endif`.

## Naming and C++ idioms

- Everything lives in `namespace muc`, in a module sub-namespace where one exists (`muc::chrono`),
  with private helpers in a nested `impl` namespace; nothing under `detail/` is public API.
- Qualify names the way the standard library does. Inside `namespace muc` and its nested
  namespaces, refer to muc entities without the `muc::` prefix, using the shortest path that
  reaches the name (`impl::x` from `muc`, plain `x` from `impl`). Keep the prefix in two cases
  only: the unqualified name would be hidden by a member or local declaration (as in
  `tolerance::at`, where the data member `abs` hides `muc::abs`), or the reference is a call in a
  template to a muc function whose name also exists in `std` (`abs`, `bit_cast`, `div`, `imaxabs`,
  `imaxdiv`, `isnan`, `lerp`, `llround`, `pow`, `to_address`), where the prefix is a deliberate ADL
  barrier: without it a same-named function found by argument-dependent lookup in the argument
  type's namespace can be selected silently. Outside the namespace — the `MUC_STATIC_TEST` blocks,
  `test/` sources, examples — always qualify as `muc::x`. Namespace closers such as
  `} // namespace muc::chrono::impl` keep the fully qualified namespace name.
- Documentation comments use bare names in prose and the qualified `muc::x`/`std::y` in `@see` and
  other cross-references; qualify both when contrasting muc with `std`.
- `snake_case` for every public name: types (`stopwatch`, `ceta_string`, `optional_ref`), functions
  (`find_root`, `soft_cmp`, `try_demangle`), concepts (`arithmetic`, `linearly_combinable`) and
  variables alike.
- Template parameters stay short: `T`, `U`, `N`, `F`, `Alloc`, `Args...`.
- Traits read as `is_...`, `remove_...`, `type_...` and each one provides a `_v` variable template
  (`is_bounded_array_v`) built on the `std::bool_constant` family.
- Data members carry an `m_` prefix (`m_impl`, `m_value`); internal aliases get a `_t` suffix
  (`hold_t`, `alloc_t`), while public aliases read like the standard library (`duration`,
  `allocator_type`).
- Accessors take no `get_` prefix (`read()`, `value()`, `c_str()`); predicates are `is_...`/`has_...`.
- `class` for stateful types with an invariant (`soft_cmp`, `muc::chrono::stopwatch`), `struct` for
  traits, aggregates and small value types (`ceta_string`, `is_bounded_array`); a blank line
  separates an access specifier from the preceding block.
- Trailing return types everywhere (`auto f() -> T`), brace initialization (`T k{1};`,
  `inline constexpr bool x_v{...}`), the alternative tokens `not`/`and`/`or`, and no space after
  `template`.
- State intent explicitly: `constexpr`/`consteval` wherever possible, `noexcept` where genuinely
  guaranteed, `explicit` where an implicit conversion would surprise, `[[maybe_unused]]` on
  conditionally used entities, and `[[nodiscard]]` where dropping the result is a bug (as in
  `muc::rotl`).
- Use `MUC_ALWAYS_INLINE` from `detail/common/inline_macro.h++` for small hot functions instead of
  a bare compiler attribute.
- Avoid macros. An unavoidable one is `MUC_`-prefixed `SCREAMING_SNAKE_CASE` (`MUC_ALWAYS_INLINE`),
  and a standard-library backport defines a per-header sentinel such as `MUC_CPP_LIB_UNREACHABLE`;
  detect compiler and language support with `__cplusplus`/`__has_cpp_attribute` instead of assuming.
- Prefer standard-library facilities over hand-rolled equivalents; keep helpers in `impl`, and hoist
  what several headers need into `detail/common/`.

## Header conventions

- Every header under `include/` carries the MIT license block (`Copyright (C) 2021-2026 Shihan
  Zhao`), as do `CMakeLists.txt` and `include/CMakeLists.txt`; `test/` files do not.
- Public headers start with `// -*- C++ -*-`, then `#pragma once` plus a `MUC_<NAME>_<64-hex-suffix>`
  include guard; reuse the suffix of a neighbouring public header. `detail` headers use
  `#pragma once` alone, and everything under `detail/` is implementation detail, not public API.
- A new public header must also be listed in `README.md` — ask first, since it is public API.

## C++ standard compatibility

- C++17 is the baseline: everything in `detail/c++17/` must compile as C++17.
- C++20-only code lives in `detail/c++20/`, behind `#if __cplusplus >= 202002L`.
- Newer features used inside an older-standard directory must be guarded, e.g.
  `#if __cplusplus >= 202302L and __has_cpp_attribute(assume)`.
- Never make an existing C++17-visible symbol require C++20, and never raise the required standard
  or `target_compile_features` without asking.

## Testing requirements

- Tests for templates and `constexpr` code live in the header itself: a trailing
  `#ifdef MUC_STATIC_TEST … #endif` block after `} // namespace muc`, with the includes it needs and
  a `static_assert(...)` per case — see `muc/detail/c++17/type_traits/is_linearly_combinable.h++`.
- New or changed utilities need matching `static_assert` coverage; extend `test/<name>.c++` only
  when runtime (non-`constexpr`) behaviour must be exercised.
- Test sources stay minimal and standard-library only, must compile under every standard they are
  built for, and must keep the build warning-clean (`-Wall -Wextra` / `-WCL4` / `/W4`).

## API design and documentation

- Model names and semantics on the standard library; `muc::` extensions should read as if they
  belonged in `std::`. These choices always go to the user.
- Document new public utilities with Doxygen comments (`/// @brief`, `@tparam`, `@param`,
  `@return`, plus `@note`/`@warning`/`@see`); if a file you touch is still undocumented, ask whether
  to document it in this change.

## Versioning and commits

`project(muc VERSION …)` in `CMakeLists.txt` holds the version. It appears to follow a date-based
scheme `0.<YY>.<M><DD>` (e.g. `0.26.707` = 2026-07-07), but **this is inferred from history —
confirm the rule and the target value with the user every time**. A bump is its own commit,
`Bump version to 0.26.707`; never bump on your own initiative.

- Commit messages: English, imperative mood, capitalized first word, no trailing period, subject
  ≤ 72 characters, following the existing patterns `Add …`, `Fix …`, `Remove …`, `Refactor …`,
  `Update …`, `Format source`, `Bump version to …`; one logical change per commit.
- Committing is a git write operation: propose the message and wait for approval before running
  `git commit` — same for pushing, branching, tagging and any other history change.
