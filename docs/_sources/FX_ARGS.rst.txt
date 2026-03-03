FX_ARGS
=======

This page lists the exact, case-sensitive argument keys returned by the current
`makeArgSetter()` implementations under
`include/core/audioRender/ManualMix/ManualFausts/*.hpp`.

Use these strings exactly as written. `FXControlPanel::GetArgSetter(FXList)`
returns a map keyed by the verbatim literals from the source headers, so
changing uppercase/lowercase or using older alias names will not match.

The section titles below match the `FXList` enum names from `ManualMix.hpp`.

COMPRESSOR
----------

Source: `include/core/audioRender/ManualMix/ManualFausts/compressorMan.hpp`

- `Strength`
- `ThreshDB`
- `AttackMS`
- `ReleaseMS`
- `KneeDB`

DISTORTION
----------

Source: `include/core/audioRender/ManualMix/ManualFausts/distortionMan.hpp`

- `DistortionValue`

ECHO
----

Source: `include/core/audioRender/ManualMix/ManualFausts/echoMan.hpp`

- `EchoBps`
- `EchoFeedback`
- `EchoDryWet`

EQ
--

Source: `include/core/audioRender/ManualMix/ManualFausts/eqMan.hpp`

- `EQLow`
- `EQMid`
- `EQHigh`

FILTER
------

Source: `include/core/audioRender/ManualMix/ManualFausts/filterMan.hpp`

- `HLswitch`
- `Filterfreq`

FLANGER
-------

Source: `include/core/audioRender/ManualMix/ManualFausts/flangerMan.hpp`

- `Bps`
- `FlangerDryWet`

OCSFILTER
---------

Source: `include/core/audioRender/ManualMix/ManualFausts/ocsFilterMan.hpp`

This section follows the current enum name `FXList::OCSFILTER`. The accepted
argument keys are only the strings listed below.

- `OCSFilterHighLowSW`
- `MiddleFreq`
- `RangeFreqHalf`
- `Bps`
- `OCSFilterDryWet`

PANNER
------

Source: `include/core/audioRender/ManualMix/ManualFausts/pannerMan.hpp`

- `Bps`
- `PGain`
- `PannerDryWet`

PHASER
------

Source: `include/core/audioRender/ManualMix/ManualFausts/phaserMan.hpp`

- `Bps`
- `PhaserDryWet`

ROBOT
-----

Source: `include/core/audioRender/ManualMix/ManualFausts/robotMan.hpp`

- `RobotFreq`
- `RobotDryWet`

ROLL
----

Source: `include/core/audioRender/ManualMix/ManualFausts/rollMan.hpp`

- `RollBpm`
- `RollPower`

TRANCE
------

Source: `include/core/audioRender/ManualMix/ManualFausts/tranceMan.hpp`

- `Bps`
- `Gain`
- `TranceDryWet`

VOL
---

Source: `include/core/audioRender/ManualMix/ManualFausts/volMan.hpp`

- `VolPower`

Usage Example
-------------

.. code-block:: c++

   auto fx = player->GetFXControlPanel();
   if (!fx) {
       return;
   }

   fx->FX_ON_OFF(FXList::ECHO, true);
   auto echo = fx->GetArgSetter(FXList::ECHO);
   echo["EchoBps"](2.0);
   echo["EchoFeedback"](0.35);
   echo["EchoDryWet"](0.5);
