import("stdfaust.lib");
// fvariable(int low_freq_value, "EFFECTS.hpp");
filter_order=12;//fvariable(int NumFilters, "EFFECTS.hpp");//12;

delay_samples = fvariable(int InterpolateSamples, "EFFECTS.hpp");
sw = fvariable(int InterpolateSW, ""); //0 for interpolate
Lowsw = fvariable(int LowFilterSW, "");
Highsw = fvariable(int HighFilterSW, "");

mlow = _ <: ba.line(delay_samples), _ : select2(sw) : _;
low_freq = fvariable(float LowFreq, "") : mlow ;
high_freq = fvariable(float HighFreq, "") : mlow;


LPF=ba.bypass1(Lowsw, fi.lowpass(filter_order, low_freq));
HPF=ba.bypass1(Highsw, fi.highpass(filter_order, high_freq));


FILTER_RAIL=HPF:LPF;


process=_,_:FILTER_RAIL,FILTER_RAIL:_,_;