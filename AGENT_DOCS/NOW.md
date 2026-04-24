# PDJE Now

This file is the thin current-state control doc for `Project-DJ-Engine`.

## Snapshot

- Version file: `PDJE_VERSION` currently reports `0.8.7`
- Canonical agent Markdown docs now live under `AGENT_DOCS/`
- Generated HTML output lives under `docs/`

## Source Defaults That Matter

These come from `cmakes/Options.cmake`, not from the local `/build` cache.

- `PDJE_TEST=ON`
- `PDJE_DEV_TEST=OFF`
- `PDJE_DYNAMIC=OFF`
- `PDJE_SWIG_BUILD=OFF`
- `PDJE_DEVELOP_INPUT=ON` on Linux and Windows
- `PDJE_DEVELOP_INPUT=OFF` on macOS

## Local Checkout Caveat

The current local `/build` cache is configured differently from source
defaults and from the fixed verification baseline.

- `CMAKE_BUILD_TYPE=Release`
- `PDJE_TEST=ON`
- `PDJE_DEV_TEST=ON`
- `PDJE_DYNAMIC=ON`
- `PDJE_DEVELOP_INPUT=ON`
- The canonical shared build matrix now lives in root `CMakePresets.json`.
- That matrix fixes the repo-root `/build` directory.
- That matrix supports `Release` and `RelWithDebInfo` only.
- That matrix fixes `PDJE_DYNAMIC=ON` everywhere.
- That matrix enables `PDJE_TEST` and `PDJE_DEV_TEST` only in
  `RelWithDebInfo`.

Treat local binaries and `ctest` output as observations of this checkout, not
as documentation of the source defaults or the fixed verification baseline.

## Platform Status

- Linux: core, util, input, and judge supported
- Windows: core, util, input, and judge supported
- macOS: core and util build, input/judge gated off

## Current Watchouts

- `docs/` is generated output and should not be hand-edited.
- `BluePrint_PDJE/` is archive material only.
- `include/` contains both headers and implementation `.cpp` files.
- explicit C ABI unit test sources exist, but current module test lists do not
  wire them into `ctest --test-dir ./build -L unit`.
- `Judge_Init::SetInputLine()` currently requires `input_arena`, so MIDI-only
  input lines do not satisfy judge startup.
