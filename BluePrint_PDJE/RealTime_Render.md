```mermaid
flowchart TD
MixBinary --> MixTranslator
MixTranslator --> MixMachine
Musics --> MixMachine
MixMachine -->|N_frames| Rendered_audio
Rendered_audio -->|Every Audio Callback| MixMachine
```

The difference between [[PreRender]] and [[RealTime_Render]] is that [[RealTime_Render]] generates and delivers only the necessary audio data in real-time during each audio callback.

See: [[MixBinary]], [[MixTranslator]], [[MixMachine]]