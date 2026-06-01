# Finder - Core

Role: Finder
Model / effort: `gpt-5.4-mini medium`
Actor: Confucius
Scope: Core

## Coverage

- Facade / playback: inspected search/player/data-line overlap.
- Editor bridge: inspected push-to-root DB and track-title sanitization.
- DB/search: no accepted new DB/search candidate.
- Data-line behavior: overlap with Judge/Input candidates.
- C ABI or cross-cutting findings: duplicates classified in final review.

## Candidates

### CORE-CAND-001 - Long judge rail names keep suffix instead of prefix

- State: `duplicate`
- Affected area: judge rail registration, surfaced during Core overlap.
- Cross-cutting tags: Judge, rail contract
- Suspected failure mode: names longer than 255 bytes are keyed by suffix.
- Evidence or code path: `include/judge/Init/PDJE_Judge_Init.cpp`.
- Expected behavior: oversize names should be rejected or consistently capped.
- Observed or inferred behavior: `std::string(existing, 255)` starts at offset
  255.
- Verification notes: duplicate of `JUDGE-CAND-001`.

### CORE-CAND-002 - Editor track title sanitizer failure is dereferenced

- State: `accepted`
- Affected area: editor push path.
- Cross-cutting tags: Core, editor
- Suspected failure mode: overlong track title throws `std::bad_optional_access`
  instead of returning `false`.
- Evidence or code path:
  `include/core/MainObjects/editorObject/editorObject.cpp:26-32`,
  `include/core/MainObjects/editorObject/editorObject.cpp:180-181`,
  `include/global/DataLines/fileNameSanitizer.cpp:7-12`.
- Expected behavior: invalid titles should be rejected cleanly before trackdata
  construction.
- Observed or inferred behavior: `safeTitle.value()` is called even after
  `sanitizeFileName()` returns `nullopt`.
- Verification notes: accepted by Euler and merged by Lead Bohr as `BUG-004`.

### CORE-CAND-003 - Judge double Start has no running-state guard

- State: `duplicate`
- Affected area: judge lifecycle, surfaced during Core overlap.
- Cross-cutting tags: Judge, lifecycle
- Suspected failure mode: repeated native `Start()` can overwrite a joinable
  thread.
- Evidence or code path: `include/judge/PDJE_Judge.cpp:46-56`.
- Expected behavior: repeated `Start()` should fail safely or no-op.
- Observed or inferred behavior: `loop.emplace(...)` has no existing-thread
  guard.
- Verification notes: duplicate of `JUDGE-CAND-002`.

### CORE-CAND-004 - Input Config mutates MIDI state before state gate

- State: `duplicate`
- Affected area: input lifecycle, surfaced during Core overlap.
- Cross-cutting tags: Input, lifecycle
- Suspected failure mode: a rejected config call can leave stale MIDI state.
- Evidence or code path: `include/input/PDJE_Input.cpp:58-63`.
- Expected behavior: failed config should leave state untouched.
- Observed or inferred behavior: MIDI fields are assigned before `CanConfig()`.
- Verification notes: duplicate of accepted `INPUT-CAND-003` / `BUG-007`.
