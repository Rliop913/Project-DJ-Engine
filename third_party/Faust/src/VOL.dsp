import("stdfaust.lib");
import("INTERPOLATOR.dsp");

VOL_CTR = _*ITSW;

GATE = _ <: VOL_CTR : _;

process = _,_:GATE,GATE:_,_;