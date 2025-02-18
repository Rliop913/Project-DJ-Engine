import("stdfaust.lib");
import("INTERPOLATOR.dsp");

FVAL = fvariable(int robotFreq, "robot.hpp");

lfo = os.osc(FVAL);

TP = _ * lfo;

RBSW = ITSW : max(0) : min(1);

ROBOT_RAIL = ef.dryWetMixer(RBSW, TP);

process = _,_ : ROBOT_RAIL, ROBOT_RAIL: _,_;