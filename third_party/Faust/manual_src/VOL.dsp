import("stdfaust.lib");

VOL_CTR = _ * fvariable(float VolPower, "volMan.hpp");

GATE = _ <: VOL_CTR : _;

process = _,_:GATE,GATE:_,_;