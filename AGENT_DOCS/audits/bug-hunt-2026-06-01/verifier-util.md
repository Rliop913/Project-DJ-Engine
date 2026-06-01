# Verifier - Util

Role: Verifier
Model / effort: `gpt-5.4-mini xhigh`
Actor: Popper
Verification scope: Cross-check Util Finder candidates independently.

## Candidate Disposition

### UTIL-CAND-001

- State: `rejected`
- Terminal: yes
- Independent evidence:
  `include/util/function/stft/STFT_Parallel.cpp:154,245`;
  `include/util/function/stft/detail/PDJE_Parallel_Runtime_Loader.cpp:7`;
  `include/util/function/stft/detail/OpenclBackend.cpp:44`;
  `AGENT_DOCS/UTIL_SYSTEM.md`.
- Expected behavior: the probe would describe the instantiated backend if that
  were the API contract.
- Actual or inferred behavior: `detect_available_backend()` is a runtime
  capability check; `active_backend()` is the actual post-construction answer.
- Rejection or deferral reason: intentional probe-vs-instance split.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: consider documenting the semantic difference.

### UTIL-CAND-002

- State: `rejected`
- Terminal: yes
- Independent evidence:
  `include/util/ai/beat_this/BeatThis.hpp:19`;
  `include/util/ai/beat_this/BeatThisFrontend.cpp:185,215`;
  `include/util/ai/beat_this/BeatThis.cpp:136`.
- Expected behavior: short-clip support would need a documented graceful
  fallback.
- Actual or inferred behavior: reflect-pad rejects `input.size() <= pad`, and
  no public contract promises shorter-input support.
- Rejection or deferral reason: deliberate precondition, not a code defect.
- If deferred, follow-up condition: n/a
- Accepted severity: n/a
- Notes for Lead: document minimum length or change padding policy if short
  snippets matter.

### UTIL-CAND-003

- State: `accepted`
- Terminal: yes
- Independent evidence:
  `include/util/ai/beat_this/BeatThisPostprocessor.hpp:50`;
  `include/util/ai/beat_this/BeatThisPostprocessor.cpp:79,163`.
- Expected behavior: a broad connected peak cluster should collapse to one
  representative.
- Actual or inferred behavior: comparing each peak to a running mean is not
  transitive, so `[0,1,2]` can split into multiple outputs.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Major
- Notes for Lead: switch to gap-to-previous or interval-based clustering if
  connected-peak grouping is intended.
