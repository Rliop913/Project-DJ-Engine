# PDJE Platform Verification Flows

These are command shapes for the root `CMakePresets.json` and `./build`. They
require the approval described in `AGENT_DOCS/VERIFY.md`.

Choose `Release` for compile-only work and `RelWithDebInfo` for unit or
manual/dev verification. Bootstrap Conan when dependencies for that host and
mode are not already prepared.

## Linux

~~~bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . <Release|RelWithDebInfo>
cmake --preset linux-<release|relwithdebinfo>
cmake --build --preset linux-<release|relwithdebinfo>
~~~

## macOS

~~~bash
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . <Release|RelWithDebInfo>
cmake --preset macos-<release|relwithdebinfo>
cmake --build --preset macos-<release|relwithdebinfo>
~~~

The macOS preset disables input/judge. The current unit root also excludes
`pdje_unit_util` on Apple.

## Windows

~~~cmd
BuildInitwithConan.bat . dynamic <Release|RelWithDebInfo>
call .\windows_conf_and_build.bat <Release|RelWithDebInfo> <jobs> <on|off>
~~~

The wrapper activates the Conan/MSVC environment and performs configure plus
build in one `cmd.exe` process. Its third argument controls `cmake --fresh` and
defaults to `off`; choose `on` only when the approved flow requires a clean
configure.

## Narrow Build And Test

After an approved RelWithDebInfo configure:

~~~text
cmake --build --preset <host>-relwithdebinfo --target <target>
ctest --test-dir ./build -L <module> --output-on-failure
~~~

Use target and label names from `AGENT_DOCS/TEST_MAP.md`. For the full
host-applicable suite:

~~~text
cmake --build --preset <host>-relwithdebinfo
ctest --preset <host>-relwithdebinfo
~~~

Report whether bootstrap/configure occurred, whether a manual target was only
built or also run, and which platform or source-list gaps remain.
