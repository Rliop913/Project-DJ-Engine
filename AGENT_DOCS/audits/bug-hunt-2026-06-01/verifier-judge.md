# Verifier - Judge

Role: Verifier
Model / effort: `gpt-5.4-mini xhigh`
Actor: McClintock
Verification scope: Cross-check Judge Finder candidates independently.

## Candidate Disposition

### JUDGE-CAND-001

- State: `accepted`
- Terminal: yes
- Independent evidence:
  `include/judge/Init/PDJE_Judge_Init.cpp:44-45,73-75`;
  `include/judge/InputParser/InputParser.hpp:36-39,70-72`;
  `include/judge/Loop/Match/Keyboard.cpp:12-16`;
  `include/judge/Loop/Match/Mouse.cpp:50-55`.
- Expected behavior: long rail/device names should be truncated or normalized
  in a way that still matches parser raw names.
- Actual or inferred behavior: `std::string(key.port_name, 255)` and
  `std::string(key.Device_Name, 255)` take the substring starting at position
  255, so stored rail keys become suffixes.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Major
- Notes for Lead: affects both MIDI and device rails.

### JUDGE-CAND-002

- State: `accepted`
- Terminal: yes
- Independent evidence:
  `include/judge/PDJE_Judge.cpp:46-56,63-76`;
  `include/judge/PDJE_Judge.hpp:27-31`.
- Expected behavior: a second `Start()` without `End()` should be rejected
  cleanly or be safe to repeat.
- Actual or inferred behavior: `Start()` has no running-state guard and can
  destroy a joinable thread during `loop.emplace(...)`.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Critical
- Notes for Lead: C ABI wrapper has a `running` guard; native C++ class does
  not.

### JUDGE-CAND-003

- State: `rejected`
- Terminal: yes
- Independent evidence:
  `include/judge/CPDJE_Judge.cpp:15-19,93-119,197-228,402-429`;
  `AGENT_DOCS/LIFECYCLES.md`.
- Expected behavior: if ownership retention were promised, attached handles
  would need to remain safe regardless of caller teardown.
- Actual or inferred behavior: wrapper attachments are borrowed handles under
  current lifecycle ordering.
- Rejection or deferral reason: contract precondition rather than a separate
  code defect.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: document lifetime requirements more explicitly if desired.
