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
- SWIG is required only when `PDJE_SWIG_BUILD=ON`.
- `PDJE_DEVELOP_INPUT` is active on Linux and Windows and forced off on macOS.

## Codebase Shape

- `include/` contains both headers and implementation `.cpp` files.
- `PDJE`, `PDJE_Input`, `PDJE_JUDGE::JUDGE`, and `PDJE_UTIL` are the main
  native integration surfaces.
- `PDJE_UTIL` is active code, not roadmap-only material.
- Pointer-based data-line structs must be treated as non-owning and null-checked.

## Verification Expectations

- Unit test truth comes from `ctest --test-dir build -L unit`.
- Generated documentation should be produced through `DOCUMENT_GENERATOR.sh`,
  not by editing `docs/` directly.
