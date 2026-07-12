# PDJE Invariants

These rules are stable until the repository is deliberately restructured.

## Truth And Scope

- Current source and checked-in build files outrank all prose.
- `AGENT_DOCS/` is the canonical agent-facing Markdown surface; `AGENTS.md` is
  only its root entrypoint.
- `README.md` is public-facing. `docs/` only redirects to external docs, and
  `BluePrint_PDJE/` is archive-only.
- Preserve unrelated user changes and do not expand a requested subsystem
  scope without approval.

## Build

- Source option defaults live in `cmakes/Options.cmake`.
- The supported shared preset matrix lives in `CMakePresets.json`; every
  checked-in preset uses Ninja, repo-root `./build`, and `PDJE_DYNAMIC=ON`.
- `*-release` presets use `Release` with `PDJE_TEST=OFF` and
  `PDJE_DEV_TEST=OFF`.
- `*-relwithdebinfo` presets use `RelWithDebInfo` with `PDJE_TEST=ON` and
  `PDJE_DEV_TEST=ON`.
- Presets select MSVC on Windows and Clang/AppleClang on Linux/macOS.
- `PDJE_DEVELOP_INPUT` is available on Linux and Windows and is forced off on
  macOS. SWIG targets exist only when `PDJE_SWIG_BUILD=ON`.
- A local CMake cache is evidence about that build tree, never a source default.

Before the first dependency install, configure, build, test execution, smoke
binary, or wrapper command in a task, obtain the user's platform preset,
configuration, and verification scope. Use only `./build` unless the user
explicitly approves another directory.

## Code And Wiring

- The project uses C++20.
- `include/` contains public headers, internal headers, and implementation
  `.cpp` files.
- Production and unit-test sources are explicitly listed under `cmakes/src/`
  and `cmakes/tests/units/`; adding a file does not automatically compile it.
- Native public surfaces are `PDJE`, `PDJE_Input`, `PDJE_JUDGE::JUDGE`, and
  `PDJE_UTIL`. C ABI contracts are declared in the corresponding `CPDJE*.h`
  headers, not their implementation `.cpp` files.
- Core/input data lines are non-owning pointer views. Consumers must validate
  required members and must not outlive the owning runtime.
- `.clang-format` is the formatting authority for C/C++ sources.
