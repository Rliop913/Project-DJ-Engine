# PDJE Utility System

This page covers the active utility surface behind `PDJE_UTIL`.

## Purpose

- provide reusable status and result transport
- provide database wrapper layers
- provide STFT, runtime backend loading, waveform, and WebP helpers
- expose a small umbrella include for common utility entrypoints

## Primary Entry Points

- `include/util/PDJE_Util.hpp`
- `include/util/common/`
- `include/util/db/`
- `include/util/function/stft/`
- `include/util/function/image/`

## Owning Paths

- `include/util/common/`
- `include/util/db/`
- `include/util/function/`
- `include/util/ai/`

## Current Surface

- `Status`, `StatusCode`, and `Result<T>` live under `include/util/common/`.
- database abstractions and compatible wrappers live under `include/util/db/`.
- stable STFT public surface is
  `include/util/function/stft/STFT_Parallel.hpp`,
  `include/util/function/stft/BackendLess.hpp`, and
  `include/util/function/stft/MelFilterBank.hpp`.
- stable image public surface is `include/util/function/image/WaveformWebp.hpp`
  and `include/util/function/image/WebpWriter.hpp`.
- STFT backend/runtime implementation is owned by
  `include/util/function/stft/detail/`.
- waveform/WebP internals are owned by `include/util/function/image/detail/`.
- scalar and text helpers live under `include/util/function/scalar/` and
  `include/util/function/text/`.

## Contracts / Invariants

- `PDJE_UTIL` is active code, not roadmap-only material.
- utility doctest coverage is registered under the `unit.core::...` prefix in
  the current checkout.
- backend-loading logic must keep serial fallback behavior intact when optional
  accelerated paths fail.
- utility code is shared infrastructure; changes here can affect core, tests,
  and wrappers at once.
- util does not keep public-looking forwarding headers for `detail/` ownership;
  callers should include either the stable public leaf or the canonical
  internal path directly.

## Common Change Points

- result/status behavior: `include/util/common/`
- DB wrappers and backend concepts: `include/util/db/`
- STFT and runtime loaders: `include/util/function/stft/`,
  especially `include/util/function/stft/detail/`
- waveform and WebP helpers: `include/util/function/image/`,
  especially `include/util/function/image/detail/`

## No Redirect Layer

- the legacy backend-loader compatibility directory is removed; STFT runtime
  loading is owned directly by `include/util/function/stft/detail/`.
- `include/util/function/image/` keeps only real public surface headers.
  Internal pipeline stage headers live only under
  `include/util/function/image/detail/`.
- `include/util/function/stft/` keeps only real public surface headers.
  Internal backend/helper headers live only under
  `include/util/function/stft/detail/`.

## Verify

- `ctest --test-dir ./build -R 'unit.core::(rocksdb|annoy|sqlite|util)' --output-on-failure`
- `ctest --test-dir ./build -R 'unit.core::(backendless|post process rgb mode|stft|mel filter bank|waveform|encode_waveform_webps|encode_webp|write_webp)' --output-on-failure`
- `ctest --test-dir ./build -L unit --output-on-failure`

## Known Traps

- utility behavior is spread across both umbrella headers and direct includes.
- the current test namespace does not advertise itself as `unit.util::...`.
- OpenCL-related tests verify fallback behavior as much as accelerated success.
