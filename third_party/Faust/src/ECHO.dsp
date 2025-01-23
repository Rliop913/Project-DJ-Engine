import("stdfaust.lib");
import("INTERPOLATOR.dsp");

// VEcho = hslider("DUR", 1, 1, 10, 0.1);
// FEcho = hslider("FEEDBACK", 0.5, 0.0, 1.0, 0.1);

VEcho = fvariable(float EchoBps, "");
FEcho = fvariable(float EchoFeedback, "");

E_VAL = ITSW:min(1.0):max(0.0);


ECHO=_<: ef.echo(10, VEcho, FEcho) * E_VAL, _ * (1.0 - E_VAL) :> _;

process = _,_ : ECHO,ECHO:_,_;
