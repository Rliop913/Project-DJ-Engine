import("stdfaust.lib");
import("INTERPOLATOR.dsp");
filter_order=12;

Lowsw = fvariable(int LowFilterSW, "filter.hpp");
Highsw = fvariable(int HighFilterSW, "");


LPF=ba.bypass1(Lowsw, fi.lowpass(filter_order, ITSW : min(24000) : max(1)));
HPF=ba.bypass1(Highsw, fi.highpass(filter_order, ITSW : min(24000) : max(1)));


FILTER_RAIL=HPF:LPF;


process=_,_:FILTER_RAIL,FILTER_RAIL:_,_;