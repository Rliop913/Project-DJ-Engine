# PDJE Build And Verification Flows

Use these commands when the repository state matches the current Windows workflow with the existing `winRelease` build tree.

## Baseline Docs

- Read `AGENT_DOCS/VERIFY.md` for repo-wide defaults.
- Read `AGENT_DOCS/TEST_MAP.md` for the smallest stable test route by area.
- Read `AGENT_DOCS/CHANGE_MAP.md` if the owning slice is still unclear.

## Util Hard-Refactor Command Set

Build the util unit target:

```powershell
cmake --build winRelease --config Debug --target pdje_unit_util
```

Run the util DB, status, and public-surface checks:

```powershell
ctest --test-dir winRelease -C Debug -R "unit.core::(rocksdb|annoy|sqlite|util)" --output-on-failure
```

Run the util signal, runtime, image, and WebP checks:

```powershell
ctest --test-dir winRelease -C Debug -R "unit.core::(backendless|post process rgb mode|stft|mel filter bank|waveform|encode_waveform_webps|encode_webp|write_webp)" --output-on-failure
```

## Dev Consumer Flow

Enable dev executables in the existing build tree:

```powershell
cmake -S . -B winRelease -DPDJE_DEV_TEST=ON
```

Build the waveform/WebP consumer executable:

```powershell
cmake --build winRelease --config Debug --target music_to_waveform_webp
```

Treat this build as compile compatibility only unless you also run the executable.

## Selection Notes

- Prefer the existing `winRelease` tree over creating a fresh Windows build tree when you only need incremental verification.
- Expect a fresh tree without the right Visual Studio environment to fail compiler discovery.
- Expect sandboxed Windows builds to sometimes fail in `ZERO_CHECK` or MSBuild file tracking with access-denied errors.
- If that happens, rerun the same configure or build with escalation instead of changing targets or switching to a different verification story.

## Reporting Notes

- Report the exact commands you ran.
- Report whether `winRelease` was reconfigured.
- Report whether a target was only built or also executed.
