import("stdfaust.lib");
import("INTERPOLATOR.dsp");

FVAL = ITSW : min(100) : max(1);

lfo = os.osc(FVAL);

TP = _ * lfo;

process = _,_ : TP, TP: _,_;