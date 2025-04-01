import("stdfaust.lib");

VEcho = fvariable(float EchoBps, "echoMan.hpp");
FEcho = fvariable(float EchoFeedback, "");

E_VAL = fvariable(float EchoDryWet, ""):min(1.0):max(0.0);


ECHO=_<: ef.echo(10, VEcho, FEcho) * E_VAL, _ * (1.0 - E_VAL) :> _;

process = _,_ : ECHO,ECHO:_,_;
