# PDJE Bug Hunt Run

Date: YYYY-MM-DD

Mode: static investigation first. No `conan install`, `cmake`,
`cmake --build`, `ctest`, smoke binary, or wrapper script command may run unless
a separate meeting closes that verification flow and the user approves it.

## Lead

- Name / agent:
- Model / effort: `gpt-5.5 xhigh`

## Scope

- Core:
- Input:
- Judge:
- Util:

## Assignments

Verifier must not verify candidates from the same actor's Finder work.

| Module | Finder actor | Verifier actor | Cross-check notes |
| --- | --- | --- | --- |
| Core |  |  |  |
| Input |  |  |  |
| Judge |  |  |  |
| Util |  |  |  |

## Constraints

- Investigation and verification only.
- Fixes move to separate implementation plans.
- Cross-cutting findings are classified by the Lead in final review.

## Approved Execution Checks

No execution checks are approved by default.

| Command | Approval source | Purpose | Status |
| --- | --- | --- | --- |
|  |  |  | not-approved | 

## Status

- Run state: `new`
- Finder phase:
- Verifier phase:
- Final review:

## Closure Criteria

The run can close only when:

- all candidates are in a terminal state: `accepted`, `rejected`, `duplicate`,
  or `deferred`
- all accepted candidates are represented in `BUG_FOUND.md`
- all duplicate merges are recorded in `final-review.md`
- all deferred candidates include a reason and follow-up condition
- approved execution checks are recorded above
- `final-review.md` is complete
