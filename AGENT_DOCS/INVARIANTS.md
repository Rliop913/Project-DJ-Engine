# PDJE Invariants

These rules are intended to stay stable unless the repository is deliberately
restructured.

## Documentation Ownership

- `AGENT_DOCS/` is the canonical Markdown control-doc location.
- Root `README.md` is the public landing page.
- Root `AGENTS.md` is the thin agent entrypoint.
- Root `ARCHITECTURE.md`, `PROJECT_STATE.md`, and `HOW_TO_VERIFY.md` are
  compatibility aliases, not canonical deep docs.
- `DECISIONS.md` is the short rationale ledger for these control-doc rules.
- `document_sources/` may still exist as external/generated-doc source, but it
  is not part of the required maintenance reading flow.
- `docs/` is generated output.
- `BluePrint_PDJE/` is archive material, not current source of truth.

## Build Truth

- Source defaults come from `cmakes/Options.cmake`.
- Local build caches can diverge from source defaults and must not overwrite
  them in docs.
- The checked-in `CMakePresets.json` is the canonical shared build matrix.
- Shared preset flows use the repo-root `/build` directory, written as
  `./build` in commands.
- Shared preset flows support exactly `Release` and `RelWithDebInfo`.
- Shared preset flows fix `PDJE_DYNAMIC=ON`.
- Shared preset flows allow `PDJE_TEST=ON` and `PDJE_DEV_TEST=ON` only in
  `RelWithDebInfo`.
- Shared preset flows fix compilers by platform: Windows=`cl`,
  Linux=`clang`, macOS=`clang` expecting AppleClang.
- SWIG is required only when `PDJE_SWIG_BUILD=ON`.
- `PDJE_DEVELOP_INPUT` is active on Linux and Windows and forced off on macOS.

## Codebase Shape

- `include/` contains both headers and implementation `.cpp` files.
- `PDJE`, `PDJE_Input`, `PDJE_JUDGE::JUDGE`, and `PDJE_UTIL` are the main
  native integration surfaces.
- `PDJE_UTIL` is active code, not roadmap-only material.
- Pointer-based data-line structs must be treated as non-owning and null-checked.

## Verification Expectations

- Unit test truth comes from `ctest --test-dir ./build -L unit`.
- Generated documentation should be produced through `DOCUMENT_GENERATOR.sh`,
  not by editing `docs/` directly.
