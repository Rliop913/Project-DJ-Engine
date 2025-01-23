#!/bin/bash
faust -o compiled/FAUST_ROLL.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "RollFAUST" -scn "RollVal" -vec -vs 64 src/ROLL.dsp
faust -o compiled/FAUST_FILTERS.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/FILTER.dsp
faust -o compiled/FAUST_EQ.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/EQ.dsp
faust -o compiled/FAUST_DISTORTION.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/DISTORTION.dsp
faust -o compiled/FAUST_VOL.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/VOL.dsp
faust -o compiled/FAUST_ECHO.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/ECHO.dsp
faust -o compiled/FAUST_OCS_FILTER.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/OCSFILTER.dsp
faust -o compiled/FAUST_TRANCE.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/TRANCE.dsp
faust -o compiled/FAUST_PANNER.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/PANNER.dsp
faust -o compiled/FAUST_PHASER.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/PHASER.dsp
faust -o compiled/FAUST_FLANGER.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/FLANGER.dsp
faust -o compiled/FAUST_COMPRESSOR.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/COMPRESSOR.dsp
