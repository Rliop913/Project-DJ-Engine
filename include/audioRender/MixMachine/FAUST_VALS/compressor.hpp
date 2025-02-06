#pragma once
#include "Faust_interpolate.hpp"

class Compressor_PDJE{
protected:
    float strength;
    int threshDB;
    int attackMS;
    int releaseMS;
    int kneeDB;
};