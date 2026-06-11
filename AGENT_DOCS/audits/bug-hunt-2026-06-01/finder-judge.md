# Finder - Judge

Role: Finder
Model / effort: `gpt-5.4-mini medium`
Actor: Kierkegaard
Scope: Judge

## Coverage

- Init prerequisites: inspected `Judge_Init` and `JUDGE::Start()`.
- Strict core/input line acceptance: known MIDI-only limitation not re-filed.
- Rails: inspected name capping, lookup, and parser paths.
- Note timing conversion: no new accepted issue.
- Loop/callback teardown: inspected repeated start/end lifecycle.
- C ABI wrapper: inspected attachment lifetime; rejected by verifier.

## Candidates

### JUDGE-CAND-001 - Long rail names keep suffix instead of prefix

- State: `accepted`
- Affected area: rail registration and lookup.
- Cross-cutting tags: Judge, Input names
- Suspected failure mode: long device or MIDI port names are registered under a
  suffix and later fail exact-name lookup.
- Evidence or code path: `include/judge/Init/PDJE_Judge_Init.cpp:35-77`,
  `include/judge/InputParser/InputParser.hpp`.
- Expected behavior: long names should be preserved, rejected, or capped
  consistently.
- Observed or inferred behavior: `std::string(existing, 255)` keeps text from
  offset 255.
- Verification notes: accepted by McClintock.

### JUDGE-CAND-002 - Native double Start can terminate process

- State: `accepted`
- Affected area: native judge lifecycle.
- Cross-cutting tags: Judge, lifecycle
- Suspected failure mode: calling `Start()` twice without `End()` destroys a
  joinable `std::thread`.
- Evidence or code path: `include/judge/PDJE_Judge.cpp:46-56`,
  `include/judge/PDJE_Judge.cpp:63-76`.
- Expected behavior: repeated `Start()` should fail safely or no-op.
- Observed or inferred behavior: no running-state guard before `loop.emplace`.
- Verification notes: accepted by McClintock.

### JUDGE-CAND-003 - C ABI attached handle lifetime is borrowed

- State: `rejected`
- Affected area: C ABI judge wrapper attachment lifetime.
- Cross-cutting tags: Judge, C ABI
- Suspected failure mode: destroyed attached handles become dangling pointers.
- Evidence or code path: `include/judge/CPDJE_Judge.cpp`.
- Expected behavior: if ownership retention were promised, attachments would
  remain safe after external teardown.
- Observed or inferred behavior: verifier found borrowed-handle semantics under
  current lifecycle ordering.
- Verification notes: rejected by McClintock as contract precondition, not a
  separate defect.
