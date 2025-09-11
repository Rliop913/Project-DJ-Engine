#pragma once
#include "Faust_interpolate.hpp"

class Panner_PDJE : public FaustInterpolate {
  public:
    float bps;
    float PGain;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.bps   = bps;
        dest.PGain = PGain;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
        copyInterpolates(dest);
    }
};