# PDJE Agent Harness Maintenance

The harness should help an agent find and protect repository truth without
becoming a second implementation of the project.

## Source-Of-Truth Order

1. Public headers and implementation code define current runtime behavior.
2. Checked-in CMake files define options, targets, source membership, platform
   gates, and tests.
3. Executable tests support claims only when their sources are wired into a
   target and that target was run.
4. `AGENT_DOCS/` explains stable boundaries and routes agents to items 1-3.
5. `README.md` is a public landing page; local build caches, `docs/` redirects,
   and `BluePrint_PDJE/` are not control truth.

When code and prose disagree, correct the prose or explicitly fix the code; do
not preserve the contradiction as undocumented intent.

## What Belongs Here

Keep:

- project purpose and module ownership;
- public entrypoints and C/C++ boundary locations;
- non-obvious ownership, lifecycle, state, and platform constraints;
- exact source-of-truth files for build/test wiring;
- known coverage gaps that materially change what an agent can claim;
- stable verification labels and approval boundaries.

Avoid:

- exhaustive symbol or source-file inventories already owned by CMake;
- local cache values, absolute machine paths, or transient test counts;
- speculative plans presented as current contracts;
- agent model names, effort levels, staffing roles, or meeting rituals;
- duplicated rules whose only owner should be another harness document;
- archived behavior copied from `BluePrint_PDJE/`.

## Update Triggers

| Repository change | Harness review |
| --- | --- |
| option, preset, compiler, build directory, or bootstrap script | `INVARIANTS.md`, `VERIFY.md`, build skill references |
| target, platform gate, or production source list | `ARCHITECTURE.md`, `CHANGE_MAP.md` |
| unit target, label, test source membership, or dev target | `TEST_MAP.md`, `VERIFY.md` |
| public C++/C entrypoint or namespace | `ARCHITECTURE.md`, `CHANGE_MAP.md` |
| data line, state transition, ownership, or teardown behavior | `RUNTIME_CONTRACTS.md` |
| new subsystem or top-level directory | `INDEX.md`, `ARCHITECTURE.md`, `CHANGE_MAP.md` |
| documentation ownership/boundary | `AGENTS.md`, `INDEX.md`, `INVARIANTS.md` |

## Review Checklist

- Read the referenced source; do not update prose from memory.
- Keep every active harness document linked from `INDEX.md`.
- Resolve all relative Markdown links and verify referenced repository paths.
- Search removed names across `AGENTS.md` and `AGENT_DOCS/`.
- Ensure test claims match `cmakes/tests/UnitTestRoot.cmake` and the owning
  `Module_*.cmake` list.
- Ensure build claims match `cmakes/Options.cmake` and `CMakePresets.json`,
  not `CMakeCache.txt`.
- Run `git diff --check` and inspect the final documentation diff.
- For documentation-only changes, do not configure or build merely to validate
  Markdown. Run code verification only when the documented command or changed
  code requires it and the user approves the flow.

If a document mostly repeats another one, merge the unique information into the
authoritative page and remove the duplicate from the index.
