# PDJE Agent Docs Index

`AGENT_DOCS/` is the canonical Markdown documentation surface for repository
operations, agent guidance, and source-backed project control notes.

This index follows the same broad idea as the Sphinx landing page: short
overview first, then the current control docs, then deeper references, then
legacy material.

## Start Here

- [NOW.md](NOW.md): current verified state, active caveats, and immediate risks
- [INVARIANTS.md](INVARIANTS.md): repository rules that should not drift
- [VERIFY.md](VERIFY.md): commands and success criteria for confirming reality
- [ARCHITECTURE.md](ARCHITECTURE.md): deeper module and data-flow map

## Control Docs

| Doc | Purpose |
| --- | --- |
| `NOW.md` | What is true right now and what to watch before making changes |
| `INVARIANTS.md` | Stable rules about doc ownership, build defaults, and archive boundaries |
| `VERIFY.md` | Quick and clean verification flows |

## Operational References

- [CHANGE_MAP.md](CHANGE_MAP.md): where to look first for each common change type
- [CORE_STYLE.md](CORE_STYLE.md): default structural rules generalized from the observed patterns in `include/core/`
- [PROJECT_SKILLS.md](PROJECT_SKILLS.md): repo-local Codex skills and when to use them
- [TEST_MAP.md](TEST_MAP.md): smallest stable verification routes by area
- [DECISIONS.md](DECISIONS.md): short rationale ledger for non-obvious repo choices
- [GLOSSARY.md](GLOSSARY.md): compact definitions for project-specific terms
- [KNOWN_GAPS.md](KNOWN_GAPS.md): current wiring and documentation gaps to keep visible

## Subsystem Deep Dives

- [CORE_RUNTIME.md](CORE_RUNTIME.md): `PDJE`, playback, search, and core data-line behavior
- [EDITOR_SYSTEM.md](EDITOR_SYSTEM.md): editor lifecycle, mutation, history, and push paths
- [INPUT_SYSTEM.md](INPUT_SYSTEM.md): input state machine, backend config, and input data-line rules
- [JUDGE_SYSTEM.md](JUDGE_SYSTEM.md): judge prerequisites, rails, note staging, and loop lifecycle
- [UTIL_SYSTEM.md](UTIL_SYSTEM.md): active utility surface, DB wrappers, STFT, and image helpers
- [LIFECYCLES.md](LIFECYCLES.md): valid call order across core, input, judge, and wrappers
- [DATA_CONTRACTS.md](DATA_CONTRACTS.md): current contracts for data lines, play modes, and timing types

## Deep Dives

- [ARCHITECTURE.md](ARCHITECTURE.md): canonical architecture deep dive
- [../README.md](../README.md): public-facing project landing page
- `../docs/`: redirecting HTML only; public docs live outside this checkout

## Current Known Risks

- The local `build/` cache does not represent source defaults.
- explicit C ABI unit test sources exist, but current unit target wiring does
  not include them.
- `Judge_Init::SetInputLine()` still assumes a non-null `input_arena`.
- `docs/` contains only redirecting HTML and must stay separate from
  agent-control docs.
- `BluePrint_PDJE/` can still surface in search results despite being archive
  material.

## Legacy References

- `../BluePrint_PDJE/`: archived design-note vault
- `../manual_doc_code_audit_2026-02-16.md`: historical `.rst` audit note
- root compatibility aliases:
  `../ARCHITECTURE.md`, `../PROJECT_STATE.md`, `../HOW_TO_VERIFY.md`
