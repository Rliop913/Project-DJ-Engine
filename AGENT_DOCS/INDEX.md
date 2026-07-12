# PDJE Agent Docs

This directory is a source-backed navigation and control surface for coding
agents. It intentionally describes stable boundaries and current contracts;
source files remain authoritative for implementation detail.

## Task Routes

| Need | Read |
| --- | --- |
| orient in the project | [ARCHITECTURE.md](ARCHITECTURE.md) |
| find the owning code and CMake list | [CHANGE_MAP.md](CHANGE_MAP.md) |
| change lifecycle, state, ownership, or public API behavior | [RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md) |
| choose an approved build/test route | [VERIFY.md](VERIFY.md), then [TEST_MAP.md](TEST_MAP.md) |
| maintain this harness | [MAINTENANCE.md](MAINTENANCE.md) |

Always apply [INVARIANTS.md](INVARIANTS.md).

## Active Documents

| Document | Purpose |
| --- | --- |
| `INVARIANTS.md` | repository boundaries and facts that agents must preserve |
| `ARCHITECTURE.md` | project purpose, components, build graph, and runtime flow |
| `CHANGE_MAP.md` | change-to-owner/source-list/test routing |
| `RUNTIME_CONTRACTS.md` | cross-module lifecycles, ownership, and current edge behavior |
| `VERIFY.md` | approval rule, preset matrix, and verification sequence |
| `TEST_MAP.md` | stable target/label coverage and known gaps |
| `MAINTENANCE.md` | source-of-truth hierarchy and harness update triggers |

The repo-local build skill is
[skills/pdje-build-verify/SKILL.md](skills/pdje-build-verify/SKILL.md). It does
not grant permission to run commands.

## Documentation Boundaries

- [../README.md](../README.md) is the public landing page.
- `../docs/index.html` redirects to the external documentation site.
- `../BluePrint_PDJE/` is historical material, not current project truth.
