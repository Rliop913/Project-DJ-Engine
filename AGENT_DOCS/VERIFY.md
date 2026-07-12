# PDJE Verification

## Permission Boundary

Commands shown here are examples, not permission. Before the first dependency
install, configure, build, CTest run, smoke executable, or wrapper invocation in
a task, ask the user to choose the host preset/configuration and verification
scope. One explicit choice may cover the agreed flow; do not silently broaden
it. Use only repo-root `./build` unless another directory is explicitly
approved.

Static source inspection, Markdown link/path checks, `git diff --check`, and
worktree inspection do not select or mutate a build configuration.

## Sources Of Build Truth

| Question | Inspect |
| --- | --- |
| source option default | `cmakes/Options.cmake` |
| supported preset value and build directory | `CMakePresets.json` |
| dependency bootstrap | `BuildInitwithConan.sh`, `BuildInitwithConan.bat` |
| Windows configure/build wrapper | `windows_conf_and_build.bat` |
| target and source membership | top-level `CMakeLists.txt`, `cmakes/src/`, `cmakes/tests/` |
| what a local tree was configured with | `./build/CMakeCache.txt`, only as local-state evidence |
| CI coverage | `.github/workflows/` |

The checked-in preset workflow requires CMake 3.23 or newer, as declared by
`CMakePresets.json`.

The current CI matrix compiles `*-release` presets. It does not replace an
approved `RelWithDebInfo` unit-test run.

## Preset Matrix

| Family | Build type | Dynamic | Unit/dev tests | Platform compiler |
| --- | --- | --- | --- | --- |
| `windows-release` / `windows-relwithdebinfo` | name-matching | on | off / on | MSVC |
| `linux-release` / `linux-relwithdebinfo` | name-matching | on | off / on | Clang |
| `macos-release` / `macos-relwithdebinfo` | name-matching | on | off / on | AppleClang |

Input and judge are disabled on macOS. Util production code still builds there,
but `pdje_unit_util` is currently excluded on Apple platforms.

## Verification Sequence

After the user approves a flow:

1. Bootstrap dependencies only when the selected build tree does not already
   match the chosen preset. Use the platform commands in
   [verification-flows.md](skills/pdje-build-verify/references/verification-flows.md).
2. Configure with `cmake --preset <host>-<mode>`, or use the approved Windows
   wrapper.
3. Build the narrowest owning target first:
   `cmake --build --preset <host>-relwithdebinfo --target <target>`.
4. Run its stable CTest label from [TEST_MAP.md](TEST_MAP.md).
5. For cross-cutting work, run
   `ctest --preset <host>-relwithdebinfo` after the full build.
6. Build or execute a manual/dev binary only when it is part of the agreed
   acceptance scope. Build success alone is not runtime coverage.

Doctest cases are discovered after their binaries build, so `ctest -N` before
the build is not a complete inventory.

## Reporting

Report:

- the exact approved commands that ran;
- the preset and whether `./build` was reconfigured or bootstrapped;
- targets, CTest labels/regexes, and pass/fail counts;
- manual binaries built versus actually executed;
- platform-gated or unwired coverage that was not exercised.
