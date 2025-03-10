import("stdfaust.lib");

EQSW = fvariable(int EQSelect, "eqMan.hpp");

ITSW = fvariable(int EQPower, "");



LOW_GATE=fi.lowshelf(3, ITSW, 250);
MID_GATE=fi.peak_eq(ITSW, 2125, 1875);
HIGH_GATE=fi.highshelf(3, ITSW, 4000);

EQGATE = _ <: HIGH_GATE, MID_GATE, LOW_GATE : select3(EQSW) : _ ;

process = _,_:EQGATE,EQGATE:_,_;