# AGENTS.md - PDJE Agent Entry

This is the thin root entrypoint for coding agents. The canonical Markdown
control docs live under `AGENT_DOCS/`.

## Start Here

- [AGENT_DOCS/INDEX.md](AGENT_DOCS/INDEX.md)
- [AGENT_DOCS/NOW.md](AGENT_DOCS/NOW.md)
- [AGENT_DOCS/INVARIANTS.md](AGENT_DOCS/INVARIANTS.md)
- [AGENT_DOCS/VERIFY.md](AGENT_DOCS/VERIFY.md)
- [AGENT_DOCS/ARCHITECTURE.md](AGENT_DOCS/ARCHITECTURE.md)

## Critical Rules

- `AGENT_DOCS/` is the canonical Markdown doc surface.
- `document_sources/` is the Sphinx source tree.
- `docs/` is generated output.
- `BluePrint_PDJE/` is archive material only.
- Source defaults come from `cmakes/Options.cmake`, not from local build
  caches.
- Agents must ask the user which build/config or verification flow to use
  before running any `conan install`, `cmake`, `cmake --build`, `ctest`, or
  wrapper script command.
- Agents must not create or use build directories other than `./build` unless
  the user explicitly requests an exception.
- `include/` contains both headers and implementation `.cpp` files.

## Primary Entry Points

- `include/core/interface/PDJE_interface.hpp`
- `include/input/PDJE_Input.hpp`
- `include/judge/PDJE_Judge.hpp`
- `include/util/PDJE_Util.hpp`

## Fast Commands

```bash
ctest --test-dir build -L unit --output-on-failure
ctest --test-dir build -N -L unit
bash RunClangFormat.sh
```
