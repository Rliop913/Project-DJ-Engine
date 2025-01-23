import("stdfaust.lib");
import("INTERPOLATOR.dsp");

S = fvariable(float strength, "");
THR = fvariable(int threshDB, "");
ATT = fvariable(int attackMS, "");
REL = fvariable(int releaseMS, "");
KNEE = fvariable(int kneeDB, "");

PRE = 0;

compress = 
co.peak_compression_gain_mono(
    S,
    THR,
    ATT,
    REL,
    KNEE,
    PRE
);

changePos(A,B,C,D) = A,C,B,D;

process = _,_ <:_,_,compress,compress:_,_,_,_: changePos:*,*:_,_;