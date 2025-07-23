# Project_DJ_Engine

This project is a DJ/rhythm game engine written in C++. All sources live under `include/` and `source/`.

## Building

Prerequisites:

- C++20 compatible compiler
- CMake 3.12 or later

To build the engine:

```bash
mkdir build
cd build
cmake ..
make
```

## Generating Documentation

Documentation is produced using Doxygen and Sphinx. Ensure both tools are available in your PATH, then run:

```bash
bash DOCUMENT_GENERATOR.sh
```

The resulting HTML documentation will appear in the `docs/` directory.
