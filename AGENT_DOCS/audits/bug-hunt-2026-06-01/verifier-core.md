# Verifier - Core

Role: Verifier
Model / effort: `gpt-5.4-mini xhigh`
Verification scope: Cross-check assigned Finder candidates independently.

## Candidate Disposition

### CORE-CAND-001

- State: `duplicate`
- Terminal: yes
- Independent evidence: accepted independently as `JUDGE-CAND-001`.
- Expected behavior: see `verifier-judge.md`.
- Actual or inferred behavior: see `verifier-judge.md`.
- Rejection or deferral reason: duplicate of `JUDGE-CAND-001`.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: canonical record is Judge.

### CORE-CAND-002

- State: `accepted`
- Terminal: yes
- Independent evidence:
  Euler checked `include/global/DataLines/fileNameSanitizer.cpp:9`,
  `include/core/MainObjects/editorObject/editorObject.cpp:26,32`,
  `include/core/MainObjects/editorObject/render.cpp:13`, and
  `include/core/db/trackDB.cpp:32`.
- Expected behavior: sanitizer failure should be returned cleanly before
  dereferencing.
- Actual or inferred behavior: `safeTitle.value()` is called after a logged
  sanitizer failure; overlong titles can throw `std::bad_optional_access`.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Major
- Notes for Lead: accepted as `BUG-004` after reopened verifier/Lead pass.

### CORE-CAND-003

- State: `duplicate`
- Terminal: yes
- Independent evidence: accepted independently as `JUDGE-CAND-002`.
- Expected behavior: see `verifier-judge.md`.
- Actual or inferred behavior: see `verifier-judge.md`.
- Rejection or deferral reason: duplicate of `JUDGE-CAND-002`.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: canonical record is Judge.

### CORE-CAND-004

- State: `duplicate`
- Terminal: yes
- Independent evidence: same candidate family as `INPUT-CAND-003`.
- Expected behavior: invalid-state config should not mutate runtime state.
- Actual or inferred behavior: MIDI state is assigned before the state gate.
- Rejection or deferral reason: duplicate of accepted `INPUT-CAND-003`.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: canonical record is Input.
