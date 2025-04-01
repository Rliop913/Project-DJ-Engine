import("stdfaust.lib");


FVAL = fvariable(int robotFreq, "robotMan.hpp");

lfo = os.osc(FVAL);

TP = _ * lfo;

RBSW = fvariable(float RobotDryWet, "") : max(0) : min(1);

ROBOT_RAIL = ef.dryWetMixer(RBSW, TP);

process = _,_ : ROBOT_RAIL, ROBOT_RAIL: _,_;