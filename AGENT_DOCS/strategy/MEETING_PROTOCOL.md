# PDJE Strategy Meeting Protocol

Final plans come from closed meetings, not from unresolved discussion.

## Use

Escalate to a full meeting when a task may affect module ownership, public API
or C ABI shape, build/test wiring, verification cost, or long-term direction.
Ordinary tasks use a lightweight strategy check first.

## Flow

| Step | Action |
| --- | --- |
| 1 | Developer opens an agenda. |
| 2 | Agent gives grounded risks, objections, and options. |
| 3 | Discussion closes problems into decisions or deferrals. |
| 4 | Agent marks each deferral as blocking or non-blocking. |
| 5 | Developer explicitly asks for a final plan. |
| 6 | Agent writes the plan from closed decisions and accepted defaults. |

## Agent Rules

- Inspect repository truth before asking locally answerable questions.
- Challenge strategy drift with concrete reasoning.
- Explain verification cost and runtime risk before implementation.
- Keep decisions separate from assumptions.
- Do not turn unresolved blocking issues into defaults.
- Do not silently replace required model or effort settings.

## Closure

Ready only when agenda, accepted decisions, deferrals, blocking status,
affected-module model/effort/collaboration mode, and explicit final-plan request
are all known. Blocking deferrals prevent final plan creation.
