Data_Formats
==============




Editor Format
==============


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
   "NONE", "ID", "NONE", "NONE", "NONE", "NONE", "NONE"
   "ROLL(15)", "ID", "0", "ITPL", "8PointValues", "BPM", "Wet amount"
   "COMPRESSOR(16)", "ID", "0", "Strength", "Thresh, Knee", "ATT, REL", "NONE"
   "ROBOT(17)", "ID", "0", "ITPL", "8PointValues", "ocsFreq", "Wet amount"

KEYWORDS

======================================================        ============================================================
ITPL                                                            8PointValues
======================================================        ============================================================
Choose Interpolator type(linear, cosine, cubic, flat)           8 data points that define a waveform for the interpolator
======================================================        ============================================================

On Flat interpolator type, you need to write just one value


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


The "BPM" Note_Type name is prefixed for the default bpm change implementation.



FX LIST & ENUM
==================