# PDJE Agent Docs

Use this index to select the smallest source-backed document set for a task.

| Task | Read | Document ownership |
| --- | --- | --- |
| any repository work | [INVARIANTS.md](INVARIANTS.md) | global truth, scope, wiring, and ownership constraints |
| understand modules or public entrypoints | [ARCHITECTURE.md](ARCHITECTURE.md) | design intent, module relationships, and runtime data flow |
| locate code, CMake wiring, and verification | [CHANGE_MAP.md](CHANGE_MAP.md) | change-area routing |
| change lifecycle, state, data lines, or ABI behavior | [RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md) | caller-visible runtime contracts |
| select a preset or obtain verification approval | [VERIFY.md](VERIFY.md) | build truth, approval boundary, and preset selection |
| select stable targets, labels, or platform coverage | [TEST_MAP.md](TEST_MAP.md) | test and manual-target routing |
| change this documentation harness | [../DocsRule.md](../DocsRule.md) and [MAINTENANCE.md](MAINTENANCE.md) | writing standard and documentation ownership |
| run an approved build or executable check | [skills/pdje-build-verify/SKILL.md](skills/pdje-build-verify/SKILL.md) | verification flow selection and command reference |

`README.md` is the public landing page, `docs/` redirects to external
documentation, and `BluePrint_PDJE/` is archival material. They are not active
agent-documentation sources.
