# PDJE Build And Verification Flows

Use these commands when the repository state matches the current workflow with the checked-in preset matrix in root `CMakePresets.json` and the repo-root `/build` directory.

## Baseline Docs

- Read `AGENT_DOCS/VERIFY.md` for repo-wide defaults.
- Read `AGENT_DOCS/TEST_MAP.md` for the smallest stable test route by area.
- Read `AGENT_DOCS/CHANGE_MAP.md` if the owning slice is still unclear.

## Compile-Only Command Set

Use the `Release` presets when you only need a dynamic compile check without
unit or dev tests.

Linux:

```powershell
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset linux-release
cmake --build --preset linux-release
```

macOS:

```powershell
CC=clang CXX=clang++ bash ./BuildInitwithConan.sh . Release
cmake --preset macos-release
cmake --build --preset macos-release
```

Windows:

```cmd
BuildInitwithConan.bat . dynamic Release
call .\windows_conf_and_build.bat Release 16 on
```

## Util Hard-Refactor Command Set

Use the `RelWithDebInfo` presets when you need unit coverage plus dev-test
executables.

Windows:

```cmd
BuildInitwithConan.bat . dynamic RelWithDebInfo
call .\windows_conf_and_build.bat RelWithDebInfo 16 on
```

Build the util unit target from the shared preset-configured `./build` tree:

```powershell
cmake --build --preset windows-relwithdebinfo --target pdje_unit_util
```

Run the util DB, status, and public-surface checks:

```powershell
ctest --test-dir ./build -R "unit.core::(rocksdb|annoy|sqlite|util)" --output-on-failure
```

Run the util signal, runtime, image, and WebP checks:

```powershell
ctest --test-dir ./build -R "unit.core::(backendless|post process rgb mode|stft|mel filter bank|waveform|encode_waveform_webps|encode_webp|write_webp)" --output-on-failure
```

## Dev Consumer Flow

Build the waveform/WebP consumer executable:

```powershell
cmake --build --preset windows-relwithdebinfo --target music_to_waveform_webp
```

Treat this build as compile compatibility only unless you also run the executable.

## Selection Notes

- Prefer the existing `./build` directory over creating a fresh build tree when you only need incremental verification.
- Expect a fresh tree without the right compiler shell or mismatched Conan bootstrap to fail compiler discovery.
- Expect sandboxed Windows builds to sometimes fail in `ZERO_CHECK` or MSBuild file tracking with access-denied errors.
- If that happens, rerun the same configure or build with escalation instead of changing targets or switching to a different verification story.
- `windows_conf_and_build.bat` keeps the Conan/MSVC environment and the matching Windows preset configure/build inside one `cmd` process, and its third argument toggles `--fresh` on or off. The default is `off`. It still assumes the matching Conan-generated files already exist in `conan_cmakes/`.
- Linux and macOS presets lock `clang` / `clang++`, so the preceding Conan bootstrap must be run with matching compiler environment.

## Reporting Notes

- Report the exact commands you ran.
- Report whether `./build` was reconfigured.
- Report whether a target was only built or also executed.
