Editor_Format
==============

Track Data
--------------
Track is the core data structure in the PDJE engine that represents a single music mixset unit. 

Each track includes the song’s metadata, actual audio mixing information, rhythm game note data, and a cached mix list.


.. list-table:: Track Data Format
    :header-rows: 1
    :widths: 25 25 25 25

    * - TrackTitle
      - MixBinary
      - NoteBinary
      - CachedMixList
    * - Text
      - Binary(CapNProto)
      - Binary(CapNProto)
      - Text(CSV)


Music Meta Data
-----------------

Music Meta Data in the PDJE engine is a data structure that contains the attributes and fundamental information of a song itself. 

It defines the key data required for playback and analysis, such as the audio file path, composer, and BPM.


.. list-table:: Music Meta Data Format
    :header-rows: 1
    :widths: 25 25 25 25 25 25

    * - Title
      - Composer
      - MusicPath
      - Bpm
      - BpmBinary
      - firstBar
    * - Text
      - Text
      - Text
      - Double
      - Binary(CapNProto)
      - TEXT


Mix Data
-----------

Mix Data in the PDJE engine is a data structure that defines mixing events, effect parameter changes, and load/unload controls that occur during track playback. 

Each entry specifies what action takes place at a particular time (based on Bar/Beat), covering various audio control elements such as filters, EQ, volume, distortion, delay, and DJ scratching.

Unlike rhythm game note events, this data records changes in audio processing parameters themselves, which are applied in real time during playback.

.. list-table:: Mix Data Format
    :header-rows: 1
    :widths: 25 25 25 25 25 25 25 25 25 25 25 25

    * - type
      - details
      - ID
      - first
      - second
      - third
      - bar
      - beat
      - separate
      - Ebar
      - Ebeat
      - Eseparate
    * - TYPE_ENUM
      - DETAIL_ENUM
      - int
      - TEXT
      - TEXT
      - TEXT
      - long
      - long
      - long
      - long
      - long
      - long


.. csv-table:: Mix Data Table
   :header: "type", "ID", "details", "first", "second", "third", "Interpolated Value"
   :widths: 15, 10, 35, 20, 20, 30, 20

   "FILTER(0)", "ID", "HIGH(0)/LOW(2)", "ITPL", "8PointValues", "NONE", "filter Frequency"
   "EQ(1)", "ID", "HIGH(0)/MID(1)/LOW(2)", "ITPL", "8PointValues", "NONE", "eq value"
   "DISTORTION(2)", "ID", "0", "ITPL", "8PointValues", "NONE", "drive value"
   "CONTROL(3)", "ID", "PAUSE(3)/CUE(4)", "approx_loc", "X", "NONE", "NONE"
   "VOL(4)", "ID", "TRIM(7)/FADER(8)", "ITPL", "8PointValues", "NONE", "volume"
   "LOAD(5)", "ID", "0", "title", "composer", "bpm", "NONE"
   "UNLOAD(6)", "ID", "0", "X", "X", "NONE", "NONE"
   "bpmControl(7)", "ID", "timeStretch(6)", "BPM(double)", "NONE", "NONE", "NONE"
   "ECHO(8)", "ID", "0", "ITPL", "8PointValues", "BPM, feedback", "Wet amount"
   "OCS_Filter(9)", "ID", "HIGH(0)/LOW(2)", "ITPL", "8PointValues", "BPM, MiddleFreq, RangeHalfFreq", "Wet amount"
   "FLANGER(10)", "ID", "0", "ITPL", "8PointValues", "BPM", "Wet amount"
   "PHASER(11)", "ID", "0", "ITPL", "8PointValues", "BPM", "Wet amount"
   "TRANCE(12)", "ID", "0", "ITPL", "8PointValues", "BPM, GAIN", "Wet amount"
   "PANNER(13)", "ID", "0", "ITPL", "8PointValues", "BPM, GAIN", "Wet amount"
   "BATTLE_DJ(14)", "ID", "SPIN(12)/PITCH(13)/REV(14)", "SPEED", "NONE", "NONE", "NONE"
   "BATTLE_DJ(14)", "ID", "SCRATCH(15)", "StartPosition", "SPEED", "NONE", "NONE"
   "ROLL(15)", "ID", "0", "ITPL", "8PointValues", "BPM", "Wet amount"
   "COMPRESSOR(16)", "ID", "0", "Strength", "Thresh, Knee", "ATT, REL", "NONE"
   "ROBOT(17)", "ID", "0", "ITPL", "8PointValues", "ocsFreq", "Wet amount"

KEYWORDS

======================================================        ============================================================
ITPL                                                            8PointValues
======================================================        ============================================================
Choose Interpolator type(linear, cosine, cubic, flat)           8 data points that define a waveform for the interpolator
======================================================        ============================================================

- On Flat interpolator type, you need to write just one value

- SPEED: -N ~ N (float) 1 Equals 100%, -1.0 Equals -100%(reverse play), 10.0 Equals 1000%. Based on BPM before time stretching.

Note Data
------------

Note Data in the PDJE engine is a data structure that defines rhythm game note events. 

Each note specifies what kind of input is required at a particular point in time (Bar, Beat, Separate), and the player must respond accurately at that timing in order to score.

Note data is the core element representing player interaction. Unlike Mix Data, which records changes to audio parameters, Note Data strictly handles “input events.”

In particular, both Note_Type and Note_Detail are declared as TEXT fields, not as predefined ENUM values. 

Aside from reserved keywords such as the BPM prefix, users are expected to define and extend their own types and details. This design allows for flexible customization to support specific game modes, input devices, or user-defined rules.

Additionally, any Note_Type beginning with BPM carries special meaning, as it represents an event that applies a tempo (BPM) change during playback. 

In this case, the first field is treated as the new BPM value, ensuring synchronization between note judgment timing and the actual playback speed of the track.

.. list-table:: Note Table
   :header-rows: 1
   :widths: 15 20 15 15 15 15 15 15 15 15 20

   * - Note_Type
     - Note_Detail
     - first
     - second
     - third
     - bar
     - beat
     - separate
     - Ebar
     - Ebeat
     - Eseparate
   * - TEXT
     - TEXT
     - TEXT
     - TEXT
     - TEXT
     - long
     - long
     - long
     - long
     - long
     - long


- The "BPM" Note_Type name is prefixed for the default bpm change implementation.



About Bar & Beat & Separate
============================

``Bar``, ``Beat``, and ``Separate`` are indexing methods that represent music data based on **BPM (tempo) and time signature**.  
This concept is derived from the **beat-based timeline representation** commonly used in rhythm games and DAWs (Digital Audio Workstations).  
For example, in MIDI sequencing or DAWs, events are also placed at positions such as "which bar, which beat, and which subdivision."

---

Key Terms
---------

- **firstBeat**  
  The PCM frame (sample-level position) where the first beat begins.  
  Used as the reference point for synchronization of the music.

- **bar**  
  The position of the measure (bar) calculated from BPM and time signature information.

- **beat**  
  The downbeat position within a bar.  
  For example, in 4/4 time, there are four beats within one bar.

- **separate**  
  The subdivision index when a bar is divided into smaller parts.  
  For instance, if ``separate=192``, it means the bar is divided into 192 segments, and this value represents which segment is being referred to.

- **ebar / ebeat / eseparate**  
  The prefix ``e`` stands for **end**.  
  These are used when an event spans over a duration (e.g., long notes, sustained effects), recording the bar/beat/separate position where the event ends.

---

Summary
-------

- **firstBeat** represents the actual PCM frame in the audio stream where the music begins.  
- **bar / beat / separate** describe the timeline position of an event according to the BPM and time signature.  
- **ebar / ebeat / eseparate** mark the ending position of the event, allowing representation of interval-based events.  

This structure enables the PDJE engine to achieve **precise synchronization** between the audio signal (PCM-level) and rhythm game events (beat-level).
