# PDJE Development Strategy

This document defines how developers and agents should work together when a
code change may affect project direction, module boundaries, public behavior,
or verification cost.

## Common Strategy

- Prefer stable expansion over broad rewrites.
- Keep existing repository patterns as the default unless a meeting closes a
  different direction.
- Treat the developer as the strategy approver for direction, scope, and
  verification cost.
- Treat the agent as a conservative implementer that must surface strategy
  drift, verification risk, and runtime impact before those risks are hidden in
  code.
- Apply a lightweight strategy check to every code task.
- Use the full meeting protocol when a task changes structure, API shape,
  build behavior, verification cost, or long-term direction.
- Use [BUG_HUNT_PROTOCOL.md](BUG_HUNT_PROTOCOL.md) for large-scale bug-hunt
  coordination, Finder/Verifier/Lead roles, and accepted-bug recording.

## Module Collaboration Matrix

| Module | Collaboration Strategy | Model / Effort |
| --- | --- | --- |
| Core | Meeting first. Real-time developer-agent pair programming is required. Developer review feedback must be incorporated. Agent runtime-impact review is required before closure. | `gpt-5.5`, always `xhigh` |
| Input | Meeting first. Pair programming is required for implementation. Developer review and runtime-impact review are applied when the meeting identifies risk. | Meeting: `gpt-5.5 xhigh`; implementation pair: `gpt-5.5 high` |
| Judge | Meeting first. Pair programming is required for implementation. Timing and contract impact must be closed in the meeting before implementation starts. | Meeting: `gpt-5.5 xhigh`; implementation pair: `gpt-5.5 high` |
| Util | Meeting first. Writer and Reviewer roles are always separated, including small changes. The Reviewer can block completion on shared API, boundary, or verification risk. | Meeting: `gpt-5.5 xhigh`; Writer: `gpt-5.4-mini high`; Reviewer: `gpt-5.4-mini xhigh` |

## Cross-Cutting Rules

- If a required model or effort is unavailable, do not silently substitute a
  different one. Escalate to a meeting and close the fallback explicitly.
- If one task touches multiple modules, apply the strictest included module
  strategy. Any Core involvement promotes the whole task to the Core strategy.
- C ABI wrapper work follows the owning module strategy:
  `CPDJE` follows Core, `CPDJE_MODULE_INPUT` follows Input, and
  `CPDJE_MODULE_JUDGE` follows Judge.
- Build, documentation, and test-wiring work follows the strategy of the
  affected module. If several modules are affected, use the strictest included
  strategy.
- If common strategy and module strategy conflict, make the conflict a meeting
  agenda item and close it before implementation.

## Core Runtime-Impact Review

Core work must include an agent review of expected runtime impact before the
task is considered closed. The review must cover:

- playback and editor lifecycle behavior
- non-owning data-line lifetime and nullability
- DB/search behavior
- playback behavior and timing-sensitive paths
- CPU, memory, thread, and IO cost changes
