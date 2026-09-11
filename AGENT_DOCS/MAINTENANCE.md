# PDJE Agent Harness Maintenance

Apply the repository prose standard in [../DocsRule.md](../DocsRule.md). This
page owns where agent information belongs and when it must be reviewed; it does
not restate the writing standard.

## Document Ownership And Update Triggers

| Document | Sole responsibility | Review when |
| --- | --- | --- |
| `INDEX.md` | task-to-document routing and harness boundaries | an active document, subsystem, or top-level boundary changes |
| `INVARIANTS.md` | global truth, scope, explicit wiring, and ownership constraints | repository-wide constraints or source-list conventions change |
| `ARCHITECTURE.md` | design intent, module relations, data flow, and public entrypoints | module boundaries or public C++/C entrypoints change |
| `CHANGE_MAP.md` | change-area routing to code, CMake, and verification | owning paths, source lists, or verification routes change |
| `RUNTIME_CONTRACTS.md` | caller-visible ownership, lifecycle, state, invalidation, and ABI compatibility | data lines, state transitions, teardown, callbacks, or public behavior change |
| `VERIFY.md` | approval boundary, build truth, and preset selection | options, presets, compilers, build directories, bootstrap, or CI assumptions change |
| `TEST_MAP.md` | stable targets, CTest labels, manual targets, and platform gates | test wiring, labels, targets, or gates change |
| `AGENT_DOCS/skills/pdje-build-verify/SKILL.md` and its reference | approved-flow selection and exact command forms | verification policy or platform commands change |

If a fact already has an owner in this table, link to that document instead of
copying it. Preserve caller-visible failure and compatibility constraints, but
remove implementation walkthroughs that merely reproduce control flow.

## Static Review

- Compare every retained contract and path with current headers,
  implementations, and checked-in CMake files.
- Confirm every active document is linked from `INDEX.md` and every relative
  Markdown link and referenced repository path exists.
- Search for removed implementation sections and duplicated policy wording.
- Run `git diff --check` and inspect the final documentation diff.
- For documentation-only changes, do not configure, build, or run tests merely
  to validate Markdown.
