#pragma once
#include "Faust_interpolate.hpp"


class Echo_PDJE : public FaustInterpolate{
protected:
    float EchoBps;
    float EchoFeedback;
};