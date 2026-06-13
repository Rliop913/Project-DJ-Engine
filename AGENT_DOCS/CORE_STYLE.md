# PDJE Structural Style

Default placement and boundary rules. `include/core/` is the evidence base;
stronger local subsystem convention wins.

## Defaults

| Area | Rule | Anchors |
| --- | --- | --- |
| Scope | Use this for placement, naming, file pairing/splitting, headers, and boundaries; not for formatting. | `include/core/`, `PDJE_interface.hpp` |
| Placement | Place by stable responsibility. Keep `.hpp` and `.cpp` near the owning subsystem; orchestration dirs should own multiple collaborators. | `include/core/interface/`, `audioPlayer/` |
| Pairing | Default to one primary header plus one primary implementation for a named type/module. Keep private helpers in `.cpp` or anonymous namespaces. | `musicDB.hpp`, `editorObject.hpp`, `getdiff.cpp` |
| Splitting | Split large facades by behavior with operation-oriented `.cpp` names. Add directories only for real subdomains or type families. | `editorObject/addline.cpp`, `redo.cpp`, `TimeLine/` |
| Naming | Prefer local consistency over one global case style. Exported facades and integration seams get explicit names; directory-scoped runtime objects may stay shorter. | `PDJE_interface.hpp`, `audioPlayer.hpp`, `TimeLineDiffMachine.hpp` |
| Headers | Start headers with `#pragma once`; include direct dependencies; keep only small accessors, tiny helpers, and templates inline. | `TimeLine.hpp`, `dbRoot.hpp` |
| Sources | Move heavy algorithms, exception-heavy control flow, and explicit specializations into nearby `.cpp` files. | `editorObject/getdiff.cpp` |
| Public boundary | Do not add trivial forwarding headers to `detail/`. Public paths hold public declarations; internal callers use the canonical internal path. | `PDJE_interface.hpp`, `function/image/`, `function/stft/` |
| Behavior | Validate at boundaries, log where context is richest, use simple return channels, and null-check non-owning lines/handles. | `PDJE_interface.cpp`, `editorObject.cpp`, `audioPlayer.cpp` |

## Legacy

- Mixed older naming and comments are evidence, not automatic defaults.
- Prefer the table above unless neighboring code has a stronger local pattern.
- Keep intentional deviations local and explicit.
