
We use the [[Bpm]] and the position of the [[firstBar]] to derive the exact playback position in the music.


MPB(Milisecond Per BPM) = BPM / (60 * 1000)

MB = [[bar]] * MPB 

Music Playback Location
= [[firstBar]] + MB + (MB / [[separate]]) * [[beat]]