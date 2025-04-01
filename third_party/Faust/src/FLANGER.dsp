import("stdfaust.lib");
import("INTERPOLATOR.dsp");

BPS = fvariable(float bps, "flanger.hpp");


OSCL = (os.oscsin(BPS + 0.1)+1.0) * 10;
OSCR = (os.oscsin(BPS)+1.0) * 10;

FVAL = ITSW : min(1) : max(0);

FLANGER=_,_:pf.flanger_stereo(20, OSCL, OSCR, 1.0, 0.8, 0):_*FVAL,_*FVAL;
FGATE = _,_ <: FLANGER , _*(1.0-FVAL), _*(1.0-FVAL) :> _,_;
process = _,_:FGATE:_,_;
