```mermaid
flowchart TD
MixBinary --> MixTranslator
MixTranslator --> MixMachine
```

A [[MixBinary]] parser. Reads [[CapNProto]] and load datas into memory.
then, send to [[MixMachine]].