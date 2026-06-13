# PDJE Development Strategy

Use this when code work may affect direction, boundaries, behavior, or
verification cost.

## Defaults

- Prefer stable expansion over broad rewrites.
- Keep repo patterns unless a meeting closes a different direction.
- Developer approves direction, scope, and verification cost.
- Agent surfaces strategy drift, verification risk, and runtime impact.
- Every task gets a lightweight strategy check; structural/API/build/cost
  changes use [MEETING_PROTOCOL.md](MEETING_PROTOCOL.md).
- Large bug hunts use [BUG_HUNT_PROTOCOL.md](BUG_HUNT_PROTOCOL.md).

## Module Matrix

| Module | Collaboration | Model / Effort |
| --- | --- | --- |
| Core | Meeting first; pair programming; developer review; runtime-impact review before closure. | `gpt-5.5 xhigh` |
| Input | Meeting first; pair programming; review/runtime impact when risk is identified. | meeting `gpt-5.5 xhigh`; pair `gpt-5.5 high` |
| Judge | Meeting first; pair programming; timing/contract impact closed before implementation. | meeting `gpt-5.5 xhigh`; pair `gpt-5.5 high` |
| Util | Meeting first; separate Writer/Reviewer; Reviewer may block shared API, boundary, or verification risk. | meeting `gpt-5.5 xhigh`; Writer `gpt-5.4-mini high`; Reviewer `gpt-5.4-mini xhigh` |

## Cross-Cutting

- No silent model/effort substitution; escalate and close fallback explicitly.
- Multi-module work uses the strictest included strategy; any Core touch uses
  Core strategy.
- C ABI wrappers follow their owner: `CPDJE`=Core,
  `CPDJE_MODULE_INPUT`=Input, `CPDJE_MODULE_JUDGE`=Judge.
- Build/docs/test-wiring work follows the affected module, strictest if many.
- Strategy conflicts become meeting agenda before implementation.

## Core Runtime Review

Core closure needs expected impact on playback/editor lifecycle, data-line
lifetime/nullability, DB/search, timing-sensitive playback, and CPU/memory/
thread/IO cost.
