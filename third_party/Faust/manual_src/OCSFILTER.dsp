import("stdfaust.lib");

filter_order=12;


HL_SELECTOR = fvariable(int ocsFilterHighLowSW, "ocsFilterMan.hpp");

FREQ_MIDDLE = fvariable(int middleFreq, "");

FREQ_RANGE_HALF = fvariable(int rangeFreqHalf, "");

BPS = fvariable(float bps, "");

OCS = (os.osccos(BPS) + 1.0) * (FREQ_RANGE_HALF) + FREQ_MIDDLE : max(1) : min(24000);

LowOCS=fi.lowpass(12, OCS);
HighOCS=fi.highpass(12, OCS);

HLOCS = _<: HighOCS, LowOCS : select2(HL_SELECTOR);

O_Val = fvariable(float OCSFilterDryWet, "") :max(0):min(1);
LFilt = _ <: HLOCS * O_Val , _*(1.0 - O_Val) :> _;


process=_,_:LFilt,LFilt:_,_;