import("stdfaust.lib");

LOW_POWER = fvariable(int EQLow, "eqMan.hpp");
MID_POWER = fvariable(int EQMid, "");
HIGH_POWER = fvariable(int EQHigh, "");

LOW_GATE=fi.lowshelf(3, LOW_POWER, 250);
MID_GATE=fi.peak_eq(MID_POWER, 2125, 1875);
HIGH_GATE=fi.highshelf(3, HIGH_POWER, 4000);

EQGATE = _ : HIGH_GATE : MID_GATE : LOW_GATE :  _ ;

process = _,_:EQGATE,EQGATE:_,_;
