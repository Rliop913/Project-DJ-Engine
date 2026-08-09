# PDJE Invariants

These constraints apply to every task unless the repository is deliberately
restructured.

## Truth And Scope

- Current source and checked-in CMake files outrank prose.
- `AGENT_DOCS/` is the canonical agent-facing Markdown surface; `AGENTS.md` is
  its entrypoint.
- Preserve unrelated worktree changes and keep edits within the requested
  subsystem.
- Treat local caches and generated build trees only as evidence of their own
  state, never as sources for repository defaults.

## Code And Wiring

- The project uses C++20.
- `include/` contains public headers, internal headers, and implementation
  `.cpp` files.
- Production and unit-test sources are explicitly listed under `cmakes/src/`
  and `cmakes/tests/`; adding a file does not make it part of a target.
- A source or test file change must preserve its owning explicit CMake list.
- `.clang-format` is the formatting authority for C and C++ sources.

## Ownership And Compatibility

- Public C++ declarations, C ABI declarations, data lines, callbacks, and
  versioned structures are compatibility surfaces.
- Borrowed pointers and views never acquire ownership implicitly and must not
  outlive their documented owner.
- Ownership, lifecycle, invalidation, and ABI details are defined in
  [RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md); do not infer them from wrapper
  convenience behavior.
