---
name: pdje-build-verify
description: Build and verify changes in Project_DJ_Engine using the repository's checked-in preset matrix and CTest flows. Use when Codex needs to choose a repo-specific build target, run focused verification after code changes, reconfigure the repo-root `./build` directory through presets, or validate subsystem-specific unit and dev/manual executable coverage.
---

# PDJE Build Verify

## Overview

Use this skill to pick the smallest reliable PDJE build/test path when generic
advice would miss target names, regexes, presets, or `./build` habits.

## Start

- Read `AGENT_DOCS/VERIFY.md` for repository-wide verification rules and success criteria.
- Read `AGENT_DOCS/TEST_MAP.md` when you need the smallest stable test route for a subsystem.
- Read `AGENT_DOCS/CHANGE_MAP.md` when the owning slice is still ambiguous.
- Use root `CMakePresets.json`, repo-root `./build`, and only `Release` or
  `RelWithDebInfo`.
- Keep `PDJE_DYNAMIC=ON`; only `*-relwithdebinfo` enables `PDJE_TEST` and
  `PDJE_DEV_TEST`.
- Run matching `BuildInitwithConan*.{bat,sh}` before the platform preset.
- On Windows, prefer `call .\windows_conf_and_build.bat <Release|RelWithDebInfo> <jobs> <on|off>` for one-shell Conan/MSVC configure+build.
- Reuse existing `./build` when possible.

## Choose The Flow

- Use the repository-wide flow from `AGENT_DOCS/VERIFY.md` when the change is cross-cutting or the affected subsystem is unclear.
- Use `AGENT_DOCS/TEST_MAP.md` for focused unit routes.
- Use `references/verification-flows.md` for platform bootstrap and preset command
  shapes.
- Build a changed `PDJE_DEV_TEST` executable when the change affects a
  dev/manual consumer.

## Working Rules

- Build the narrowest target first with `cmake --build --preset <host>-<mode> --target <target>`.
- Run focused `ctest` regexes before broader suites.
- Reconfigure the existing `./build` directory by rerunning the matching preset instead of inventing a new build tree.
- Treat dev/manual executable build success as compile coverage only unless
  executed.
- If sandboxed MSBuild or `ZERO_CHECK` hits file-tracking/access-denied issues,
  rerun the same build with escalation.
- If fresh `./build` cannot find the compiler, check shell/bootstrap vs preset
  compiler lock first.

## Report The Result

- List the exact build and test commands you ran.
- State which target or regex each command was meant to validate.
- Call out unverified edges, especially binaries that were built but not executed.
- Mention if `./build` was reconfigured during the task, because that changes the local build-tree state.

## References

- Read `references/verification-flows.md` for platform bootstrap and preset
  command sets.
