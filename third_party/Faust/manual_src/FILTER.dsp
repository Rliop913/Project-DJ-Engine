import("stdfaust.lib");

filter_order=12;

HL_SELECTOR = fvariable(int HLswitch, "filterMan.hpp");

ITSW = fvariable(int Filterfreq, "");

LPF=fi.lowpass(filter_order, ITSW : min(24000) : max(1));
HPF=fi.highpass(filter_order, ITSW : min(24000) : max(1));


FILTER_RAIL= _<: HPF, LPF : select2(HL_SELECTOR);

process=_,_:FILTER_RAIL,FILTER_RAIL:_,_;