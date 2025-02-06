#pragma once
#include "Faust_interpolate.hpp"

class Filter_PDJE: public FaustInterpolate{
protected:
    int LowFilterSW;
    int HighFilterSW;
};