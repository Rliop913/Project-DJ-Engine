Data_Formats
==============




Editor Format
==============


| [[TrackTitle]] | [[MixBinary]]         | [[NoteBinary]]        | [[CachedMixList]] |
| -------------- | --------------------- | --------------------- | ----------------- |
| Text           | Binary([[CapNProto]]) | Binary([[CapNProto]]) | Text(CSV)         |





| [[Title]] | [[Composer]] | [[MusicPath]] | [[Bpm]] | [[BpmBinary]]         | [[firstBar]] |
| --------- | ------------ | ------------- | ------- | --------------------- | ------------ |
| Text      | Text         | Text          | Double  | Binary([[CapNProto]]) | TEXT         |


| [[type]]      | [[details]]     | [[ID]] | [[first]] | [[second]] | [[third]] | [[bar]] | [[beat]] | [[separate]] | [[Ebar]] | [[Ebeat]] | [[Eseparate]] |
| ------------- | --------------- | ------ | --------- | ---------- | --------- | ------- | -------- | ------------ | -------- | --------- | ------------- |
| [[TYPE_ENUM]] | [[DETAIL_ENUM]] | int    | TEXT      | TEXT       | TEXT      | long    | long     | long         | long     | long      | long          |


| [[type]]       | [[ID]] | [[details]]                            | [[first]]     | [[second]]       | [[third]]                      | Interpolated Value |
| -------------- | ------ | -------------------------------------- | ------------- | ---------------- | ------------------------------ | ------------------ |
| FILTER(0)      | ID     | HIGH(0)/LOW(2)                         | [[ITPL]]      | [[8PointValues]] |                                | filter Frequency   |
| EQ(1)          | ID     | HIGH(0)/MID(1)/LOW(2)                  | [[ITPL]]      | [[8PointValues]] |                                | eq value           |
| DISTORTION(2)  | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] |                                | drive  value       |
| CONTROL(3)     | ID     | PAUSE(3)/CUE(4)                        | approx_loc    | X                |                                |                    |
| VOL(4)         | ID     | TRIM(7)/FADER(8)                       | [[ITPL]]      | [[8PointValues]] |                                | volume             |
| LOAD(5)        | ID     | 0                                      | title         | composer         | bpm                            |                    |
| UNLOAD(6)      | ID     | 0                                      | X             | X                |                                |                    |
| bpmControl(7)  | ID     | timeStretch(6)                         | BPM(double)   |                  |                                |                    |
| ECHO(8)        | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM, feedback                  | Wet amount         |
| OCS_Filter(9)  | ID     | HIGH(0)/LOW(2)                         | [[ITPL]]      | [[8PointValues]] | BPM, MiddleFreq, RangeHalfFreq | Wet amount         |
| FLANGER(10)    | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            | Wet amount         |
| PHASER(11)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            | Wet amount         |
| TRANCE(12)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM,GAIN                       | Wet amount         |
| PANNER(13)     | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM,GAIN                       | Wet amount         |
| BATTLE_DJ(14)  | ID     | [[SPIN]](12)/[[PITCH]](13)/[[REV]](14) | SPEED         |                  |                                |                    |
| BATTLE_DJ(14)  | ID     | SCRATCH(15)                            | StartPosition | SPEED            |                                |                    |
|                | ID     |                                        |               |                  |                                |                    |
| ROLL(15)       | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | BPM                            | Wet amount         |
| COMPRESSOR(16) | ID     | 0                                      | Strength      | Thresh,Knee      | ATT,REL                        |                    |
| ROBOT(17)      | ID     | 0                                      | [[ITPL]]      | [[8PointValues]] | ocsFreq                        | Wet amount         |

SPEED: -N ~ N (float) 1 Equals 100%, -1.0 Equals -100%(reverse play), 10.0 Equals 1000%. Based on BPM before time stretching.


| [[Note_Type]] | [[Note_Detail]] | [[first]] | [[second]] | [[third]] | [[bar]] | [[beat]] | [[separate]] | [[Ebar]] | [[Ebeat]] | [[Eseparate]] |
| ------------- | --------------- | --------- | ---------- | --------- | ------- | -------- | ------------ | -------- | --------- | ------------- |
| TEXT          | TEXT            | TEXT      | TEXT       | TEXT      | long    | long     | long         | long     | long      | long          |
Prefixed Type Name: BPM
use for: changes only note bpm


FX LIST & ENUM
==================