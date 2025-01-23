import("stdfaust.lib");
import("INTERPOLATOR.dsp");



// EQSW = hslider("SELECTOR", 0, 0, 2, 1);

EQSW = fvariable(int EQSelect, "");


LOW_GATE=fi.lowshelf(3, ITSW, 250);
MID_GATE=fi.peak_eq(ITSW, 2125, 1875);
HIGH_GATE=fi.highshelf(3, ITSW, 4000);

EQGATE = _ <: HIGH_GATE, MID_GATE, LOW_GATE : select3(EQSW) : _ ;

process = _,_:EQGATE,EQGATE:_,_;