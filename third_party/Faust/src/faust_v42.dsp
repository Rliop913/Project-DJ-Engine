declare filename "faust_v42.dsp";
declare name "faust_v42";
declare filename "faust_v32.dsp";
declare name "faust_v32";
import("stdfaust.lib");
//1 is bypass enable
//---------INIT-------
filter_order=12;
low_freq=fvariable(int low_freq_value, "");
high_freq=fvariable(int high_freq_value, "");

flanger_switch = fvariable(int flanger_sw, "");
flanger_var = (os.hs_oscsin(fvariable(float flanger_bps,""),flanger_switch)+1.0)*30;
_flanger_gain = fvariable(float flanger_gain, "");
flanger_power = fvariable(float flanger_p, "");

l_f_s_switch=fvariable(int l_f_s_sw, "");
l_f_s_gain = fvariable(int l_f_s_gain_min_freq, "");
_l_f_s_bps = fvariable(float l_f_s_bps, "");
l_f_s_power = fvariable(float l_f_s_p, "");

trance_switch=fvariable(int trance_sw, "");
trance_gain=fvariable(float trance_gain, "");
trance_power = fvariable(float trance_p, "");

panner_switch = fvariable(int panner_sw, "");
_panner_gain = fvariable(float panner_gain, "");
_panner_bps = fvariable(float panner_bps, "");
panner_osc = (os.hs_oscsin(_panner_bps, panner_switch) * _panner_gain);
panner_power = fvariable(float panner_p, "");

distortion_switch=fvariable(int distortion_sw,"");
distortion_gain=fvariable(float distortion_gain,"");


low_filter_sw = fvariable(int lo_fi_sw, "");
high_filter_sw = fvariable(int hi_fi_sw, "");


_echo_sw = fvariable(int echo_sw, "");
_echo_dur = fvariable(float echo_dur, "");
_echo_feedback_var = fvariable(float echo_feedback_var, "");
echo_power = fvariable(float echo_p, "");

_phaser_sw = fvariable(int phaser_sw, "");
_phaser_bps = fvariable(float phaser_bps,"");
_phaser_gain = fvariable(float phaser_gain,"");
phaser_power = fvariable(float phaser_p, "");


trance_osc = (os.lf_sawpos_reset(fvariable(float trance_bps, ""),trance_switch)-trance_gain)*-1:ceil;

roll_table_size = 48000;
roll_read_itr_P = (+(1) %(roll_input_bpm)) ~_* (roll_switch<0.5);
roll_rec_itr_P = (+(1),roll_input_bpm-1:min:int ) ~_* (roll_switch<0.5);
roll_input_bpm=ba.tempo(fvariable(float roll_I_BPM, ""));
roll_switch = fvariable(int roll_sw, "");
roller_power= fvariable(float roller_p, "");

//-------CFX----------
LPF=ba.bypass1(low_filter_sw, fi.lowpass(filter_order, low_freq));
HPF=ba.bypass1(high_filter_sw, fi.highpass(filter_order, high_freq));
FILTER_RAIL=HPF:LPF;

DISTORTION_RAIL=ba.bypass1(distortion_switch,ef.cubicnl(distortion_gain,0));

//--------BFX-------------------
ECHO=_<:ef.echo(10, _echo_dur, _echo_feedback_var)*echo_power,_*(1.0-echo_power):>_;
ECHO_RAIL=ba.bypass1(_echo_sw,ECHO);

FLANGER=_,_:pf.flanger_stereo(flanger_var,flanger_var,flanger_var,_flanger_gain,0,0):_*flanger_power,_*flanger_power:_,_;
FLANGER__=_,_<:FLANGER, _*(1.0-flanger_power), _*(1.0-flanger_power):>_,_;
FLANGER_RAIL=ba.bypass2(flanger_switch, FLANGER__);

PHASER=_,_:pf.phaser2_stereo(8, 4500, 500, 1, 5000, _phaser_bps, 1, _phaser_gain, 0):_*phaser_power,_*phaser_power;
PHASER__=_,_<:PHASER, _*(1.0-phaser_power), _*(1.0-phaser_power) :>_,_;
PHASER_RAIL=_,_:ba.bypass2(_phaser_sw, PHASER__):_,_;

L_F_S=fi.lowpass(12, (((os.hs_oscsin(_l_f_s_bps, l_f_s_switch)) + 1.0)*(10000 - l_f_s_gain / 2) + l_f_s_gain));
L_F_S__=_<:L_F_S*l_f_s_power,_*(1.0-l_f_s_power):>_;
L_F_S_RAIL=ba.bypass1(l_f_s_switch, L_F_S__);

TRANCE=_*trance_osc;
TRANCE__=_<:TRANCE*trance_power,_*(1.0-trance_power):>_;
TRANCE_RAIL=ba.bypass1(trance_switch,TRANCE__);

//Panner >STEREO<
PANNER=_,_:ba.bypass1(ceil(-1 * panner_osc), _ * (1.0 - panner_osc)), ba.bypass1(ceil((1 * panner_osc)), _ * (1.0 - (-1 * panner_osc))):_*panner_power,_*panner_power;
PANNER__=_,_<:PANNER, _*(1.0-panner_power),_*(1.0-panner_power) :> _,_;
PANNER_RAIL=ba.bypass2(panner_switch, PANNER__);

//ROLL

ROLL = rwtable(roll_table_size,0.0,roll_rec_itr_P,_,roll_read_itr_P);
ROLL__ = _<:ROLL*roller_power,_*(1.0-roller_power):>_;
ROLL_RAIL = ROLL__;





//-----VOLUMES
FADER_GATE= _*fvariable(float fader, "");
TRIM_GATE= _*fvariable(float trim, "");
ALBUM_VOLUME_LIMIT= _*fvariable(float volume_limit, "");
LOW_GATE=ba.bypass1(fvariable(int eq_low_sw, ""),fi.lowshelf(3,fvariable(float eq_low_v, ""),500));
MID_GATE=ba.bypass1(fvariable(int eq_mid_sw, ""),fi.peak_eq(fvariable(float eq_mid_v, ""),2750,2250));
HIGH_GATE=ba.bypass1(fvariable(int eq_high_sw, ""),fi.highshelf(3,fvariable(float eq_hi_v, ""),5000));



PFLANGER_GATE=FLANGER_RAIL:PHASER_RAIL;




STEREO_GATE=PFLANGER_GATE:PANNER_RAIL;


EQ_=LOW_GATE,LOW_GATE:MID_GATE,MID_GATE:HIGH_GATE,HIGH_GATE;

CFX=FILTER_RAIL,FILTER_RAIL:ECHO_RAIL,ECHO_RAIL:DISTORTION_RAIL,DISTORTION_RAIL;

BFX=L_F_S_RAIL,L_F_S_RAIL:TRANCE_RAIL,TRANCE_RAIL:STEREO_GATE;

MEMORY_FX=_,_:ROLL_RAIL,ROLL_RAIL:_,_;

VOLUME=TRIM_GATE,TRIM_GATE:FADER_GATE,FADER_GATE:ALBUM_VOLUME_LIMIT,ALBUM_VOLUME_LIMIT;



process = _,_:MEMORY_FX:VOLUME:EQ_:CFX:BFX:_,_;




