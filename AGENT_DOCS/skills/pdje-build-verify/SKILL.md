---
name: pdje-build-verify
description: Build and verify changes in Project_DJ_Engine using the repository's proven CMake and CTest flows. Use when Codex needs to choose a repo-specific build target, run focused verification after code changes, reconfigure the existing `winRelease` tree with `PDJE_DEV_TEST`, or validate util/STFT/waveform work against `music_to_waveform_webp` and other PDJE targets.
---

# PDJE Build Verify

## Overview

Use this skill to pick the smallest reliable build and verification path for this repository, especially on Windows with the existing `winRelease` tree. Prefer it when generic advice would miss PDJE-specific target names, test regexes, or the repository's current build-tree habits.

## Start

- Read `AGENT_DOCS/VERIFY.md` for repository-wide verification rules and success criteria.
- Read `AGENT_DOCS/TEST_MAP.md` when you need the smallest stable test route for a subsystem.
- Read `AGENT_DOCS/CHANGE_MAP.md` when the owning slice is still ambiguous.
- Prefer the existing `winRelease` build tree on Windows when it already exists.

## Choose The Flow

- Use the repository-wide flow from `AGENT_DOCS/VERIFY.md` when the change is cross-cutting or the affected subsystem is unclear.
- Use the util-focused commands in `references/verification-flows.md` when working on `include/util/`, util tests, OpenCL/STFT, waveform/WebP, or util-adjacent build wiring.
- Use the dev-consumer flow in `references/verification-flows.md` when the change must compile against `include/tests/music_to_waveform_webp.cpp` or another `PDJE_DEV_TEST` executable.

## Working Rules

- Build the narrowest target first with `cmake --build winRelease --config Debug --target <target>`.
- Run focused `ctest` regexes before broader suites.
- Reconfigure the existing `winRelease` tree with `-DPDJE_DEV_TEST=ON` instead of creating a fresh tree when you only need a dev executable.
- Treat `music_to_waveform_webp` build success as compile coverage only; say explicitly if you did not run the executable.
- If sandboxed MSBuild or `ZERO_CHECK` fails because of file-tracking or access-denied issues, rerun the same build with escalation instead of inventing a new build path.
- If a fresh build tree cannot find the compiler, fall back to the existing configured tree before spending time on generator debugging.

## Report The Result

- List the exact build and test commands you ran.
- State which target or regex each command was meant to validate.
- Call out unverified edges, especially binaries that were built but not executed.
- Mention if `winRelease` was reconfigured during the task, because that changes the local build-tree state.

## References

- Read `references/verification-flows.md` for the exact command sets already used successfully in this repository.
