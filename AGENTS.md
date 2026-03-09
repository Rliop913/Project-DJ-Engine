# AGENTS.md - PDJE Codebase Guide

Quick reference for coding agents working in Project DJ Engine (PDJE).  
C++20 engine for DJ/rhythm workflows. All source under `include/`.

---

## Build & Test Commands

### Full Build (Release)
```bash
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=./conan_cmakes/conan_toolchain.cmake \
  -DPDJE_TEST=ON \
  -DPDJE_DEV_TEST=OFF
cmake --build build --parallel
```

### Run All Unit Tests
```bash
ctest --test-dir build -L unit --output-on-failure
```

### List Available Tests
```bash
ctest --test-dir build -N
```

### Run Single Test (by pattern)
```bash
# Example: all input tests
ctest --test-dir build -R "unit.input::" --output-on-failure

# Example: single test case
ctest --test-dir build -R "unit.judge::judge: frame to microsecond" --output-on-failure
```

### Run Specific Test Binary Directly
```bash
./build/pdje_unit_core
./build/pdje_unit_input
./build/pdje_unit_judge
```

### Format Code
```bash
bash RunClangFormat.sh
```

---

## Code Style Guidelines

### Formatting (enforced by `.clang-format`)
- **Style**: LLVM-based
- **Indent**: 4 spaces
- **Braces**: Allman (break after function definition)
- **Return types**: Always break after return type
- **No bin packing**: Arguments and parameters on separate lines
- **Alignment**: Consecutive assignments and declarations aligned

### Naming Conventions
| Element | Convention | Example |
|---------|------------|---------|
| Classes/Structs | PascalCase | `BpmStruct`, `PDJE_JUDGE` |
| Functions | PascalCase | `MakeSampleBpmStructUnsorted()` |
| Variables | camelCase | `s.fragments`, `k1.Device_Name` |
| Constants | UPPER_SNAKE_CASE | `PDJE_KEY::A` |
| Namespaces | lowercase | `namespace PDJE_JUDGE` |
| Enum values | PascalCase or UPPER_SNAKE | `CORE_LINE_IS_MISSING` |

### Imports & Includes
- Headers: Use `#pragma once`
- Order (recommended): 1) paired header, 2) local includes, 3) project includes, 4) system includes
- Example:
  ```cpp
  #include "PDJE_Judge.hpp"           // paired header
  #include "PDJE_Note_OBJ.hpp"         // local
  #include "PDJE_LOG_SETTER.hpp"       // project
  #include <atomic>                     // system
  #include <exception>                  // system
  ```

### Types
- Use C++20 standard library types (`std::vector`, `std::string`, etc.)
- Prefer `unsigned long long` for frame/cursor positions
- Use `std::atomic` for thread-safe primitives
- Use `std::optional` / `std::variant` where appropriate

### Error Handling
- **Logging**: Use spdlog wrappers (`startlog()`, `critlog()`, `warnlog()`, `infolog()`, `debuglog()`)
- **Result pattern**: Use `PDJE_UTIL::common::Result<T>` for fallible operations
- **Exceptions**: Catch with `const std::exception& e`, always log before handling
- **Never suppress errors**: No `catch(...)` without logging

### Testing
- **Framework**: doctest
- **Test case naming**: `TEST_CASE("module: description")`
- **Assertions**: Use `CHECK()` for soft assertions, `REQUIRE()` for hard/fatal
- **Test file location**: `tests/unit/<module>/<name>.test.cpp`
- Example:
  ```cpp
  #include "FrameCalcCore.hpp"
  #include <doctest/doctest.h>

  TEST_CASE("core: bpm struct sorts fragments by beat/subBeat position")
  {
      auto s = MakeSampleBpmStructUnsorted();
      s.sortFragment();
      REQUIRE(s.fragments.size() == 3);
      CHECK(s.fragments[0].beat == 0);
  }
  ```

---

## Project Structure

```
include/
├── core/         # Main engine (audio, DB, editor, interface)
├── input/        # Input handling (keyboard, mouse, MIDI, evdev)
├── judge/        # Rhythm judgment (rails, notes, rules)
├── util/         # Utilities (AI, DB backends, image processing)
├── global/       # Cross-cutting (crypto, logging, IPC, clocks)
└── tests/        # Legacy manual tests
tests/
└── unit/         # Unit tests (doctest)
    ├── core/
    ├── input/
    ├── judge/
    └── util/
```

---

## Key CMake Options
| Option | Default | Description |
|--------|---------|-------------|
| `PDJE_TEST` | ON | Enable unit tests (doctest + CTest) |
| `PDJE_DEV_TEST` | OFF | Enable legacy manual test executables |
| `PDJE_DEVELOP_INPUT` | ON (Win), OFF (Linux/Mac) | Build input/judge modules |
| `PDJE_DYNAMIC` | OFF | Build as shared library |
| `PDJE_SWIG_BUILD` | OFF | Build Python/C# bindings |

---

## Important Files
- **Main API**: `include/core/interface/PDJE_interface.hpp`
- **Input**: `include/input/PDJE_Input.hpp`
- **Judge**: `include/judge/PDJE_Judge.hpp`
- **Logging**: `include/global/PDJE_LOG_SETTER.hpp`
- **Result type**: `include/util/common/Result.hpp`

---

## Common Patterns

### Data Line (pointer-based inter-module communication)
```cpp
struct PDJE_CORE_DATA_LINE {
    unsigned long long *nowCursor = nullptr;
    unsigned long long *maxCursor = nullptr;
    float *preRenderedData = nullptr;
    std::atomic<audioSyncData> *syncD = nullptr;
};
```
**Always check for null before use.**

### State Machine Pattern (Input module)
```cpp
if (!PDJE_INPUT_STATE_LOGIC::CanInit(state)) {
    critlog("pdje input module init failed...");
    return false;
}
```

### Logging + Exception Handling
```cpp
try {
    // operation
} catch (const std::exception &e) {
    critlog("failed to execute code. WHY: ");
    critlog(e.what());
    return false;
}
```

---

## CI / Build Notes
- CI runs: `cmake` → `cmake --build` → `ctest`
- Conan dependencies required before building
- Tests registered at **test-case level** (not just binary level)
