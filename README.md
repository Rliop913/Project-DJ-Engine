# Project-DJ-Engine

This project is a DJ/rhythm game engine written in C++. All sources live under `include/`.

## Documents

- 📚 **Documentation**: [PDJE DOCS](https://rliop913.github.io/Project-DJ-Engine-Docs/)

- 🤖 **Codebase Assistant**: [AskToPDJE](https://github.com/Rliop913/AskToPDJE)

- 💻 **Discord**: [Official Discord](https://discord.gg/2Pwju7xhmS)

## Prebuilt

- **Godot Plugin**: [Project_DJ_Godot](https://github.com/Rliop913/Project_DJ_Godot)

## Building

Prerequisites:

- C++20 compatible compiler
- CMake 3.23 or later for the checked-in preset workflow
- SWIG when `PDJE_SWIG_BUILD=ON`
- OpenSSL
- [Conan](https://conan.io/)
  
To build the engine:

The checked-in presets are the canonical build entrypoint:

- `windows-release`
- `windows-relwithdebinfo`
- `linux-release`
- `linux-relwithdebinfo`
- `macos-release`
- `macos-relwithdebinfo`

Preset rules:

- all presets build in `./build`
- `PDJE_DYNAMIC=ON` in every preset
- `*-release` sets `PDJE_TEST=OFF` and `PDJE_DEV_TEST=OFF`
- `*-relwithdebinfo` sets `PDJE_TEST=ON` and `PDJE_DEV_TEST=ON`
- Linux presets fix `clang` / `clang++`
- macOS presets fix `clang` / `clang++` and expect AppleClang from Xcode or Command Line Tools
- Windows presets fix `cl` / `cl` and still require an MSVC x64 environment

Bootstrap Conan before running a preset. The Conan output must match the platform,
compiler, and build type of the preset you choose.

on linux
```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset linux-release
cmake --build --preset linux-release

CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset linux-relwithdebinfo
cmake --build --preset linux-relwithdebinfo
ctest --preset linux-relwithdebinfo
```

on macOS
```bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset macos-release
cmake --build --preset macos-release

CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . RelWithDebInfo
cmake --preset macos-relwithdebinfo
cmake --build --preset macos-relwithdebinfo
ctest --preset macos-relwithdebinfo
```

on windows
```cmd
BuildInitwithConan.bat . dynamic Release
call .\windows_conf_and_build.bat Release 16 on

BuildInitwithConan.bat . dynamic RelWithDebInfo
call .\windows_conf_and_build.bat RelWithDebInfo 16 on
ctest --preset windows-relwithdebinfo
```

> ⚠️ **WARNING**  
> To change the build type or switch between compilers on Linux/macOS,  
> you must re-run the `BuildInitwithConan` script with the new options.  
> Otherwise, Conan dependencies may not be configured correctly.
> 
> On Windows, `windows_conf_and_build.bat` is the canonical helper for preset-based
> configure+build runs. Use
> `call .\windows_conf_and_build.bat Release <jobs> <on|off>` or
> `call .\windows_conf_and_build.bat RelWithDebInfo <jobs> <on|off>` from `cmd.exe`, a
> Developer Prompt, or another batch file. The helper wraps `conanvcvars.bat`,
> the matching mode-specific `conanbuildenv-<config>-x86_64.bat` script, then
> runs the matching Windows preset configure/build in the same shell.
> The third argument controls whether configure uses `--fresh`; the default is `off`.
> The generated `conanbuild.bat` and `conanrun.bat` follow the last Conan install
> and are not stable when switching between `Release` and `RelWithDebInfo`.

## Tests

The project now separates automated unit tests from legacy/manual developer tests.

- source defaults still come from `cmakes/Options.cmake`
- checked-in presets intentionally use a different matrix
- `*-release` presets skip both test families
- `*-relwithdebinfo` presets build doctest unit tests and legacy/manual dev tests

`PDJE_TEST=ON` now registers doctest cases with CTest at the **test-case level**
(via post-build discovery). This means `ctest -N` shows the expanded list after
the unit test binaries are built.

Examples:

```bash
# Run the preset-managed unit suite for the current host
ctest --preset linux-relwithdebinfo
# or
ctest --preset macos-relwithdebinfo
# or
ctest --preset windows-relwithdebinfo
```

```bash
# Optional: run a focused test regex from the shared ./build tree after
# a *-relwithdebinfo configure/build
ctest --test-dir ./build -R "unit.input::" --output-on-failure
```



## Dependencies

This project uses the following dependencies, which are automatically fetched using `FetchContent` or `Conan`:

- **[miniaudio](https://github.com/mackron/miniaudio)**: Audio playback and capture library.
- **[highway](https://github.com/google/highway)**: High-performance SIMD library.
- **[CapnProto](https://github.com/capnproto/capnproto)**: Serialization library.
- **[nlohmann/json](https://github.com/nlohmann/json)**: JSON for Modern C++.
- **[sqlite3](https://www.sqlite.org/index.html)**: SQL database engine.
- **[cppcodec](https://github.com/tplgy/cppcodec)**: Header-only C++11 library for encoding/decoding schemes.
- **[annoy](https://github.com/spotify/annoy)**: Approximate Nearest Neighbors Oh Yeah.
- **[spdlog](https://github.com/gabime/spdlog)**: Fast C++ logging library.
- **[libgit2](https://github.com/libgit2/libgit2)**: Git core library.
- **[rocksdb](https://github.com/facebook/rocksdb)**: A library that provides an embeddable, persistent key-value store for fast storage.
- **[SoundTouch](https://www.surina.net/soundtouch/)**: Audio processing library.
- **[Faust language](https://github.com/grame-cncm/faust)**: Functional Programming Language for Real Time Signal Processing
- **[Botan](https://github.com/randombit/botan)**: Cryptography Toolkit
- **[Beat This](https://github.com/CPJKU/beat_this)**: Beat and downbeat tracking model pipeline.
- **[soxr pysoxr fork](https://github.com/dofuuz/soxr.git)**: Sample-rate conversion library used through the pysoxr fork.
- **[ONNX Runtime](https://github.com/microsoft/onnxruntime.git)**: Runtime for ONNX model inference.
## Generating Documentation

Documentation is produced using Doxygen and Sphinx. Ensure both tools are available in your PATH, then run:

```bash
bash DOCUMENT_GENERATOR.sh
```

The resulting HTML documentation will appear in the `docs/` directory.
