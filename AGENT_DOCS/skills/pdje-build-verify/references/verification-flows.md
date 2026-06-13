# PDJE Build And Verification Flows

Platform command shapes for root `CMakePresets.json` and repo-root `./build`.
Baseline docs: `AGENT_DOCS/VERIFY.md`, `TEST_MAP.md`, `CHANGE_MAP.md`.

## Compile-Only

Use `Release` for dynamic compile checks without unit/dev tests.

Linux:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset linux-release
cmake --build --preset linux-release
```

macOS:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset macos-release
cmake --build --preset macos-release
```

Windows:

```cmd
BuildInitwithConan.bat . dynamic Release
call .\windows_conf_and_build.bat Release 16 on
```

## Unit And Dev Verification

Use `RelWithDebInfo` when unit tests or `PDJE_DEV_TEST` executables are needed.

Linux:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset linux-relwithdebinfo
cmake --build --preset linux-relwithdebinfo
```

macOS:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset macos-relwithdebinfo
cmake --build --preset macos-relwithdebinfo
```

Windows:

```cmd
BuildInitwithConan.bat . dynamic RelWithDebInfo
call .\windows_conf_and_build.bat RelWithDebInfo 16 on
```

After the build, choose the focused or full run from `AGENT_DOCS/TEST_MAP.md`.

## Targeted Builds

- Build the narrowest owning target first when the target is known.
- Use `<host>-relwithdebinfo` for unit targets and dev/manual executables.
- Treat dev/manual executable build success as compile coverage only unless the
  executable also runs.

## Selection Notes

- Reuse existing `./build` for incremental verification.
- Fresh trees need a compiler shell and Conan bootstrap that match the preset.
- Sandboxed Windows builds can fail in `ZERO_CHECK` or MSBuild file tracking;
  rerun the same command with escalation instead of changing verification story.
- `windows_conf_and_build.bat` keeps Conan/MSVC and preset configure/build in
  one `cmd`; arg 3 toggles `--fresh` and defaults to `off`.
- Linux/macOS presets lock `clang` / `clang++`; bootstrap must match.

## Reporting Notes

- Report exact build and test commands run.
- Report whether `./build` was reconfigured.
- Report whether a target was only built or also executed.
