PDJE provides moduled engines

[[audio Engine]]

[[Input Engine]]

[[Note Engine]]

[[Utility Engine]]

```mermaid
flowchart TD

audioEngine -- audioLine --- InputEngine

audioEngine -- audioLine --- NoteEngine

InputEngine -- InputLine --- NoteEngine

audioEngine -- audioLine --- UtilityEngine
```

