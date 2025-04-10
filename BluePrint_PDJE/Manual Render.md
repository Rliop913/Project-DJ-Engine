

```mermaid
flowchart TD
FAUST_COMPILED_DSP_CODE -->|inherit| FXFAUSTMan
ARGSETTER --> FXMan
FXMan -->|inherit| FXFAUSTMan
FXFAUSTMan --> FXControlPannel
```


See: [[ARGSETTER]], [[FXMan]], [[FAUST_COMPILED_DSP_CODE]], [[FXFAUSTMan]], [[FXControlPannel]]