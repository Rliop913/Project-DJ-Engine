# PDJE Strategy Meeting Protocol

The development strategy harness is meeting-based. A final implementation plan
is the output of a closed meeting, not the starting point.

## When To Use The Meeting

Use the full protocol for any task that may affect:

- module boundaries or ownership
- public API or C ABI shape
- build, preset, or test wiring
- verification cost or confidence
- long-term development direction

For ordinary code tasks, perform a lightweight strategy check and escalate to a
meeting if drift, risk, or unclear ownership appears.

## Meeting Flow

1. The developer opens with an agenda.
2. The agent responds with grounded opinions, risks, objections, and options.
3. The discussion closes open problems into explicit decisions or deferrals.
4. The agent records whether each deferred item blocks the current plan.
5. The developer explicitly asks for a final plan.
6. The agent writes the final plan from the closed decisions and accepted
   defaults.

## Agent Responsibilities

- Inspect repository truth before asking questions that can be answered locally.
- Challenge strategy drift with concrete reasoning.
- Explain verification cost and runtime risk before implementation begins.
- Keep decisions separate from assumptions.
- Do not convert unresolved blocking issues into implementation defaults.
- Do not silently replace required model or effort settings.

## Closure Rules

A meeting is ready to produce a final plan only when:

- the agenda is clear
- accepted decisions are listed
- deferred issues are listed
- each deferred issue says whether it blocks the current execution
- model, effort, and collaboration mode are known for the affected module
- the developer has explicitly requested final plan creation

Deferred non-blocking issues become assumptions or follow-up notes in the final
plan. Deferred blocking issues prevent final plan creation until the meeting
closes them.

