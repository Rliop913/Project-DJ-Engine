# PDJE Verify

Rules and minimum checks for repository verification.

## Agent Rule

- Commands here are examples, not permission.
- Ask before any `conan install`, `cmake`, `cmake --build`, `ctest`, or wrapper.
- Do not choose preset/toolchain/build type/verification without user direction.
- Use only `./build` unless the user requests another build directory.

## Checks

| Kind | Command / source |
| --- | --- |
| source defaults | inspect `cmakes/Options.cmake` for `PDJE_TEST`, `PDJE_DEV_TEST`, `PDJE_DYNAMIC`, `PDJE_SWIG_BUILD`, `PDJE_DEVELOP_INPUT` |
| doc surface | list root Markdown files and `AGENT_DOCS/**/*.md` |
| build cache inspection | inspect `cmake -LA -N ./build` for `PDJE_*` and `CMAKE_BUILD_TYPE` values |
| preset listing | `cmake --list-presets`; `ctest --list-presets` |
| unit listing/run | `ctest --test-dir ./build -N -L unit`; `ctest --test-dir ./build -L unit --output-on-failure` |
| focused tests | [TEST_MAP.md](TEST_MAP.md) |
| platform flows | [skills/pdje-build-verify/references/verification-flows.md](skills/pdje-build-verify/references/verification-flows.md) |

Rows using `cmake` or `ctest` still require approval.

## Preset Shape

| Preset family | Build type | Dynamic | Tests | Dev tests | Compiler lock |
| --- | --- | --- | --- | --- | --- |
| `*-release` | `Release` | `ON` | `OFF` | `OFF` | Linux/macOS=`clang`; Windows=`cl` |
| `*-relwithdebinfo` | `RelWithDebInfo` | `ON` | `ON` | `ON` | Linux/macOS=`clang`; Windows=`cl` |

Windows also requires a prepared MSVC x64 shell; macOS expects AppleClang.

## Success Criteria

- `AGENT_DOCS/` is indexed and no control doc treats `docs/` or
  `BluePrint_PDJE/` as canonical.
- Approved release builds use dynamic libraries with test flags off.
- Approved relwithdebinfo builds use dynamic libraries with test flags on and
  pass `ctest --preset <host>-relwithdebinfo`.
