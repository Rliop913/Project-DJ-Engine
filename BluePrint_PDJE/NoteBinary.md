qA data for note description

| [[Note_Type]] | [[Note_Detail]] | [[first]] | [[second]] | [[third]] | [[bar]] | [[beat]] | [[separate]] | [[Ebar]] | [[Ebeat]] | [[Eseparate]] | RailID |
| ------------- | --------------- | --------- | ---------- | --------- | ------- | -------- | ------------ | -------- | --------- | ------------- | ------ |
| TEXT          | uint16          | TEXT      | TEXT       | TEXT      | long    | long     | long         | long     | long      | long          | uint64 |
Prefixed Type Name: BPM
use for: changes only note bpm

Prefixed Type Name: AXIS
use for: prefix flag for axis datas. ex) mouse movement.
Warn: you need to implement your own judge logic.


Set Eseparete = -1 to skip second position calculation