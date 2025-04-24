#!/bin/bash
faust -o compiled/FAUST_ROLL.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "RollFAUST" -scn "Roll_PDJE" -vec -vs 32 src/ROLL.dsp
faust -o compiled/FAUST_FILTERS.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "Filter_PDJE" -vec -vs 32 src/FILTER.dsp
faust -o compiled/FAUST_EQ.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "EQFAUST" -scn "EQ_PDJE" -vec -vs 32 src/EQ.dsp
faust -o compiled/FAUST_DISTORTION.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "DistortionFAUST" -scn "FaustInterpolate" -vec -vs 32 src/DISTORTION.dsp
faust -o compiled/FAUST_VOL.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "VolFAUST" -scn "FaustInterpolate" -vec -vs 32 src/VOL.dsp
faust -o compiled/FAUST_ECHO.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "EchoFAUST" -scn "Echo_PDJE" -vec -vs 32 src/ECHO.dsp
faust -o compiled/FAUST_OCS_FILTER.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "OcsFilterFAUST" -scn "OcsFilter_PDJE" -vec -vs 32 src/OCSFILTER.dsp
faust -o compiled/FAUST_TRANCE.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "TranceFAUST" -scn "Trance_PDJE" -vec -vs 32 src/TRANCE.dsp
faust -o compiled/FAUST_PANNER.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "PannerFAUST" -scn "Panner_PDJE" -vec -vs 32 src/PANNER.dsp
faust -o compiled/FAUST_PHASER.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "PhaserFAUST" -scn "Phaser_PDJE" -vec -vs 32 src/PHASER.dsp
faust -o compiled/FAUST_FLANGER.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "FlangerFAUST" -scn "Flanger_PDJE" -vec -vs 32 src/FLANGER.dsp
faust -o compiled/FAUST_COMPRESSOR.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "CompressorFAUST" -scn "Compressor_PDJE" -vec -vs 32 src/COMPRESSOR.dsp
faust -o compiled/FAUST_ROBOT.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "RobotFAUST" -scn "Robot_PDJE" -vec -vs 32 src/ROBOT.dsp

#!/bin/bash
faust -o manual_compiled/FAUST_ROLL_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "RollFAUSTMan" -scn "RollMan" -vec -vs 32 manual_src/ROLL.dsp
faust -o manual_compiled/FAUST_FILTERS_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "FilterFAUSTMan" -scn "FilterMan" -vec -vs 32 manual_src/FILTER.dsp
faust -o manual_compiled/FAUST_EQ_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "EQFAUSTMan" -scn "EQMan" -vec -vs 32 manual_src/EQ.dsp
faust -o manual_compiled/FAUST_DISTORTION_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "DistortionFAUSTMan" -scn "DistortionMan" -vec -vs 32 manual_src/DISTORTION.dsp
faust -o manual_compiled/FAUST_VOL_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "VolFAUSTMan" -scn "VolMan" -vec -vs 32 manual_src/VOL.dsp
faust -o manual_compiled/FAUST_ECHO_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "EchoFAUSTMan" -scn "EchoMan" -vec -vs 32 manual_src/ECHO.dsp
faust -o manual_compiled/FAUST_OCS_FILTER_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "OcsFilterFAUSTMan" -scn "OcsFilterMan" -vec -vs 32 manual_src/OCSFILTER.dsp
faust -o manual_compiled/FAUST_TRANCE_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "TranceFAUSTMan" -scn "TranceMan" -vec -vs 32 manual_src/TRANCE.dsp
faust -o manual_compiled/FAUST_PANNER_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "PannerFAUSTMan" -scn "PannerMan" -vec -vs 32 manual_src/PANNER.dsp
faust -o manual_compiled/FAUST_PHASER_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "PhaserFAUSTMan" -scn "PhaserMan" -vec -vs 32 manual_src/PHASER.dsp
faust -o manual_compiled/FAUST_FLANGER_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "FlangerFAUSTMan" -scn "FlangerMan" -vec -vs 32 manual_src/FLANGER.dsp
faust -o manual_compiled/FAUST_COMPRESSOR_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "CompressorFAUSTMan" -scn "CompressorMan" -vec -vs 32 manual_src/COMPRESSOR.dsp
faust -o manual_compiled/FAUST_ROBOT_manual.hpp -lang cpp  -light -mapp -single -nvi -exp10 -it -cn "RobotFAUSTMan" -scn "RobotMan" -vec -vs 32 manual_src/ROBOT.dsp

