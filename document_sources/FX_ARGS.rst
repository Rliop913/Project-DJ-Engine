
FX_ARGS
==========

FILTER
-------

- HLswitch : 0 is Highpass Filter, 1 is LowPass Filter
- Filterfreq : Frequency of the filter

EQ
----

- EQLow : lowshelf filter, affects below ~250Hz
- EQMid : peak EQ filter, Center frequency around 2125 Hz with a bandwidth of 1875 Hz
- EQHigh : highshelf filter, affects above ~4000Hz


DISTORTION
------------

- DistortionValue : 0.0 - 1.0, distortion intensity


VOL
-----

- VolPower : 0.0 - 1.0, controls volume


ECHO
------

- EchoBps : Delay time (BPS-synced)
- EchoFeedback : 0.0 - 1.0, amount of repeats
- EchoDryWet : 0.0 - 1.0, Balance between original and echo

OCS_Filter
------------

- OcsFilterHighLowSW : 0 = High-pass mode, 1 = Low-pass mode
- MiddleFreq : Center frequency (Hz) around which oscillation is applied
- RangeFreqHalf : Half of the oscillation range (defines how wide the cutoff frequency swings)
- Bps : Oscillation speed in beats per second (synchronizes with track tempo)
- OCSFilterDryWet : 0.0 - 1.0, Balance between original signal and processed filter output, 0.0 = dry only, 1.0 = wet only



FLANGER
---------

- Bps : Oscillation rate in beats per second
- FlangerDryWet : 0.0 - 1.0, Balance between original signal (dry) and flanged signal (wet), 0.0 = dry only, 1.0 = wet only.

PHASER
-------

- Bps : Oscillation rate in beats per second
- PhaserDryWet : 0.0 - 1.0, Balance between original (dry) and phase-processed (wet) signal, 0.0 = dry only, 1.0 = wet only.

TRANCE
---------

- Bps : Beats per second
- Gain : 0.0 - 1.0, Threshold value for shaping the sawtooth into a gate, Determines how aggressively the audio is cut.
- TranceDryWet : 0.0 - 1.0, Balance between original (dry) and gated (wet) signal, 0.0 = dry only, 1.0 = gated signal only.

PANNER
---------

- Bps : Beats per second
- PGain : 0.0 - 1.0, Modulation depth (how far the signal moves across the stereo field), Higher values = wider panning movement.
- PannerDryWet : 0.0 - 1.0, Mix balance between original (dry) and panned (wet) signal, 0.0 = dry only, 1.0 = fully panned.

ROLL
------

- RollBpm : BPM-synced timing reference for the roll loop, Controls how the roll aligns with the beat grid.
- RollPower : Activation and intensity control, 0.0 = off (bypass), 1.0 = full roll effect

.. important::

    Audio is written into the roll buffer only when RollPower (=roll_switch) is greater than 0.01.

    If RollPower is 0.01 or below, recording stops and no new data is stored.

    So, RollPower acts both as the activation switch for recording and as the wet/dry mix control of the roll effect.

COMPRESSOR
-----------

- Strength : Overall compression strength (acts like a ratio/intensity control).
- ThreshDB : Threshold above which compression begins.
- AttackMS : Time for the gain reduction to reach the target after the signal exceeds the threshold.
- ReleaseMS : Time for the gain to recover after the signal falls below the threshold.
- KneeDB : Soft-knee width; larger values make the transition into compression smoother.


ROBOT
------

- RobotFreq : Carrier oscillator frequency (Hz). Higher values → more metallic/buzzy; lower values → tremolo-like.
- RobotDryWet : 0.0 - 1.0, Wet/dry balance. 0.0 = dry only, 1.0 = fully robotized.