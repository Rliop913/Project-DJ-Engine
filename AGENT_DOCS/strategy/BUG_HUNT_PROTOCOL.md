# PDJE Bug Hunt Protocol

Multi-module investigation protocol. It records accepted bugs; fixes move to a
separate plan.

## Scope And Roles

| Item | Rule |
| --- | --- |
| Use case | broader than one focused review or touching multiple module surfaces |
| Default scopes | Core, Input, Judge, Util |
| Cross-cutting | build, C ABI, test wiring, docs, and similar findings are classified by Lead |
| Lead | scope, assignments, duplicate merges, accepted-bug list; `gpt-5.5 xhigh` |
| Finder | suspected bugs with evidence for verification; `gpt-5.4-mini medium` |
| Verifier | independent confirm/reject; `gpt-5.4-mini xhigh` |

Missing required model/effort requires a strategy meeting and explicit fallback.

## Run Rules

| Stage | Requirement |
| --- | --- |
| Open | Lead records date, mode, coverage, constraints, assignments. |
| Find | Finders file candidates; Lead merges duplicates. |
| Verify | Verifiers cross-check outside their Finder role; accepted bugs need independent evidence. |
| Close | Lead records verifier-accepted bugs, rejected/deferred themes, cross-cutting scope, and follow-up notes. |

## Candidate And Record Shape

| Item | Requirement |
| --- | --- |
| Candidate fields | ID, area, summary, suspected failure mode, evidence/code path, expected behavior, observed/inferred behavior, verifier notes |
| Active states | `new`, `needs-info`, `under-review` |
| Terminal states | `accepted`, `rejected`, `duplicate`, `deferred` |
| Accepted record | severity, modules, independent evidence, expected vs actual/inferred behavior, verification mode, approved commands, commands run, not-run reason, fix-plan status |
| Severity | `Critical` blocks/corrupts/crashes/high-impact contract; `Major` breaks narrower feature/lifecycle/platform/test contract; `Minor` covers correctness/diagnostics/maintainability/edge cases |

No run closes with active candidates; unresolved candidates become `deferred`
with reason and follow-up condition.

## Verification And Fix Policy

- Static investigation is default.
- `conan install`, `cmake`, `cmake --build`, `ctest`, smoke binaries, and
  wrappers require meeting decision plus explicit approval.
- Records must say what was approved, what ran, and why approved commands were
  not run.
- Do not fix bugs inside the run. Emergency fixes require an explicit meeting
  decision before code changes begin.
