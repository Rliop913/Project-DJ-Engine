```mermaid
flowchart TD
Code --> MixMachine
Musics --> MixMachine
MixMachine -->|N_frames| Rendered_audio
Rendered_audio -->|Every Audio Callback| MixMachine
```

```mermaid
flowchart TD
ON --> Pannel
OFF --> Pannel
Args --> Pannel
Pannel --> FX

```

In **[[RealTime_Render]]**, you can think of it as bypassing **[[MixBinary]]** and **[[MixTranslator]]**, directly manipulating their roles through code.

However, since full control is handed over to the user, **frame-perfect** precision is no longer guaranteed.

See: [[MixMachine]]