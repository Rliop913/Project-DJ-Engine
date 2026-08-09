# PDJE Verification

## Approval Boundary

Before the first dependency install, configure, build, CTest run, smoke
executable, or wrapper invocation in a task, obtain the user's platform preset,
configuration, and verification scope. One explicit choice may cover the
agreed flow; do not broaden it silently. Use only repo-root `./build` unless the
user approves another build directory.

Static source inspection, Markdown link/path checks, worktree inspection, and
`git diff --check` do not select or mutate a build configuration.

## Build Truth

| Question | Authoritative file |
| --- | --- |
| source option default | `cmakes/Options.cmake` |
| supported preset, compiler, and build directory | `CMakePresets.json` |
| dependency bootstrap | `BuildInitwithConan.sh`, `BuildInitwithConan.bat` |
| Windows wrapper behavior | `windows_conf_and_build.bat` |
| target and source membership | top-level `CMakeLists.txt`, `cmakes/src/`, `cmakes/tests/` |
| CI coverage | `.github/workflows/` |
| existing build-tree state | `./build/CMakeCache.txt`, as local-state evidence only |

`CMakePresets.json` requires CMake 3.23 or newer. Checked-in CI coverage does
not substitute for an approved local test flow.

## Preset Selection

| Need | Preset family | Configuration effect |
| --- | --- | --- |
| compile-only acceptance | `<host>-release` | `Release`, dynamic libraries, unit/dev tests off |
| unit or manual/dev acceptance | `<host>-relwithdebinfo` | `RelWithDebInfo`, dynamic libraries, unit/dev tests on |

Windows uses MSVC, Linux uses Clang, and macOS uses AppleClang. Platform test
and target gates are owned by [TEST_MAP.md](TEST_MAP.md).

After approval, use the repository
[build-verification skill](skills/pdje-build-verify/SKILL.md) for flow selection
and command syntax.

## Reporting

Report the exact commands and preset, whether dependencies or `./build` were
reconfigured, targets and labels exercised, pass/fail results, manual targets
built versus executed, and platform-gated or unwired coverage not exercised.
