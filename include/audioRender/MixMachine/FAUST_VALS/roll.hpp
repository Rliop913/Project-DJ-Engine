#pragma once
#include "Faust_interpolate.hpp"

class Roll_PDJE : public FaustInterpolate {
  public:
    float RollBpm;
    int   RollSwitch;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.RollBpm    = RollBpm;
        dest.RollSwitch = RollSwitch;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
        copyInterpolates(dest);
    }
};