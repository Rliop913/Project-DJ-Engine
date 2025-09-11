#pragma once
#include "Faust_interpolate.hpp"

class Compressor_PDJE {
  public:
    float strength;
    int   threshDB;
    int   attackMS;
    int   releaseMS;
    int   kneeDB;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.strength  = strength;
        dest.threshDB  = threshDB;
        dest.attackMS  = attackMS;
        dest.releaseMS = releaseMS;
        dest.kneeDB    = kneeDB;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
    }
};
