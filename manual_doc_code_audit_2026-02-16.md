# Historical Audit: PDJE Manual Docs vs Codebase (2026-02-16)

Audit date: 2026-02-16  
Status: Historical review note, not the current root-document baseline  
Scope: Manual documentation sources only (`document_sources/*.rst`, `document_sources/conf.py`)  
Excluded: `document_sources/api/**`, `document_sources/xml/**`, `docs/**`

This file captures a point-in-time audit of the Sphinx manual docs. Use
`README.md`, `AGENTS.md`, and `AGENT_DOCS/INDEX.md` for the current canonical
Markdown baseline.

## Section 1: P0 Issues (Immediate Fix Required)

| ID | Severity | Doc Reference | Documented Content | Code Evidence | Recommended Fix Text |
|---|---|---|---|---|---|
| P0-001 | P0 | `document_sources/Input_Engine.rst:73` | `PDJE_Input::Kill` uses `:project: Project_Input` | `document_sources/conf.py:25` defines Breathe project key as `Project_DJ_Engine` | Change to `:project: Project_DJ_Engine` so symbol resolution is consistent. |
| P0-002 | P0 | `document_sources/Data_Lines.rst:75`, `document_sources/Data_Lines.rst:99`, `document_sources/Data_Lines.rst:125` | `core_line.used_frame` field exists | `include/global/DataLines/PDJE_Core_DataLine.hpp:12` has only `nowCursor`, `maxCursor`, `preRenderedData`, `syncD` | Replace `used_frame` references with `core_line.syncD->load().consumed_frames` (and mention `pre_calculated_unused_frames` if needed). |
| P0-003 | P0 | `document_sources/Data_Lines.rst:174`, `document_sources/Data_Lines.rst:177` | `input_arena->Get()` and `id_name_conv` are available | Linux/Windows input transfer expose `Receive()` + `datas`: `include/input/IPC/memory/linux/Input_Transfer.hpp:11`, `include/input/IPC/memory/windows/Input_Transfer.hpp:60` | Update example to: call `dline.input_arena->Receive();` then iterate `dline.input_arena->datas`; remove `id_name_conv` usage. |
| P0-004 | P0 | `document_sources/Core_Engine.rst:640` | `editor->Go<...>(branchName, branch_head_oid)` in C++ | Actual C++ API is single commit OID: `include/core/MainObjects/editorObject/editorObject.hpp:371` | Change C++ example to `editor->Go<EDIT_ARG_NOTE>(commitOID)` and same for MIX/KV/MUSIC variants. |
| P0-005 | P0 | `document_sources/Core_Engine.rst:1076`, `document_sources/Core_Engine.rst:1083` | Log JSON schema documented as `BRANCH`/`COMMIT` | Real schema uses `LINE`/`LOGS`: `include/core/editor/TimeLine/TimeLine.hpp:132`, `include/core/editor/TimeLine/TimeLine.hpp:133` | Replace schema documentation keys with `LINE` (line pointers) and `LOGS` (commit graph metadata). |
| P0-006 | P0 | `document_sources/Editor_Format.rst:99`, `document_sources/Editor_Format.rst:102`, `document_sources/Editor_Format.rst:109`, `document_sources/Editor_Format.rst:110` | `DetailEnum` numeric map listed as `TRIM(7)/FADER(8)`, `timeStretch(6)`, `SPIN(12)/PITCH(13)/REV(14)`, `SCRATCH(15)` | Cap'n Proto source of truth is `trim @5`, `fader @6`, `timeStretch @7`, `spin @8`, `pitch @9`, `rev @10`, `scratch @11`: `third_party/Capnp/MixBinary.capnp:31` | Correct all enum numbers to match `MixBinary.capnp` definitions. |
| P0-007 | P0 | `document_sources/Core_Engine.rst:108`, `document_sources/Core_Engine.rst:111`, `document_sources/Core_Engine.rst:114` | C++ playback example resets player and then calls `Activate/Deactivate` through `engine->player` | `ResetPlayer()` nulls player: `include/core/interface/PDJE_interface.hpp:89` | Reorder snippet: `Activate/Deactivate` first, call `ResetPlayer()` last (or reacquire/re-init player before calling). |

## Section 2: P1 Issues (Accuracy Degradation)

| ID | Severity | Doc Reference | Documented Content | Code Evidence | Recommended Fix Text |
|---|---|---|---|---|---|
| P1-001 | P1 | `document_sources/Judge_Engine.rst:13` | Init header path shown as `include/judge/Init/Judge_Init.hpp` | Actual header is `include/judge/Init/PDJE_Judge_Init.hpp:1` | Replace the path string with the real header path. |
| P1-002 | P1 | `document_sources/index.rst:112`, `document_sources/Util_Engine.rst:4` | Utility engine is presented as available/implemented (`Available on PDJE 1.2.0`) | No `PDJE_MODULE_UTILITY` target in build graph; only `PDJE`, `PDJE_MODULE_INPUT`, `PDJE_MODULE_JUDGE`: `CMakeLists.txt:236`, `CMakeLists.txt:247` | Rephrase as roadmap/placeholder status (not currently shipped as active module target). |
| P1-003 | P1 | `document_sources/index.rst:131` | Milestone claims mac input module implemented (`0.9.0`) | No `include/input/apple` dir in repo; CMake references path but no implementation exists (`cmakes/includeDIRS.cmake:131`) | Mark mac input as planned/incomplete unless implementation is added. |
| P1-004 | P1 | `document_sources/Core_Engine.rst:506` | C# snippet calls `engine.DESTROY_PROJECT()` | `DESTROY_PROJECT` belongs to `editorObject`: `include/core/MainObjects/editorObject/editorObject.hpp:249` | Change to `editor.DESTROY_PROJECT()` in this section. |
| P1-005 | P1 | `document_sources/index.rst:59` | Says modules are "independent, dynamically loadable" | Default build is static (`PDJE_DYNAMIC=OFF`): `CMakeLists.txt:52`; module libs are ordinary libs, not runtime plugin loader model | Replace with "modular components, optionally built as shared libraries when `PDJE_DYNAMIC=ON`". |

## Section 3: P2 Issues (Consistency / Clarity)

| ID | Severity | Doc Reference | Documented Content | Code Evidence | Recommended Fix Text |
|---|---|---|---|---|---|
| P2-001 | P2 | `document_sources/index.rst:4`, `document_sources/index.rst:94` | Top summary says "millisecond-level" while same doc later says "microsecond-level" | Judge and sync logic use microsecond domain (`audioSyncData.microsecond`): `include/global/PDJE_SYNC_CORE.hpp:7` | Standardize wording to "microsecond-level judgment precision" with latency caveat. |
| P2-002 | P2 | `document_sources/index.rst:150` | Linux low latency claim includes "epoll + RT mode" | `epoll` is active, RT scheduling block is currently commented out: `include/input/DefaultDevs/linux/InputCore.cpp:85` | Rephrase to "epoll-based low-latency path; RT scheduling hooks exist but are not currently enabled by default." |
| P2-003 | P2 | `document_sources/FX_ARGS.rst:41` | OCS filter key documented as `OcsFilterHighLowSW` | Actual key is `OCSFilterHighLowSW`: `include/core/audioRender/ManualMix/ManualFausts/ocsFilterMan.hpp:14` | Fix case-sensitive key name to `OCSFilterHighLowSW`. |
| P2-004 | P2 | `document_sources/Editor_Format.rst:206` | RST inline link syntax is malformed (`See: :`Serato Beat Grid ...`) | RST syntax check by inspection; this pattern is invalid | Change to valid RST link format: ``See: `Serato Beat Grid <...>`_``. |
| P2-005 | P2 | `document_sources/index.rst:156` | License line says "LGPLv2.1 (by SoundTouch License)" | Project license file is LGPL 2.1: `LICENSE:1` | Rephrase to "License: LGPLv2.1" and separate third-party license notes elsewhere. |

## Section 4: Patch-Ready Recommended Wording

1. For `Data_Lines.rst` core field section:
`PDJE_CORE_DATA_LINE` exposes `nowCursor`, `maxCursor`, `preRenderedData`, and `syncD`. Playback progress is available from `syncD->load().consumed_frames`.

2. For `Data_Lines.rst` input section:
Use `dline.input_arena->Receive();` and iterate `dline.input_arena->datas`.

3. For `Judge_Engine.rst` init class path:
`Judge_Init` (`include/judge/Init/PDJE_Judge_Init.hpp`).

4. For `Core_Engine.rst` time-travel C++ snippet:
Use `editor->Go<EDIT_ARG_NOTE>(commitOID);` and parse logs from `LINE`/`LOGS`.

5. For `Core_Engine.rst` playback snippet:
Initialize player → Activate → Deactivate → ResetPlayer (last).

6. For `Input_Engine.rst` doxygen tag:
Set `:project: Project_DJ_Engine` for `PDJE_Input::Kill()`.

7. For `Editor_Format.rst` DetailEnum number map:
`trim(5)`, `fader(6)`, `timeStretch(7)`, `spin(8)`, `pitch(9)`, `rev(10)`, `scratch(11)`.

8. For utility status text (`index.rst`, `Util_Engine.rst`):
"Utility capabilities are planned/roadmap items; not currently shipped as an active PDJE module target in this repository."

9. For module architecture sentence (`index.rst`):
"PDJE is organized into modular components; libraries can be built shared when `PDJE_DYNAMIC=ON`, static by default."

10. For timing precision sentence (`index.rst`):
"PDJE uses microsecond-resolution synchronization for judgment timing; audio callback cadence still affects playback latency characteristics."

## Section 5: File-by-File Fix Priority

1. `document_sources/Data_Lines.rst`  
Reason: Contains multiple hard API mismatches (`used_frame`, `Get()`, `id_name_conv`) that directly break consumer code.

2. `document_sources/Core_Engine.rst`  
Reason: Contains invalid C++ usage order and stale time-travel API/schema examples.

3. `document_sources/Editor_Format.rst`  
Reason: Enum numeric mismatch can cause wrong serialized values and interop errors.

4. `document_sources/Input_Engine.rst`  
Reason: Doxygen project typo can break reference generation for `Kill()`.

5. `document_sources/index.rst` + `document_sources/Util_Engine.rst`  
Reason: Architectural/status claims are outdated and can mislead roadmap expectations.

6. `document_sources/FX_ARGS.rst`  
Reason: Case-sensitive key typo causes runtime confusion in arg setter usage.

7. `document_sources/Getting Started.rst`, `document_sources/conf.py`  
Reason: No material code mismatch found in this audit scope.

## Scenario Coverage Result

- Scenario 1 (Reference integrity): **Fail** (1 confirmed doxygen project tag mismatch).
- Scenario 2 (Structural integrity): **Fail** (`PDJE_CORE_DATA_LINE`, input transfer fields/examples mismatch).
- Scenario 3 (Signature integrity): **Fail** (`Go<...>` C++ signature mismatch, playback misuse order).
- Scenario 4 (Schema integrity): **Fail** (`DetailEnum` numeric table mismatch, stale log JSON key schema).
- Scenario 5 (Platform/module claims): **Fail** (utility/mac/RT-mode wording not aligned with current code status).
