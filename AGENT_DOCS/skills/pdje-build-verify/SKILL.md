---
name: pdje-build-verify
description: Build and verify Project-DJ-Engine with its checked-in presets, explicit targets, and CTest module labels after the user approves the verification scope.
---

# PDJE Build Verify

Use this skill for build or executable verification, not documentation-only
inspection.

## Select The Flow

1. Apply the approval boundary and preset criteria in
   [../../VERIFY.md](../../VERIFY.md).
2. Use [../../CHANGE_MAP.md](../../CHANGE_MAP.md) to identify the owning
   subsystem when needed.
3. Select the narrowest reliable target and stable label from
   [../../TEST_MAP.md](../../TEST_MAP.md); use the full host-applicable suite for
   cross-cutting changes.
4. Compare the approved preset with existing dependency and `./build` state;
   bootstrap or configure only when the selected flow requires it.
5. Read [references/verification-flows.md](references/verification-flows.md)
   for the exact approved platform command forms.

Build and execution are separate evidence for manual/dev targets. Report the
selected flow and its coverage as required by the verification and test maps.
