# PDJE Build And Verification Flows

Use these commands when the repository state matches the current workflow with the shared `build` tree. Use only `RelWithDebInfo` or `Release`, and reuse one `BuildInitwithConan ... Release` bootstrap for both modes.

## Baseline Docs

- Read `AGENT_DOCS/VERIFY.md` for repo-wide defaults.
- Read `AGENT_DOCS/TEST_MAP.md` for the smallest stable test route by area.
- Read `AGENT_DOCS/CHANGE_MAP.md` if the owning slice is still unclear.

## Util Hard-Refactor Command Set

Initialize Conan once, then configure the shared build tree in one of the two allowed modes:

```powershell
BuildInitwithConan.bat . dynamic Release
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=./conan_cmakes/conan_toolchain.cmake -DPDJE_TEST=ON -DPDJE_DEV_TEST=ON -DPDJE_DYNAMIC=ON
```

Swap `RelWithDebInfo` for `Release` when you want the pure optimized mode.

Build the util unit target:

```powershell
cmake --build build --target pdje_unit_util
```

Run the util DB, status, and public-surface checks:

```powershell
ctest --test-dir build -R "unit.core::(rocksdb|annoy|sqlite|util)" --output-on-failure
```

Run the util signal, runtime, image, and WebP checks:

```powershell
ctest --test-dir build -R "unit.core::(backendless|post process rgb mode|stft|mel filter bank|waveform|encode_waveform_webps|encode_webp|write_webp)" --output-on-failure
```

## Dev Consumer Flow

Enable dev executables in the existing build tree:

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=./conan_cmakes/conan_toolchain.cmake -DPDJE_DEV_TEST=ON
```

Build the waveform/WebP consumer executable:

```powershell
cmake --build build --target music_to_waveform_webp
```

Treat this build as compile compatibility only unless you also run the executable.

## Selection Notes

- Prefer the existing `build` tree over creating a fresh build tree when you only need incremental verification.
- Expect a fresh tree without the right Visual Studio environment to fail compiler discovery.
- Expect sandboxed Windows builds to sometimes fail in `ZERO_CHECK` or MSBuild file tracking with access-denied errors.
- If that happens, rerun the same configure or build with escalation instead of changing targets or switching to a different verification story.

## Reporting Notes

- Report the exact commands you ran.
- Report whether `build` was reconfigured.
- Report whether a target was only built or also executed.
