import("stdfaust.lib");
import("INTERPOLATOR.dsp");

DVAL = ITSW:min(1):max(0);

D_RAIL=ef.cubicnl(DVAL,0);

process = _,_:D_RAIL, D_RAIL: _,_;