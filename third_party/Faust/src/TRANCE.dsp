import("stdfaust.lib");
import("INTERPOLATOR.dsp");

BPS = fvariable(float bps, "");
TGAIN = fvariable(float gain, "");

GATE = (os.lf_sawpos(BPS) - TGAIN) * -1 : ceil;

TRANCE= _ * GATE;
TVAL = ITSW : min(1) : max(0);
TGATE=_ <: TRANCE * TVAL, _ * (1.0 - TVAL) :> _;

process = _,_: TGATE, TGATE :_,_;