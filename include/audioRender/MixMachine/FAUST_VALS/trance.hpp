#pragma once
#include "Faust_interpolate.hpp"

class Trance_PDJE : public FaustInterpolate {
  public:
    float bps;
    float gain;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.bps  = bps;
        dest.gain = gain;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
        copyInterpolates(dest);
    }
};