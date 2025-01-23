music mixing file. implemented with [[CapNProto]]

| [[type]]      | [[details]]     | [[ID]] | [[first]] | [[second]] | [[third]] | [[bar]] | [[beat]] | [[separate]] | [[Ebar]] | [[Ebeat]] | [[Eseparate]] |
| ------------- | --------------- | ------ | --------- | ---------- | --------- | ------- | -------- | ------------ | -------- | --------- | ------------- |
| [[TYPE_ENUM]] | [[DETAIL_ENUM]] | int    | TEXT      | TEXT       | TEXT      | long    | long     | long         | long     | long      | long          |
```mermaid
flowchart TD
MixBinary --> MixTranslator
```

See: [[MixTranslator]]
"I" means it can get frame in only
"O" means it can get frame in-out only
"IO" means it can get frame in only, and in-out

| in-out | [[type]]       | [[ID]] | [[details]]                            | [[first]]     | [[second]]       | [[third]]                      | when off |
| ------ | -------------- | ------ | -------------------------------------- | ------------- | ---------------- | ------------------------------ | -------- |
| O      | FILTER(0)      | ID     | HIGH(0)/LOW(2)                         | [[ITPL]]      | [[8PointValues]] |                                |          |
| IO     | EQ(1)          | ID     | HIGH(0)/MID(1)/LOW(2)                  | [[ITPL]]      | [[8PointValues]] |                                | val<-60  |
| IO     | DISTORTION(2)  | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] |                                | val<0    |
| I      | CONTROL(3)     | ID     | PAUSE(3)                               | approx_loc    | X                |                                |          |
| IO     | VOL(4)         | ID     | TRIM(7)/FADER(8)                       | [[ITPL]]      | [[8PointValues]] |                                |          |
| I      | LOAD(5)        | ID     | 0                                      | title         | composer         | bpm                            |          |
| I      | UNLOAD(6)      | ID     | 0                                      | X             | X                |                                |          |
| I      | bpmControl(8)  | ID     | timeStretch(6)                         | BPM(double)   |                  |                                |          |
| O      | ECHO(9)        | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM, feedback                  | bps<0    |
| O      | OCS_Filter(10) | ID     | HIGH(0)/LOW(2)                         | [[ITPL]]      | [[8PointValues]] | BPM, MiddleFreq, RangeHalfFreq | bps<0    |
| O      | FLANGER(11)    | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            | bps<0    |
| O      | PHASER(12)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            | bps<0    |
| O      | TRANCE(13)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM,GAIN                       | bps<0    |
| O      | PANNER(14)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM,GAIN                       | bps<0    |
| O      | BATTLE_DJ(15)  | ID     | [[SPIN]](12)/[[PITCH]](13)/[[REV]](14) | SPEED         |                  |                                |          |
| I      | BATTLE_DJ(15)  | ID     | SCRATCH(15)                            | StartPosition | SPEED            |                                |          |
| X      |                | ID     |                                        |               |                  |                                |          |
| O      | ROLL(16)       | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            |          |
|        | COMPRESSOR(17) | ID     | 0                                      | Strength      | Thresh,Knee      | ATT,REL                        |          |
|        |                |        |                                        |               |                  |                                |          |

SPEED: -N ~ N (float) 1 Equals 100%, -1.0 Equals -100%(reverse play), 10.0 Equals 1000%. Based on BPM before time stretching.
