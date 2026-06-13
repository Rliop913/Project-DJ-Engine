# PDJE Agent Docs Index

`AGENT_DOCS/` is the canonical Markdown documentation surface for repository
operations, agent guidance, and source-backed project control notes.

This index keeps the active control surface small: entry docs first, then
operational references, then subsystem deep dives.

## Start Here

- [INVARIANTS.md](INVARIANTS.md): repository rules that should not drift
- [VERIFY.md](VERIFY.md): commands and success criteria for confirming reality
- [ARCHITECTURE.md](ARCHITECTURE.md): deeper module and data-flow map

## Control Docs

| Doc | Purpose |
| --- | --- |
| `INVARIANTS.md` | Stable rules about doc ownership, build defaults, and archive boundaries |
| `VERIFY.md` | Quick and clean verification flows |

## Operational References

- [CHANGE_MAP.md](CHANGE_MAP.md): where to look first for each common change type
- [CORE_STYLE.md](CORE_STYLE.md): default structural rules generalized from the observed patterns in `include/core/`
- [PROJECT_SKILLS.md](PROJECT_SKILLS.md): repo-local Codex skills and when to use them
- [TEST_MAP.md](TEST_MAP.md): smallest stable verification routes by area
- [DECISIONS.md](DECISIONS.md): short rationale ledger for non-obvious repo choices
- [GLOSSARY.md](GLOSSARY.md): compact definitions for project-specific terms
- [strategy/STRATEGY.md](strategy/STRATEGY.md): developer-agent collaboration strategy by module
- [strategy/MEETING_PROTOCOL.md](strategy/MEETING_PROTOCOL.md): meeting flow for closing strategy agendas into plans
- [strategy/BUG_HUNT_PROTOCOL.md](strategy/BUG_HUNT_PROTOCOL.md): large-scale bug-hunt coordination and acceptance protocol

## Subsystem Deep Dives

- [CORE_RUNTIME.md](CORE_RUNTIME.md): `PDJE`, playback, search, and core data-line behavior
- [EDITOR_SYSTEM.md](EDITOR_SYSTEM.md): editor lifecycle, mutation, history, and push paths
- [INPUT_SYSTEM.md](INPUT_SYSTEM.md): input state machine, backend config, and input data-line rules
- [JUDGE_SYSTEM.md](JUDGE_SYSTEM.md): judge prerequisites, rails, note staging, and loop lifecycle
- [UTIL_SYSTEM.md](UTIL_SYSTEM.md): active utility surface, DB wrappers, STFT, and image helpers
- [LIFECYCLES.md](LIFECYCLES.md): valid call order across core, input, judge, and wrappers
- [DATA_CONTRACTS.md](DATA_CONTRACTS.md): current contracts for data lines, play modes, and timing types

## External

- [../README.md](../README.md): public-facing project landing page
- `../docs/`: redirecting HTML only; public docs live outside this checkout
