# PDJE Verify

This is the thin verification doc for the repository and its control-doc set.

## Agent Rule

The commands in this file are examples, not blanket permission for agents to
run them automatically.

- Before any `conan install`, `cmake` configure, `cmake --build`, `ctest`, or
  helper-script invocation, agents must ask the user which build/config or
  verification flow to use.
- Agents must not pick a preset, toolchain, build type, or verification
  command on their own.
- Agents must not create or use build directories other than `./build` unless
  the user explicitly requests an exception.
- Build/config/verification commands require explicit user approval every time;
  do not start them speculatively.

## Quick Verify Existing Checkout

Check source defaults:

```bash
rg -n "option\\(PDJE_(TEST|DEV_TEST|DYNAMIC|SWIG_BUILD|DEVELOP_INPUT)" cmakes/Options.cmake
```

Inspect the local build cache separately:

```bash
cmake -LA -N ./build | rg '^PDJE_|^CMAKE_BUILD_TYPE'
```

Enumerate unit tests:

```bash
ctest --test-dir ./build -N -L unit
```

Run the unit-label suite:

```bash
ctest --test-dir ./build -L unit --output-on-failure
```

For area-specific commands, use [TEST_MAP.md](TEST_MAP.md).

List the shared presets for the current host:

```bash
cmake --list-presets
ctest --list-presets
```

Check the doc surface:

```bash
find AGENT_DOCS -maxdepth 1 -name '*.md' | sort
find . -maxdepth 1 -type f -name '*.md' | sort
rg -n "CORE_STYLE\\.md|PROJECT_SKILLS\\.md|pdje-build-verify" AGENT_DOCS/INDEX.md AGENT_DOCS/VERIFY.md AGENT_DOCS/PROJECT_SKILLS.md
```

## Clean Verify

Use the checked-in `CMakePresets.json` matrix with repo-root `/build`.

- `*-release`: `CMAKE_BUILD_TYPE=Release`, `PDJE_DYNAMIC=ON`,
  `PDJE_TEST=OFF`, `PDJE_DEV_TEST=OFF`
- `*-relwithdebinfo`: `CMAKE_BUILD_TYPE=RelWithDebInfo`, `PDJE_DYNAMIC=ON`,
  `PDJE_TEST=ON`, `PDJE_DEV_TEST=ON`
- Linux presets fix `clang` / `clang++`
- macOS presets fix `clang` / `clang++` and expect AppleClang
- Windows presets fix `cl` / `cl` and still require a prepared MSVC x64 shell
- Conan bootstrap must match the preset platform, compiler, and build type

Linux:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset linux-release
cmake --build --preset linux-release

CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset linux-relwithdebinfo
cmake --build --preset linux-relwithdebinfo
ctest --preset linux-relwithdebinfo
```

macOS:

```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset macos-release
cmake --build --preset macos-release

CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset macos-relwithdebinfo
cmake --build --preset macos-relwithdebinfo
ctest --preset macos-relwithdebinfo
```

Windows:

```bat
BuildInitwithConan.bat . dynamic Release
call .\windows_conf_and_build.bat Release 16 on

BuildInitwithConan.bat . dynamic RelWithDebInfo
call .\windows_conf_and_build.bat RelWithDebInfo 16 on
ctest --preset windows-relwithdebinfo
```

`windows_conf_and_build.bat` is the repository wrapper for Windows preset
configure+build runs. Use
`call .\windows_conf_and_build.bat Release <jobs> <on|off>` or
`call .\windows_conf_and_build.bat RelWithDebInfo <jobs> <on|off>` from `cmd.exe`, a
Developer Prompt, or another batch file. The helper wraps `conanvcvars.bat`,
the matching mode-specific `conanbuildenv-<config>-x86_64.bat` script generated
by Conan, then runs the matching Windows preset configure/build in the same
shell. The third argument controls whether configure uses `--fresh`; the
default is `off`.

## Success Criteria

- `AGENT_DOCS/` contains `INDEX.md`, `NOW.md`, `INVARIANTS.md`, `VERIFY.md`,
  `ARCHITECTURE.md`, `CHANGE_MAP.md`, `CORE_STYLE.md`, `TEST_MAP.md`,
  `DECISIONS.md`, `GLOSSARY.md`, `CORE_RUNTIME.md`, `EDITOR_SYSTEM.md`,
  `INPUT_SYSTEM.md`, `JUDGE_SYSTEM.md`, `UTIL_SYSTEM.md`, `LIFECYCLES.md`,
  `DATA_CONTRACTS.md`, `KNOWN_GAPS.md`, and `PROJECT_SKILLS.md`
- `AGENT_DOCS/INDEX.md` links to `CORE_STYLE.md`
- `AGENT_DOCS/INDEX.md` links to `PROJECT_SKILLS.md`
- `AGENT_DOCS/skills/pdje-build-verify/SKILL.md` exists; run the skill-creator
  quick validator when `PyYAML` is available in the local Python environment
- root `CMakePresets.json` exists and exposes the host-appropriate preset names
- root compatibility aliases exist and point to the new canonical location
- `ctest --test-dir ./build -N -L unit` is non-empty
- the `Release` preset builds with `PDJE_DYNAMIC=ON` and both test flags off
- the `RelWithDebInfo` preset builds with `PDJE_DYNAMIC=ON` and both test
  flags on
- `ctest --preset <host>-relwithdebinfo` passes on the current host
- no control doc treats `docs/` or `BluePrint_PDJE/` as canonical
