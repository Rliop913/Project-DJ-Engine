# PDJE Large-Scale Bug Hunt Protocol

This protocol coordinates large bug-hunting runs. It is for investigation,
verification, and accepted-bug recording only. Fixes move into separate
development plans after the run closes.

## Run Location

Create each run under:

```text
AGENT_DOCS/audits/bug-hunt-YYYY-MM-DD/
```

Use the templates in `bug-hunt-template/` when starting a new run.

## Roles

| Role | Responsibility | Model / Effort |
| --- | --- | --- |
| Lead | Defines scope, assigns module coverage, merges duplicate candidates, classifies cross-cutting scope, and writes final accepted bugs. | `gpt-5.5 xhigh` |
| Finder | Searches assigned module scope and records suspected bugs. A strong suspicion is enough to file a candidate. | `gpt-5.4-mini medium` |
| Verifier | Cross-checks Finder candidates independently and marks disposition. Accepted bugs require independent confirmation. | `gpt-5.4-mini xhigh` |

If a required model or effort is unavailable, do not silently substitute another
one. Escalate to a strategy meeting and close the fallback explicitly.

## Scope Partition

Default Finder scopes are:

- Core
- Input
- Judge
- Util

Build, test wiring, C ABI, documentation, and other cross-cutting findings are
recorded wherever they are found. The Lead classifies those findings in final
review and applies the strictest affected module strategy when follow-up work is
planned.

## Run Flow

1. The Lead opens the run, records date, mode, module coverage, and constraints.
2. The Lead records the Finder and Verifier assignment table in `RUN.md`.
3. Finders investigate their module scopes in parallel.
4. Finders record candidates with enough context for independent verification.
5. The Lead merges duplicates into canonical candidates when overlap appears.
6. Verifiers cross-check submitted candidates outside their Finder role.
7. The Lead records only verifier-accepted bugs in `BUG_FOUND.md`.
8. The Lead writes `final-review.md` with coverage, rejected/deferred themes,
   cross-cutting classifications, and follow-up planning notes.

## Candidate Rules

Finder candidates may be broad. Each candidate should still include:

- candidate ID
- affected area
- summary
- suspected failure mode
- evidence or code path
- expected behavior
- observed or inferred behavior
- verification notes needed from the Verifier

Candidate states are:

- `new`
- `needs-info`
- `under-review`
- `accepted`
- `rejected`
- `duplicate`
- `deferred`

Terminal states are only:

- `accepted`
- `rejected`
- `duplicate`
- `deferred`

A run cannot close while any candidate remains in `new`, `needs-info`, or
`under-review`. If a candidate cannot be resolved before closure, the Lead must
move it to `deferred` and record the reason and follow-up condition in
`final-review.md`.

## Verification Rules

- Verifiers must not accept a candidate by repeating the Finder's claim.
- A Verifier must not verify candidates from the same actor's Finder work.
  The Lead records the actor assignment table in `RUN.md` before verification
  starts.
- Accepted bugs require independent confirmation from code, tests, docs,
  contracts, or a separately approved execution check.
- Static investigation is the default mode.
- `conan install`, `cmake`, `cmake --build`, `ctest`, smoke binaries, and
  wrapper scripts require a separate meeting decision and explicit approval
  before use.
- Approved execution checks must be recorded in `RUN.md`. Accepted bug cards
  must also record approved commands, commands actually run, and the reason any
  relevant approved command was not run.
- When runtime execution is not performed, the accepted bug record must say so.

## Accepted Bug Records

Severity values are:

- `Critical`: blocks a supported workflow, corrupts data, crashes, or violates a
  public contract in a high-impact path.
- `Major`: breaks a meaningful feature, lifecycle, platform path, or test
  contract but has a narrower blast radius.
- `Minor`: correctness, maintainability, diagnostics, or edge-case issue that
  should be fixed but does not block normal use.

`BUG_FOUND.md` records accepted bugs as detailed cards. Each card should include
severity, affected modules, summary, independent evidence, expected behavior,
actual or inferred behavior, verification mode, approved commands, commands
run, not-run reason, and follow-up fix-plan status.

## Run Closure

A run can close only when:

- every candidate is in a terminal state
- every accepted candidate is represented in `BUG_FOUND.md`
- duplicate merges are recorded in `final-review.md`
- deferred candidates include a reason and follow-up condition
- approved execution checks are recorded in `RUN.md`
- `final-review.md` is complete

## Fix Policy

Do not fix bugs inside the bug-hunt run. Use the accepted records to open a
separate strategy meeting and implementation plan. Emergency exceptions require
an explicit meeting decision before code changes begin.
