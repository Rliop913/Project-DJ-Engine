# PDJE Now

This file is the thin current-state control doc for `Project-DJ-Engine`.

## Snapshot

- Version file: `PDJE_VERSION` currently reports `0.8.7`
- Canonical agent Markdown docs now live under `AGENT_DOCS/`
- Public documentation lives outside this checkout; `docs/` contains only
  redirecting HTML

## Source Defaults That Matter

These come from `cmakes/Options.cmake`, not from the local `/build` cache.

- `PDJE_TEST` defaults on.
- `PDJE_DEV_TEST` defaults off.
- `PDJE_DYNAMIC` defaults off.
- `PDJE_SWIG_BUILD` defaults off.
- `PDJE_DEVELOP_INPUT` defaults on for Linux and Windows.
- `PDJE_DEVELOP_INPUT` is forced off on macOS.

## Local Checkout Caveat

The repo-root `./build` cache is mutable local state. Inspect it separately
before relying on cached build options:

```bash
cmake -LA -N ./build | rg '^PDJE_|^CMAKE_BUILD_TYPE'
```

The canonical shared build matrix now lives in root `CMakePresets.json`.
That matrix fixes the repo-root `./build` directory, supports only `Release`
and `RelWithDebInfo`, fixes `PDJE_DYNAMIC=ON` everywhere, and enables
`PDJE_TEST` plus `PDJE_DEV_TEST` only in `RelWithDebInfo`.

Treat local binaries and `ctest` output as observations of this checkout, not
as documentation of the source defaults or the fixed verification baseline.

## Platform Status

- Linux: core, util, input, and judge supported
- Windows: core, util, input, and judge supported
- macOS: core and util build, input/judge gated off

## Current Watchouts

- `docs/` contains only redirecting HTML for the external docs site; do not
  treat it as canonical documentation content.
- `BluePrint_PDJE/` is archive material only.
- `include/` contains both headers and implementation `.cpp` files.
- explicit C ABI unit test sources exist, but current module test lists do not
  wire them into `ctest --test-dir ./build -L unit`.
- `Judge_Init::SetInputLine()` currently requires `input_arena`, so MIDI-only
  input lines do not satisfy judge startup.
