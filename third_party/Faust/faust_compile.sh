#!/bin/bash
faust -o compiled/FAUST_ROLL.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "RollFAUST" -scn "RollVal" -vec -vs 64 src/ROLL.dsp
faust -o compiled/FAUST_FILTERS.hpp -lang cpp -clang -light -mapp -single -nvi -exp10 -it -cn "FilterFAUST" -scn "FilterVal" -vec -vs 64 src/FILTER.dsp
