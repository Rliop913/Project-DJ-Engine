# PDJE Structural Style

This document generalizes the repeated authoring patterns observed in
`include/core/` into the default structural rules for repository code.
`include/core/` remains the evidence base, so each section keeps one
representative anchor from that tree.

## Scope

- Use these rules as the default for placement, naming, file pairing, file
  splitting, header/source boundaries, directory creation, and boundary
  behavior when no subsystem-specific rule says otherwise. Representative
  anchor: `include/core/`
- Treat this as a structural style guide, not a formatter or whitespace spec.
  Representative anchor: `include/core/interface/PDJE_interface.hpp`
- If a local slice has an intentional convention of its own, follow the local
  convention and treat this document as the fallback rule set. Representative
  anchor: `include/core/editor/TimeLine/DiffMachine/TimeLineDiffMachine.cpp`

## Placement Rules

- Place files by stable responsibility first; top-level directories should map
  to subsystem ownership or long-lived roles, not temporary implementation
  detail. Representative anchor: `include/core/`
- Keep declarations and implementations physically close to the subsystem that
  owns them; this repository does not assume a separate `src/` mirror.
  Representative anchor: `include/core/interface/PDJE_interface.cpp`
- Reserve orchestration-focused locations for objects that own or coordinate
  several collaborators, not for isolated helpers. Representative anchor:
  `include/core/MainObjects/audioPlayer/audioPlayer.hpp`

## File Pairing Rules

- Default to one primary header and one primary implementation file for a named
  type or module. Representative anchor: `include/core/db/musicDB.hpp`
- Keep the public declaration surface discoverable from one primary header even
  when the implementation later fans out. Representative anchor:
  `include/core/MainObjects/editorObject/editorObject.hpp`
- Keep private helper logic in implementation files or anonymous namespaces
  before introducing extra headers. Representative anchor:
  `include/core/MainObjects/editorObject/getdiff.cpp`

## File Splitting Rules

- Split implementation by behavior when one facade grows beyond a single
  coherent implementation unit. Representative anchor:
  `include/core/MainObjects/editorObject/`
- Prefer operation-oriented `.cpp` names when multiple files implement one
  public type, so the split remains searchable by action and intent.
  Representative anchor: `include/core/MainObjects/editorObject/addline.cpp`
- Do not create extra directory depth only to separate methods; split files
  first, then create a directory only if a real subdomain emerges.
  Representative anchor: `include/core/MainObjects/editorObject/redo.cpp`

## Directory Creation Rules

- Add a new directory only when the code has become a stable subdomain,
  pipeline, or type family with more than one likely participant.
  Representative anchor: `include/core/editor/TimeLine/`
- Use directories to group repeated families that benefit from shared
  discoverability, such as translators, diff stages, or effect types.
  Representative anchor: `include/core/audioRender/MixMachine/MixTypes/`
- If a new file mainly extends an existing family, place it beside that family
  before introducing another layer of nesting. Representative anchor:
  `include/core/db/Capnp/Translators/`

## Naming Rules

- Prefer naming consistency within a slice over forcing one repository-wide
  case style. Representative anchor: `include/core/`
- Use stronger, more explicit names for exported facades, umbrella surfaces,
  and integration seams. Representative anchor:
  `include/core/interface/PDJE_interface.hpp`
- Use lighter concrete names for runtime objects when the directory already
  supplies the needed context. Representative anchor:
  `include/core/MainObjects/audioPlayer/audioPlayer.hpp`
- Keep family names visually grouped across related files and types so a reader
  can find the whole cluster by search. Representative anchor:
  `include/core/editor/TimeLine/DiffMachine/TimeLineDiffMachine.hpp`

## Header And Source Rules

- Start headers with `#pragma once`. Representative anchor:
  `include/core/editor/TimeLine/TimeLine.hpp`
- Keep short inline accessors, tiny helpers, and template bodies in headers
  only when they are part of the direct type surface or template requirements.
  Representative anchor: `include/core/db/dbRoot.hpp`
- Move heavy algorithms, exception-heavy control flow, and explicit
  specializations into `.cpp` files near the consuming implementation.
  Representative anchor: `include/core/MainObjects/editorObject/getdiff.cpp`
- Include the file's direct dependencies and avoid unnecessary wrapper hops or
  artificial include hierarchies. Representative anchor:
  `include/core/interface/PDJE_interface.hpp`
- Do not introduce trivial forwarding headers whose only job is to redirect a
  public-looking path to a `detail/` header. If a path is meant to be public,
  keep the public declaration there; if the code is internal, keep callers on
  the internal path. This is especially disallowed for `function/image/`, where
  simple redirecting headers blur the public/private boundary without adding a
  stable contract. Representative anchor: `include/core/interface/PDJE_interface.hpp`
- Do not preserve forwarding headers as a long-lived compatibility pattern.
  Once callers move to the canonical owner path, remove the redirect layer
  instead of normalizing it into a new default. Representative anchor:
  `include/core/interface/PDJE_interface.hpp`

## Behavioral Rules

- Validate or sanitize inputs at the boundary before filesystem, persistence,
  or cross-module logic depends on them. Representative anchor:
  `include/core/interface/PDJE_interface.cpp`
- Log failures in the layer that has the richest operational context, and log
  the input, path, or identifier that explains the failure.
  Representative anchor: `include/core/MainObjects/editorObject/editorObject.cpp`
- Prefer simple return channels that match caller expectations: `bool` for
  operation success, `std::optional` for maybe-results, and empty containers for
  no-match query paths. Representative anchor:
  `include/core/editor/TimeLine/TimeLine.hpp`
- Treat pulled-out data lines, raw pointers, and borrowed handles as non-owning
  seams that require null checks and lifetime awareness before dereference.
  Representative anchor: `include/core/MainObjects/audioPlayer/audioPlayer.cpp`

## Legacy And Local Consistency

- Older parts of the tree may reflect mixed generations of naming, comments,
  and local idioms; do not promote every older pattern into a new default rule.
  Representative anchor: `include/core/FAKE_CAPNP_ENUMS_FOR_SWIG.hpp`
- When touching legacy code, prefer the default rules above unless the
  neighboring slice already forms a stronger local convention. Representative
  anchor: `include/core/db/trackDB.cpp`
- If a local deviation is intentional, keep it explicit and local rather than
  allowing silent drift in unrelated files. Representative anchor:
  `include/core/editor/TimeLine/DiffMachine/TimeLineDiffMachine.cpp`
