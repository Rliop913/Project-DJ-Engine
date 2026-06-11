# Finder - Util

Role: Finder
Model / effort: `gpt-5.4-mini medium`
Actor: Chandrasekhar
Scope: Util

## Coverage

- `Status` / `Result`: inspected, no candidate filed.
- DB wrappers: inspected, no candidate filed.
- STFT/backend fallback: one candidate rejected as intentional probe behavior.
- Image/WebP: inspected, no candidate filed.
- AI/ONNX/Beat This: one accepted Beat This postprocessor issue.
- Util test wiring: existing known gaps not re-filed.

## Candidates

### UTIL-CAND-001 - Backend detector can differ from constructed backend

- State: `rejected`
- Affected area: STFT runtime backend probing.
- Cross-cutting tags: Util, STFT
- Suspected failure mode: caller sees `OPENCL` from detector while an instance
  falls back to `SERIAL`.
- Evidence or code path: `include/util/function/stft/STFT_Parallel.cpp`,
  `include/util/function/stft/detail/PDJE_Parallel_Runtime_Loader.cpp`.
- Expected behavior: detector would reflect instantiated backend if documented
  that way.
- Observed or inferred behavior: verifier found an intentional probe-vs-instance
  split; `active_backend()` is the post-construction answer.
- Verification notes: rejected by Popper.

### UTIL-CAND-002 - BeatThis short clips reject at reflect pad

- State: `rejected`
- Affected area: Beat This frontend.
- Cross-cutting tags: Util, AI
- Suspected failure mode: samples shorter than or equal to default pad throw.
- Evidence or code path:
  `include/util/ai/beat_this/BeatThis.hpp:19`,
  `include/util/ai/beat_this/BeatThisFrontend.cpp:185`.
- Expected behavior: short-clip support would need a documented fallback.
- Observed or inferred behavior: verifier found no public contract promising
  shorter-input support.
- Verification notes: rejected by Popper as a precondition/documentation topic.

### UTIL-CAND-003 - BeatThis dedupe can split connected peak clusters

- State: `accepted`
- Affected area: Beat This postprocessor.
- Cross-cutting tags: Util, AI
- Suspected failure mode: broad connected peak clusters can produce duplicate
  beat timestamps.
- Evidence or code path:
  `include/util/ai/beat_this/BeatThisPostprocessor.cpp:66-93`,
  `include/util/ai/beat_this/BeatThisPostprocessor.hpp:50`.
- Expected behavior: one connected peak cluster should collapse to one
  representative event.
- Observed or inferred behavior: running-mean threshold is not transitive.
- Verification notes: accepted by Popper.
