# Project-DJ-Engine

This project is a DJ/rhythm game engine written in C++. All sources live under `include/`.

## Documents

- 📚 **Documentation**: [PDJE DOCS](https://rliop913.github.io/Project-DJ-Engine)

- 🤖 **Codebase Assistant**: [AskToPDJE](https://github.com/Rliop913/AskToPDJE)

- 💻 **Discord**: [Official Discord](https://discord.gg/2Pwju7xhmS)

## Prebuilt

- **Godot Plugin** [Project_DJ_Godot](https://github.com/Rliop913/Project_DJ_Godot)

## Building

Prerequisites:

- C++20 compatible compiler
- CMake 3.12 or later
- SWIG
- OpenSSL
- [Conan](https://conan.io/)
  
To build the engine:

on linux & macos
```bash
bash ./BuildInitwithConan.sh . Release
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="../conan_cmakes/conan_toolchain.cmake" -DPDJE_DYNAMIC=OFF #to get dynamic library, change here
cmake --build . --parallel #add your maximum number of cores
```
on windows
```cmd
./BuildInitwithConan.bat . static Release
::to get dynamic library, change static into dynamic
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="../conan_cmakes/conan_toolchain.cmake" -DPDJE_DYNAMIC=OFF #to get dynamic library, change here
cmake --build . --config Release --parallel #add your maximum number of cores
```

> ⚠️ **WARNING**  
> To change the build type (Debug/Release) or switch between static and dynamic builds,  
> you must re-run the `BuildInitwithConan` script with the new options.  
> Otherwise, Conan dependencies may not be configured correctly.
> 
> If you plan to build Swig binding, you should use dynamic build on windows. static build may cause link errors.



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
## Generating Documentation

Documentation is produced using Doxygen and Sphinx. Ensure both tools are available in your PATH, then run:

```bash
bash DOCUMENT_GENERATOR.sh
```

The resulting HTML documentation will appear in the `docs/` directory.
