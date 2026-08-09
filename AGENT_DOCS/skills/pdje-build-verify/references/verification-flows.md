# PDJE Platform Verification Commands

These are the exact command forms for the presets and repo-root `./build`.
Selection and approval belong to [../../../VERIFY.md](../../../VERIFY.md), and
target/label availability belongs to [../../../TEST_MAP.md](../../../TEST_MAP.md).

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

## Windows

~~~cmd
BuildInitwithConan.bat . dynamic <Release|RelWithDebInfo>
call .\windows_conf_and_build.bat <Release|RelWithDebInfo> <jobs> <on|off>
~~~

The Windows wrapper performs configure and build in one activated Conan/MSVC
process. Its final argument controls `cmake --fresh` and defaults to `off`.

## Narrow Target And Label

~~~text
cmake --build --preset <host>-relwithdebinfo --target <target>
ctest --test-dir ./build -L <label> --output-on-failure
~~~

## Full Host-Applicable Suite

~~~text
cmake --build --preset <host>-relwithdebinfo
ctest --preset <host>-relwithdebinfo
~~~
