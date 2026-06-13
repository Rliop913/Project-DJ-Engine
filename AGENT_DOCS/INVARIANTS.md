# PDJE Invariants

Stable unless the repository is deliberately restructured.

## Documentation

| Path | Rule |
| --- | --- |
| `AGENT_DOCS/` | canonical Markdown control docs |
| `README.md` | public landing page |
| `AGENTS.md` | thin agent entrypoint |
| `DECISIONS.md` | rationale ledger for control-doc choices |
| `docs/` | redirecting HTML only; public docs live outside this checkout |
| `BluePrint_PDJE/` | archive material, not current truth |

## Build Truth

| Item | Rule |
| --- | --- |
| source defaults | `cmakes/Options.cmake`, not local build cache |
| shared matrix | root `CMakePresets.json` |
| build directory | repo-root `./build` only unless user requests otherwise |
| modes | `Release` and `RelWithDebInfo`; `PDJE_DYNAMIC=ON` |
| test flags | only `RelWithDebInfo` enables `PDJE_TEST` and `PDJE_DEV_TEST` |
| compilers | Windows=`cl`; Linux=`clang`; macOS=`clang`/AppleClang |
| agent approval | ask before `conan install`, `cmake`, `cmake --build`, `ctest`, or wrappers |
| optional gates | SWIG only with `PDJE_SWIG_BUILD=ON`; input/judge on Linux/Windows, forced off on macOS |

Agents must not choose preset, toolchain, build type, verification command, or
new build directory without explicit user approval.

## Code Shape

- `include/` contains headers and implementation `.cpp` files.
- Main native surfaces: `PDJE`, `PDJE_Input`, `PDJE_JUDGE::JUDGE`, `PDJE_UTIL`.
- `PDJE_UTIL` is active code.
- Pointer data-line structs are non-owning and require null checks.
- Unit test truth comes from CTest unit labels in repo-root `./build`.
