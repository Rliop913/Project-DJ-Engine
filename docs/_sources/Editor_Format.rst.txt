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
      - firstBeat
    * - Text
      - Text
      - Text
      - Double
      - Binary(CapNProto)
      - TEXT

.. _about-mix-data:

Mix Data
-----------

Mix Data in the PDJE engine is a data structure that defines mixing events, effect parameter changes, and load/unload controls that occur during track playback. 

Each entry specifies what action takes place at a particular time (based on Beat/subBeat), covering various audio control elements such as filters, EQ, volume, distortion, delay, and DJ scratching.

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
      - beat
      - subBeat
      - separate
      - Ebeat
      - EsubBeat
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


.. code-block:: c++

  std::string EightPoint = "5000,1000,2000,3000,4000,5000,5500,6000";

this means

.. image:: _static/eightPoint_example.png

We interpolate this data using various interpolation methods.

Point Index 1 corresponds to the **Start Position**, defined as:  
`beat + (beat / separate) * subBeat`

Point Index 8 corresponds to the **End Position**, defined as:  
`Ebeat + (Ebeat / Eseparate) * EsubBeat`

As you may have noticed, the data provided through `8PointValues` is transformed into a graph consisting of 8 key points.  
This data serves as the `Interpolated Value`, which transitions smoothly from the **Start Position** to the **End Position** using the specified `ITPL` interpolation method—such as linear, cosine, cubic, or flat.

This mechanism allows for precise control of parameters (e.g., filter frequency, volume, drive) that vary over time, enabling **smooth and continuous modulation** within audio effect modules.

In other words, rather than relying on static values, the system supports **curve-based, time-dependent control**, allowing for more **dynamic and expressive musical behavior**.

.. attention:: 
  - On Flat interpolator type, you need to write just one value

  - SPEED: -N ~ N (float) 1 Equals 100%, -1.0 Equals -100%(reverse play), 10.0 Equals 1000%. Based on BPM before time stretching.

Note Data
------------

Note Data in the PDJE engine is a data structure that defines rhythm game note events. 

Each note specifies what kind of input is required at a particular point in time (Beat, subBeat, Separate), and the player must respond accurately at that timing in order to score.

Note data is the core element representing player interaction. Unlike Mix Data, which records changes to audio parameters, Note Data strictly handles “input events.”

In particular, both Note_Type and Note_Detail are declared as TEXT fields, not as predefined ENUM values. 

Aside from reserved keywords such as the BPM prefix, users are expected to define and extend their own types and details. This design allows for flexible customization to support specific game modes, input devices, or user-defined rules.

Additionally, any Note_Type beginning with BPM carries special meaning, as it represents an event that applies a tempo (BPM) change during playback. 

In this case, the first field is treated as the new BPM value, ensuring synchronization between note judgment timing and the actual playback speed of the track.

.. list-table:: Note Table
   :header-rows: 1
   :widths: 15 20 15 15 15 15 15 15 15 15 20 15

   * - Note_Type
     - Note_Detail
     - first
     - second
     - third
     - beat
     - subBeat
     - separate
     - Ebeat
     - EsubBeat
     - Eseparate
     - RailID
   * - TEXT
     - uint16
     - TEXT
     - TEXT
     - TEXT
     - long
     - long
     - long
     - long
     - long
     - long
     - uint64


- The "BPM" Note_Type name is prefixed for the default bpm change implementation.
- The "AXIS" Note_Type name is prefixed for the mouse movements. Warn: you need to implement your own judge logic


About Beat & subBeat & Separate
=================================

``Beat``, ``subBeat``, and ``Separate`` are indexing methods derived from the Beat Grid concept (See: :`Serato Beat Grid <https://support.serato.com/hc/en-us/articles/202523390-Introduction-to-Beatgrids>`_ ), which is widely used in DAWs (Digital Audio Workstations), DJ software  and MIDI sequencing.


A Beat Grid divides musical time based on **BPM (tempo)** and **time signature**, providing a precise rhythmic framework for placing musical events along a timeline.


Within this framework, ``Beat`` represents the primary rhythmic unit (typically a quarter note in 4/4 time), ``subBeat`` further subdivides each beat into smaller logical segments (such as 1/4 or 1/8 of a beat), and ``Separate`` defines custom high-resolution divisions used for fine-grained quantization or event positioning.


This indexing method is conceptually aligned with timeline systems used in rhythm games and digital audio applications, where accurate synchronization and sub-beat resolution are essential.


Key Terms
---------

- **firstBeat**  
  The PCM frame (sample-level position) where the first Beat begins.  
  Used as the reference point for synchronization of the music.

- **beat**  
  The position of the **beat** (not the measure) calculated based on BPM and time signature.
  It represents the primary rhythmic unit (e.g., a quarter note in 4/4 time).

- **subBeat**  
  The position within a beat that divides it into smaller rhythmic units.
  Typically used for finer timing precision within a beat.

- **separate**  
  The index of a fine-grained subdivision when a beat is divided into smaller, fixed segments.
  For example, if separate = 192, it means the beat is sliced into 192 equal parts, and this value specifies the current segment number.

- **ebeat / esubBeat / eseparate**  
  The prefix ``e`` stands for **end**.
  These fields indicate the ending positions of an event that spans a duration (e.g., long notes, sustained effects), using the same beat/subBeat/separate indexing system.


---

Summary
-------

- **firstBeat** represents the actual PCM frame in the audio stream where the music begins.  
- **beat / subBeat / separate** describe the timeline position of an event according to the BPM and time signature.  
- **ebeat / esubBeat / eseparate** mark the ending position of the event, allowing representation of interval-based events.  

This structure enables the PDJE engine to achieve **precise synchronization** between the audio signal (PCM-level) and rhythm game events (beat-level).
