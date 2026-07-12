---
name: pdje-build-verify
description: Build and verify Project-DJ-Engine with its checked-in presets, explicit targets, and CTest module labels. Use for repo-specific configure/build/test selection after the user approves a platform, mode, and verification scope.
---

# PDJE Build Verify

Use this skill only for build or executable verification. Documentation-only
inspection does not need it.

## Required Reading

1. Read `AGENT_DOCS/VERIFY.md`.
2. Read `AGENT_DOCS/TEST_MAP.md`.
3. Read `AGENT_DOCS/CHANGE_MAP.md` if ownership is unclear.
4. Read `references/verification-flows.md` only when dependency bootstrap or
   platform command syntax is needed.

## Permission Guard

The skill does not grant command permission. Before the first Conan, CMake
configure/build, CTest, smoke executable, or wrapper command, obtain the user's
host preset/configuration and verification scope. Use only repo-root `./build`
unless the user explicitly approves another build directory.

## Select The Smallest Reliable Flow

- Compile-only acceptance: matching `*-release` preset.
- Unit or dev/manual acceptance: matching `*-relwithdebinfo` preset.
- One subsystem: build its target, then run its CTest module label.
- Cross-cutting source/build changes: build the configured tree, then run the
  matching test preset.
- Manual/dev targets: report compile and execution coverage separately.

Respect platform gates: input/judge are disabled on macOS, and util unit targets
are currently excluded on Apple even though `PDJE_UTIL` itself builds.

## Report

State the exact commands, selected preset, whether dependencies or `./build`
were reconfigured, targets and labels exercised, pass/fail result, and all
gated or unwired edges. Never infer coverage merely from a test source existing.
