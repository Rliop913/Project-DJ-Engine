# PDJE Bug Hunt Final Review

Date: 2026-06-01

## Coverage Summary

- Core: editor bridge and cross-module lifecycle overlap reviewed; one Core-only
  candidate accepted, three candidates classified as duplicates.
- Input: Windows raw input, C ABI snapshot export, and config lifecycle
  reviewed; all Input candidates accepted after reopened verification.
- Judge: rails, lifecycle, and C ABI attachment paths reviewed; two bugs
  accepted and one candidate rejected.
- Util: STFT backend probing and BeatThis frontend/postprocessing reviewed; one
  bug accepted and two candidates rejected.

## Accepted Bugs

- See `BUG_FOUND.md`.

## Rejected Themes

- Judge C ABI attached-handle lifetime was rejected as a borrowed-handle
  lifecycle precondition under the current contract.
- STFT backend probe mismatch was rejected as an intentional probe-vs-instance
  split; `active_backend()` is the constructed instance answer.
- BeatThis short-input rejection was rejected as a precondition/documentation
  topic because no current public contract promises short-clip support.

## Deferred Themes

- None after reopened verifier and Lead pass.

## Deferred Candidates

| Candidate ID | Reason | Follow-up condition |
| --- | --- | --- |
| none | n/a | n/a |

## Duplicate Merges

This is the only canonical duplicate tracking location for the run.

| Canonical candidate | Merged duplicate candidates | Merge reason |
| --- | --- | --- |
| `JUDGE-CAND-001` | `CORE-CAND-001` | Same wrong long-name truncation in judge rail registration. |
| `JUDGE-CAND-002` | `CORE-CAND-003` | Same native judge double-start lifecycle hazard. |
| `INPUT-CAND-003` | `CORE-CAND-004` | Same input `Config()` MIDI mutation before state gate; canonical candidate accepted as `BUG-007`. |

## Cross-Cutting Classifications

- C ABI: `INPUT-CAND-002` accepted as Input C ABI snapshot export; known C ABI
  unit wiring gaps were not re-filed.
- Lifecycle: `JUDGE-CAND-002` accepted as Judge lifecycle; `INPUT-CAND-003`
  accepted as native Input lifecycle.
- Data/name contracts: `JUDGE-CAND-001` accepted as Judge rail name identity
  contract affecting Input-provided names.
- AI postprocessing: `UTIL-CAND-003` accepted as Util BeatThis behavior.
- Windows runtime: `INPUT-CAND-001` accepted as missing HID payload handling.

## Follow-Up Planning Notes

- Open a Judge strategy meeting for `BUG-001` and `BUG-002`.
- Open a Util strategy meeting for `BUG-003`.
- Open Core/Input strategy meetings for `BUG-004` through `BUG-007`.

## Run Closure Check

Terminal states are `accepted`, `rejected`, `duplicate`, and `deferred`.

| State | Count |
| --- | --- |
| accepted | 7 |
| rejected | 3 |
| duplicate | 3 |
| deferred | 0 |
| non-terminal | 0 |

- All candidates terminal: yes
- Accepted bugs copied to `BUG_FOUND.md`: yes
- Duplicate merges recorded: yes
- Deferred candidates include reason and follow-up condition: yes
- Approved execution checks reflected in accepted cards: yes
- Final review complete: yes
