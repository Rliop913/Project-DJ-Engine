# PDJE Verify

This is the thin verification doc for the repository and its control-doc set.

## Quick Verify Existing Checkout

Check source defaults:

```bash
rg -n "option\\(PDJE_(TEST|DEV_TEST|DYNAMIC|SWIG_BUILD|DEVELOP_INPUT)" cmakes/Options.cmake
```

Inspect the local build cache separately:

```bash
cmake -LA -N build | rg '^PDJE_|^CMAKE_BUILD_TYPE'
```

Enumerate unit tests:

```bash
ctest --test-dir build -N -L unit
```

Run the unit-label suite:

```bash
ctest --test-dir build -L unit --output-on-failure
```

For area-specific commands, use [TEST_MAP.md](TEST_MAP.md).

Check the doc surface:

```bash
find AGENT_DOCS -maxdepth 1 -name '*.md' | sort
find . -maxdepth 1 -type f -name '*.md' | sort
rg -n "CORE_STYLE\\.md" AGENT_DOCS/INDEX.md AGENT_DOCS/VERIFY.md
```

## Clean Verify

Linux / macOS:

```bash
bash ./BuildInitwithConan.sh . Release
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=./conan_cmakes/conan_toolchain.cmake \
  -DPDJE_TEST=ON \
  -DPDJE_DEV_TEST=OFF \
  -DPDJE_DYNAMIC=OFF
cmake --build build --parallel
ctest --test-dir build -L unit --output-on-failure
```

Windows:

```bat
BuildInitwithConan.bat . static Release
cmake -B build -S . ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE=./conan_cmakes/conan_toolchain.cmake ^
  -DPDJE_TEST=ON ^
  -DPDJE_DEV_TEST=OFF ^
  -DPDJE_DYNAMIC=OFF
cmake --build build --config Release --parallel
ctest --test-dir build -C Release -L unit --output-on-failure
```

## Success Criteria

- `AGENT_DOCS/` contains `INDEX.md`, `NOW.md`, `INVARIANTS.md`, `VERIFY.md`,
  `ARCHITECTURE.md`, `CHANGE_MAP.md`, `CORE_STYLE.md`, `TEST_MAP.md`,
  `DECISIONS.md`, `GLOSSARY.md`, `CORE_RUNTIME.md`, `EDITOR_SYSTEM.md`,
  `INPUT_SYSTEM.md`, `JUDGE_SYSTEM.md`, `UTIL_SYSTEM.md`, `LIFECYCLES.md`,
  `DATA_CONTRACTS.md`, and `KNOWN_GAPS.md`
- `AGENT_DOCS/INDEX.md` links to `CORE_STYLE.md`
- root compatibility aliases exist and point to the new canonical location
- `ctest --test-dir build -N -L unit` is non-empty
- the unit-label test run passes in the chosen verification mode
- no control doc treats `docs/` or `BluePrint_PDJE/` as canonical
