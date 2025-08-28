#pragma once
#include "Faust_interpolate.hpp"

class EQ_PDJE : public FaustInterpolate {
  public:
    int EQSelect;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.EQSelect = EQSelect;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
        copyInterpolates(dest);
    }
};