import("stdfaust.lib");
import("INTERPOLATOR.dsp");

BPS = fvariable(float bps, "");

PVAL = ITSW : max(0) : min(1);

PHASER=_,_:pf.phaser2_stereo(6, 4500, 500, 1, 5000, BPS, PVAL, 0.3, 0);

process = _,_:PHASER:_,_;