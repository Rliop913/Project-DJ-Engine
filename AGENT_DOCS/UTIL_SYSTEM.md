# PDJE Utility System

`PDJE_UTIL` is active shared infrastructure for result/status transport, DB
wrappers, STFT, runtime loading, waveform/WebP, and AI helpers.

## Surface

| Area | Public / owner paths |
| --- | --- |
| umbrella | `include/util/PDJE_Util.hpp` |
| common | `include/util/common/` for `Status`, `StatusCode`, `Result<T>` |
| DB | `include/util/db/` |
| STFT | `function/stft/STFT_Parallel.hpp`, `BackendLess.hpp`, `MelFilterBank.hpp`; internals in `function/stft/detail/` |
| image/WebP | `function/image/WaveformWebp.hpp`, `WebpWriter.hpp`; internals in `function/image/detail/` |
| AI | `include/util/ai/AI.hpp`, `include/util/ai/beat_this/BeatThis.hpp`, implementation in `beat_this/` |
| scalar/text | `include/util/function/scalar/`, `include/util/function/text/` |

## Contracts

- Utility changes can affect core, tests, and wrappers at once.
- Doctest coverage is under the `unit.util::...` prefix.
- Optional accelerated backend failure must preserve serial fallback behavior.
- No public-looking forwarding headers to `detail/`; callers include the stable
  public leaf or the canonical internal path.
- ONNX Runtime smoke coverage is registered separately as
  `unit.onnxruntime::smoke`.
- OpenCL-related tests cover fallback behavior as well as acceleration success.

## Change Points

- result/status: `include/util/common/`
- DB wrappers/backend concepts: `include/util/db/`
- STFT/runtime loaders: `include/util/function/stft/`
- waveform/WebP: `include/util/function/image/`
- ONNX Runtime facade and Beat This: `include/util/ai/`

## Verify

- Use [TEST_MAP.md](TEST_MAP.md): util DB/status, util signal/image, ONNX
  smoke, or full unit rows.
