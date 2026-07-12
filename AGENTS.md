# AGENTS.md - PDJE Agent Entry

Project-DJ-Engine (PDJE) is a C++20 DJ/rhythm-game engine. The native engine,
input capture, judge runtime, utility library, C ABI, and optional language
bindings are built from this repository.

## Read Order

1. [AGENT_DOCS/INDEX.md](AGENT_DOCS/INDEX.md)
2. [AGENT_DOCS/INVARIANTS.md](AGENT_DOCS/INVARIANTS.md)
3. The task route in [AGENT_DOCS/CHANGE_MAP.md](AGENT_DOCS/CHANGE_MAP.md)
4. [AGENT_DOCS/RUNTIME_CONTRACTS.md](AGENT_DOCS/RUNTIME_CONTRACTS.md) when
   changing a public API, lifecycle, data line, input state, or judge behavior
5. [AGENT_DOCS/VERIFY.md](AGENT_DOCS/VERIFY.md) before verification

## Non-Negotiable Rules

- Current code and checked-in CMake files outrank prose when they disagree.
- `AGENT_DOCS/` is the canonical agent-facing Markdown surface.
- `docs/` is an external-site redirect; `BluePrint_PDJE/` is archive material.
- Source defaults come from `cmakes/Options.cmake`; preset values come from
  `CMakePresets.json`. Do not infer either from a local cache.
- `include/` contains both headers and implementation `.cpp` files. Adding a
  source file normally also requires updating an explicit list under `cmakes/`.
- Preserve unrelated worktree changes and keep edits within the requested
  subsystem.
- Before the first `conan install`, CMake configure/build, CTest run, executable
  smoke test, or wrapper-script invocation in a task, ask the user which
  platform preset, configuration, and verification scope to use.
- Use only repo-root `./build` unless the user explicitly approves another
  build directory.

## Primary Interfaces

| Surface | C++ | C ABI |
| --- | --- | --- |
| Core | `include/core/interface/PDJE_interface.hpp` | `include/core/interface/CPDJE_interface.h` |
| Input | `include/input/PDJE_Input.hpp` | `include/input/CPDJE_Input.h` |
| Judge | `include/judge/PDJE_Judge.hpp` | `include/judge/CPDJE_Judge.h` |
| Utility | `include/util/PDJE_Util.hpp` plus public leaf headers | none |
