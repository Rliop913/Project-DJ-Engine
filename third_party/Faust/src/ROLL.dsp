import("stdfaust.lib");
import("INTERPOLATOR.dsp");
roll_table_size = 96000;
roll_read_itr_P = (+(1) %(roll_input_bpm)) ~_* (roll_switch<0.5);
roll_rec_itr_P = (+(1),roll_input_bpm-1:min:int ) ~_* (roll_switch<0.5);
roll_input_bpm=ba.tempo(fvariable(float RollBpm, "roll.hpp"));
roll_switch = fvariable(int RollSwitch, "");
roller_power= ITSW:min(1):max(0);

ROLL = rwtable(roll_table_size,0.0,roll_rec_itr_P,_,roll_read_itr_P);
ROLL_RAIL = _<:ROLL*roller_power,_*(1.0-roller_power):>_;


MEMORY_FX=_,_:ROLL_RAIL,ROLL_RAIL:_,_;

process = _,_:MEMORY_FX:_,_;