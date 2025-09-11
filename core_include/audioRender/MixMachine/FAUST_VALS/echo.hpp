#pragma once
#include "Faust_interpolate.hpp"

class Echo_PDJE : public FaustInterpolate {
  public:
    float EchoBps;
    float EchoFeedback;

    template <typename Duck>
    void
    copyDatas(Duck &dest)
    {
        dest.EchoBps      = EchoBps;
        dest.EchoFeedback = EchoFeedback;
    }

    template <typename Duck>
    void
    copySetting(Duck &dest)
    {
        copyDatas(dest);
        copyInterpolates(dest);
    }
};