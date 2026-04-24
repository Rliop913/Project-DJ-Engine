@echo off

if "%~2"=="" (
    echo Usage: call windows_conf_and_build.bat [Release^|RelWithDebInfo] [jobs] [on^|off]
    exit /b 2
)

set "PDJE_CONFIG=%~1"
set "PDJE_JOBS=%~2"
set "PDJE_FRESH=%~3"
if "%PDJE_FRESH%"=="" set "PDJE_FRESH=off"
set "PDJE_ROOT=%~dp0"
set "PDJE_VCVARS_BAT=%PDJE_ROOT%conan_cmakes\conanvcvars.bat"

if /I "%PDJE_CONFIG%"=="Release" (
    set "PDJE_PRESET=windows-release"
    set "PDJE_ENV_BAT=%PDJE_ROOT%conan_cmakes\conanbuildenv-release-x86_64.bat"
) else if /I "%PDJE_CONFIG%"=="RelWithDebInfo" (
    set "PDJE_PRESET=windows-relwithdebinfo"
    set "PDJE_ENV_BAT=%PDJE_ROOT%conan_cmakes\conanbuildenv-relwithdebinfo-x86_64.bat"
) else (
    echo Usage: call windows_conf_and_build.bat [Release^|RelWithDebInfo] [jobs] [on^|off]
    exit /b 2
)

call "%PDJE_VCVARS_BAT%" || exit /b 1
call "%PDJE_ENV_BAT%" || exit /b 1
if /I "%PDJE_FRESH%"=="on" (
    cmake --fresh --preset %PDJE_PRESET% || exit /b 1
) else if /I "%PDJE_FRESH%"=="off" (
    cmake --preset %PDJE_PRESET% || exit /b 1
) else (
    echo Usage: call windows_conf_and_build.bat [Release^|RelWithDebInfo] [jobs] [on^|off]
    exit /b 2
)
cmake --build --preset %PDJE_PRESET% --parallel %PDJE_JOBS%
