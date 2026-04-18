# PDJE Decisions

This is the compact rationale ledger for repository-level choices that are easy
to accidentally "clean up" into breakage.

| Decision | Current Rule | Why | If This Changes, Also Update |
| --- | --- | --- | --- |
| Markdown control-doc home | `AGENT_DOCS/` is canonical | keeps agent-facing control docs separate from generated and public docs | `INDEX.md`, `AGENTS.md`, `README.md` |
| Generated-doc boundary | `docs/` is generated output and not a maintenance doc source | keeps maintenance edits away from generated HTML | `INVARIANTS.md`, `VERIFY.md`, doc generation notes |
| Blueprint vault boundary | `BluePrint_PDJE/` is archive only | historical notes still help search, but are not source of truth | archive landing pages and cross-links |
| Source defaults | defaults come from `cmakes/Options.cmake` | local `build/` caches can drift | `NOW.md`, `VERIFY.md`, `README.md` |
| Input platform gate | `PDJE_DEVELOP_INPUT=ON` on Linux and Windows, forced `OFF` on macOS | current source tree only supports input and judge on Linux and Windows | `NOW.md`, `INVARIANTS.md`, `README.md` |
| SWIG requirement | SWIG is optional unless `PDJE_SWIG_BUILD=ON` | most builds do not need wrapper generation | `README.md`, `INVARIANTS.md`, Sphinx build docs |
| `include/` layout | headers and implementation `.cpp` files both live under `include/` | this repo is library-first and not split into a separate `src/` tree | `ARCHITECTURE.md`, onboarding docs, file-path examples |
| Unit test wiring | module unit binaries use explicit source lists under `cmakes/tests/units/` | current test coverage is shaped by CMake lists, not by auto-globbing | `TEST_MAP.md`, `CHANGE_MAP.md`, test maintenance work |
