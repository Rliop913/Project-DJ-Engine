import("stdfaust.lib");


BPS = fvariable(float bps, "pannerMan.hpp");
GAIN = fvariable(float PGain, "");

OSCSIN = (os.oscsin(BPS) * GAIN);
PVAL = fvariable(float PannerDryWet, "") : min(1) : max(0);

PANNER=_,_:
ba.bypass1(ceil(-1 * OSCSIN), _ * (1.0 - OSCSIN)), 
ba.bypass1(ceil((1 * OSCSIN)), _ * (1.0 - (-1 * OSCSIN)))
:_*PVAL, _*PVAL;

PGATE = _ , _ <: PANNER , _ * (1.0 - PVAL) , _ * (1.0 - PVAL) :> _ , _;

process = _,_:PGATE:_,_;