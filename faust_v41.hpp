/* ------------------------------------------------------------
name: "faust_v41", "faust_v41"
Code generated with Faust 2.71.0 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__
#pragma warning(push)
#pragma warning(disable: 26495)
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Faust_engine
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {
	
  private:
	
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			table[i1] = 0.0f;
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

class mydspSIG1 {
	
  private:
	
	int iVec3[2];
	int iRec25[2];
	
  public:
	
	int getNumInputsmydspSIG1() {
		return 0;
	}
	int getNumOutputsmydspSIG1() {
		return 1;
	}
	
	void instanceInitmydspSIG1(int sample_rate) {
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			iVec3[l28] = 0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			iRec25[l29] = 0;
		}
	}
	
	void fillmydspSIG1(int count, float* table) {
		for (int i2 = 0; i2 < count; i2 = i2 + 1) {
			iVec3[0] = 1;
			iRec25[0] = (iVec3[1] + iRec25[1]) % 65536;
			table[i2] = std::sin(9.58738e-05f * float(iRec25[0]));
			iVec3[1] = iVec3[0];
			iRec25[1] = iRec25[0];
		}
	}

};

static mydspSIG1* newmydspSIG1() { return (mydspSIG1*)new mydspSIG1(); }
static void deletemydspSIG1(mydspSIG1* dsp) { delete dsp; }

static float mydsp_faustpower2_f(float value) {
	return value * value;
}
static float ftbl1mydspSIG1[65536];

class Faust_engine {
public:
	//---------fader&trim---------//
	float volume_limit = 0.7f;
	float trim = 1.0f;
	float fader = 1.0f;
	void set_trim(const float& value) { trim = value; }
	void set_fader(const float& value) { fader = value; };
	//----------switches----------//
	int lo_fi_sw = 1; void filter_low_sw(const bool& turn_on) { turn_on ? lo_fi_sw = 0 : lo_fi_sw = 1; }
	int hi_fi_sw = 1; void filter_high_sw(const bool& turn_on) { turn_on ? hi_fi_sw = 0 : hi_fi_sw = 1; }
	int eq_mid_sw = 1; void EQ_mid_sw(const bool& turn_on) { turn_on ? eq_mid_sw = 0 : eq_mid_sw = 1; }
	int eq_low_sw = 1; void EQ_low_sw(const bool& turn_on) { turn_on ? eq_low_sw = 0 : eq_low_sw = 1; }
	int eq_high_sw = 1; void EQ_high_sw(const bool& turn_on) { turn_on ? eq_high_sw = 0 : eq_high_sw = 1; }
	int echo_sw = 1; void _echo_sw(const bool& turn_on) { turn_on ? echo_sw = 0 : echo_sw = 1; }
	int l_f_s_sw = 1; void low_siren_sw(const bool& turn_on) { turn_on ? l_f_s_sw = 0 : l_f_s_sw = 1; }
	int phaser_sw = 1; void _phaser_sw(const bool& turn_on) { turn_on ? phaser_sw = 0 : phaser_sw = 1; }
	int flanger_sw = 1; void _flanger_sw(const bool& turn_on) { turn_on ? flanger_sw = 0 : flanger_sw = 1; }
	int trance_sw = 1; void _trance_sw(const bool& turn_on) { turn_on ? trance_sw = 0 : trance_sw = 1; }
	int panner_sw = 1; void _panner_sw(const bool& turn_on) { turn_on ? panner_sw = 0 : panner_sw = 1; }
	int distortion_sw = 1; void _distortion_sw(const bool& turn_on) { turn_on ? distortion_sw = 0 : distortion_sw = 1; }
	int roll_sw = 1; void _roll_sw(const bool& turn_on) { turn_on ? roll_sw = 0 : roll_sw = 1; }

	//--------values------------//

	int low_freq_value = 500; void set_filter_low_freq_value(int value) { low_freq_value = value; }
	int high_freq_value = 500; void set_filter_high_freq_value(int value) { high_freq_value = value; }

	float eq_mid_v = 0; void set_eq_mid_value(const float& value) { eq_mid_v = value; }
	float eq_low_v = 0; void set_eq_low_value(const float& value) { eq_low_v = value; }
	float eq_hi_v = 0; void set_eq_high_value(const float& value) { eq_hi_v = value; }

	float echo_dur = 0.5f; void set_echo_dur_value(const float& value) { echo_dur = 1.0f / value; }
	float echo_feedback_var = 0.1f; void set_echo_feedback_value(const float& value) { echo_feedback_var = value; }
	float echo_p = 0.0f; void set_echo_power(const float& value) { value > 1.0f ? echo_p = 1.0f : (value < 0 ? echo_p = 0.0f : echo_p = value); }


	float l_f_s_gain_min_freq = 1.0f; void set_l_f_s_gain_min_freq(int value) { value < 0 ? l_f_s_gain_min_freq = 0 : l_f_s_gain_min_freq = float(value); }
	float l_f_s_bps = 1.0f; void set_l_f_s_bps_value(const float& bps) { l_f_s_bps = bps; }
	float l_f_s_p = 0.0f; void set_l_f_s_power(const float& value) { value > 1.0f ? l_f_s_p = 1.0f : (value < 0 ? l_f_s_p = 0.0f : l_f_s_p = value); }

	float flanger_bps = 1.0f; void set_flanger_bps_value(const float& bps) { flanger_bps = bps; }
	float flanger_gain = 1.0f; void set_flanger_gain_value(const float& value) { value > 1.0f ? flanger_gain = 1.0f : (value < 0 ? flanger_gain = 0 : flanger_gain = value); }
	float flanger_p = 0.0f; void set_flanger_power(const float& value) { value > 1.0f ? flanger_p = 1.0f : (value < 0 ? flanger_p = 0.0f : flanger_p = value); }


	float phaser_bps = 1.0f; void set_phaser_bps_value(const float& bps) { phaser_bps = bps; }
	float phaser_gain = 1.0f; void set_phaser_gain_value(const float& value) { value > 1.0f ? phaser_gain = 1.0f : (value < -1.0 ? phaser_gain = -1.0f : phaser_gain = value); }
	float phaser_p = 0.0f; void set_phaser_power(const float& value) { value > 1.0f ? phaser_p = 1.0f : (value < 0 ? phaser_p = 0.0f : phaser_p = value); }

	float panner_bps = 1.0f; void set_panner_bps(const float& bps) { panner_bps = bps; }
	float panner_gain = 1.0f; void set_panner_gain(const float& value) { value > 1.0f ? panner_gain = 1.0f : (value < 0 ? panner_gain = 0.0f : panner_gain = value); }
	float panner_p = 0.0f; void set_panner_power(const float& value) { value > 1.0f ? panner_p = 1.0f : (value < 0 ? panner_p = 0.0f : panner_p = value); }

	float distortion_gain = 1.0f; void set_distortion_gain(const float& value) { value > 1.0f ? distortion_gain = 1.0f : (value < 0 ? distortion_gain = 0.0f : distortion_gain = value); }

	float trance_bps = 1.0f; void set_trance_bps(const float& bps) { trance_bps = bps; }
	float trance_gain = 1.0f; void set_trance_gain(const float& value) { value > 1.0f ? trance_gain = 1.0f : (value < 0 ? trance_gain = 0.0f : trance_gain = value); }
	float trance_p = 0.0f; void set_trance_power(const float& value) { value > 1.0f ? trance_p = 1.0f : (value < 0 ? trance_p = 0.0f : trance_p = value); }

	float roller_p = 0.0f; void set_roller_power(const float& value) { value > 1.0f ? roller_p = 1.0f : (value < 0 ? roller_p = 0.0f : roller_p = value); }
	float roll_I_BPM = 0.0f; void set_roller_bpm(const float& value) { roll_I_BPM = value; }
	//-------value_getter------//
 private:
	
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0[2];
	float fConst2;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst11;
	float fConst12;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float ftbl0[48000];
	float fConst19;
	float fRec18[2];
	float fRec19[2];
	float fVec1[2];
	float fConst20;
	float fConst21;
	float fRec17[2];
	float fRec16[3];
	float fRec21[2];
	float fRec20[3];
	float fRec15[3];
	float fVec2[2];
	float fConst22;
	float fConst23;
	float fRec14[2];
	float fRec13[3];
	float fRec23[2];
	float fRec22[3];
	float fRec12[3];
	float fRec11[3];
	float fRec10[3];
	float fRec9[3];
	float fRec8[3];
	float fRec7[3];
	float fRec6[3];
	float fRec5[3];
	float fRec4[3];
	float fRec3[3];
	float fRec2[3];
	float fRec1[3];
	int IOTA0;
	float fConst24;
	float fRec24[2097152];
	float fRec26[2];
	float fRec32[3];
	float fRec31[3];
	float fRec30[3];
	float fRec29[3];
	float fRec28[3];
	float fRec27[3];
	float fVec4[64];
	float fRec34[2];
	float fConst26;
	float fConst27;
	float fRec44[2];
	float fRec45[2];
	float fConst28;
	float fRec43[3];
	float fRec42[3];
	float fRec41[3];
	float fRec40[3];
	float fRec39[3];
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[2];
	float fRec46[2];
	float ftbl2[48000];
	float fVec5[2];
	float fRec63[2];
	float fRec62[3];
	float fRec65[2];
	float fRec64[3];
	float fRec61[3];
	float fVec6[2];
	float fRec60[2];
	float fRec59[3];
	float fRec67[2];
	float fRec66[3];
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	float fRec52[3];
	float fRec51[3];
	float fRec50[3];
	float fRec49[3];
	float fRec48[3];
	float fRec47[3];
	float fRec68[2097152];
	float fRec74[3];
	float fRec73[3];
	float fRec72[3];
	float fRec71[3];
	float fRec70[3];
	float fRec69[3];
	float fVec7[64];
	float fRec84[3];
	float fRec83[3];
	float fRec82[3];
	float fRec81[3];
	float fRec80[3];
	float fRec79[3];
	float fRec78[3];
	float fRec77[3];
	float fRec76[2];
	
 public:
	 Faust_engine() {}


	int getNumInputs() {
		return 2;
	}
	int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG1* sig1 = newmydspSIG1();
		sig1->instanceInitmydspSIG1(sample_rate);
		sig1->fillmydspSIG1(65536, ftbl1mydspSIG1);
		deletemydspSIG1(sig1);
	}
	
	void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 3.1415927f / fConst0;
		float fConst3 = std::tan(15707.963f / fConst0);
		fConst4 = 1.0f / mydsp_faustpower2_f(fConst3);
		fConst5 = 2.0f * (1.0f - fConst4);
		fConst6 = 1.0f / fConst3;
		fConst7 = (fConst6 + -1.0f) / fConst3 + 1.0f;
		fConst8 = 1.0f / ((fConst6 + 1.0f) / fConst3 + 1.0f);
		fConst9 = 7068.5835f / (fConst0 * std::sin(17278.76f / fConst0));
		float fConst10 = std::tan(8639.38f / fConst0);
		fConst11 = 1.0f / fConst10;
		fConst12 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fConst10));
		float fConst13 = std::tan(1570.7964f / fConst0);
		fConst14 = 1.0f / mydsp_faustpower2_f(fConst13);
		fConst15 = 2.0f * (1.0f - fConst14);
		fConst16 = 1.0f / fConst13;
		fConst17 = (fConst16 + -1.0f) / fConst13 + 1.0f;
		fConst18 = 1.0f / ((fConst16 + 1.0f) / fConst13 + 1.0f);
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(48000, ftbl0);
		fConst19 = 6e+01f * fConst0;
		fConst20 = 1.0f - fConst16;
		fConst21 = 1.0f / (fConst16 + 1.0f);
		fConst22 = 1.0f - fConst6;
		fConst23 = 1.0f / (fConst6 + 1.0f);
		fConst24 = 1e+01f * fConst0;
		float fConst25 = std::exp(-(14137.167f / fConst0));
		fConst26 = mydsp_faustpower2_f(fConst25);
		fConst27 = 6.2831855f / fConst0;
		fConst28 = 2.0f * fConst25;
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(48000, ftbl2);
		deletemydspSIG0(sig0);
	}
	
	
	void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec18[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec19[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec17[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec16[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec21[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec20[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = l9 + 1) {
			fRec15[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fVec2[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec14[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec13[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec23[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec22[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = l15 + 1) {
			fRec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec11[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec10[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec9[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec8[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec7[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec6[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec5[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec4[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec3[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec2[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec1[l26] = 0.0f;
		}
		IOTA0 = 0;
		for (int l27 = 0; l27 < 2097152; l27 = l27 + 1) {
			fRec24[l27] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec26[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = l31 + 1) {
			fRec32[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fRec31[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec30[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec29[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec28[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec27[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 64; l37 = l37 + 1) {
			fVec4[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec34[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec44[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec45[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec43[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec42[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec41[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec40[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec39[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec38[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec37[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec36[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fRec35[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec46[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fVec5[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec63[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec62[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec65[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 3; l55 = l55 + 1) {
			fRec64[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec61[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec6[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec60[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec59[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec67[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec66[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec58[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec57[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fRec56[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec55[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec54[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec53[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec52[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec51[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec50[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec49[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 3; l72 = l72 + 1) {
			fRec48[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 3; l73 = l73 + 1) {
			fRec47[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2097152; l74 = l74 + 1) {
			fRec68[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec74[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec73[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec72[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec71[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec70[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec69[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 64; l81 = l81 + 1) {
			fVec7[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
			fRec84[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec83[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec82[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 3; l85 = l85 + 1) {
			fRec81[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec80[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec79[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fRec78[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 3; l89 = l89 + 1) {
			fRec77[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fRec76[l90] = 0.0f;
		}
	}
	
	void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceClear();
	}
	
	Faust_engine* clone() {
		return new Faust_engine();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	
	void compute(int count, FAUSTFLOAT* RESTRICT inputfirst, FAUSTFLOAT* RESTRICT inputsecond, FAUSTFLOAT* RESTRICT outputfirst, FAUSTFLOAT* RESTRICT outputsecond) {
		FAUSTFLOAT* input0 = inputfirst;
		FAUSTFLOAT* input1 = inputsecond;
		FAUSTFLOAT* output0 = outputfirst;
		FAUSTFLOAT* output1 = outputsecond;
		int iSlow0 = panner_sw;
		int iSlow1 = phaser_sw;
		int iSlow2 = flanger_sw;
		int iSlow3 = trance_sw;
		float fSlow4 = fConst1 * trance_bps;
		float fSlow5 = trance_gain;
		float fSlow6 = trance_p;
		int iSlow7 = l_f_s_sw;
		int iSlow8 = distortion_sw;
		int iSlow9 = echo_sw;
		int iSlow10 = lo_fi_sw;
		float fSlow11 = std::tan(fConst2 * float(low_freq_value));
		float fSlow12 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fSlow11));
		float fSlow13 = 1.0f / fSlow11;
		float fSlow14 = (fSlow13 + -0.26105237f) / fSlow11 + 1.0f;
		float fSlow15 = 1.0f / ((fSlow13 + 0.26105237f) / fSlow11 + 1.0f);
		float fSlow16 = (fSlow13 + -0.76536685f) / fSlow11 + 1.0f;
		float fSlow17 = 1.0f / ((fSlow13 + 0.76536685f) / fSlow11 + 1.0f);
		float fSlow18 = (fSlow13 + -1.2175229f) / fSlow11 + 1.0f;
		float fSlow19 = 1.0f / ((fSlow13 + 1.2175229f) / fSlow11 + 1.0f);
		float fSlow20 = (fSlow13 + -1.5867066f) / fSlow11 + 1.0f;
		float fSlow21 = 1.0f / ((fSlow13 + 1.5867066f) / fSlow11 + 1.0f);
		float fSlow22 = (fSlow13 + -1.847759f) / fSlow11 + 1.0f;
		float fSlow23 = 1.0f / ((fSlow13 + 1.847759f) / fSlow11 + 1.0f);
		float fSlow24 = (fSlow13 + -1.9828898f) / fSlow11 + 1.0f;
		float fSlow25 = 1.0f / ((fSlow13 + 1.9828898f) / fSlow11 + 1.0f);
		int iSlow26 = hi_fi_sw;
		float fSlow27 = std::tan(fConst2 * float(high_freq_value));
		float fSlow28 = mydsp_faustpower2_f(fSlow27);
		float fSlow29 = 2.0f * (1.0f - 1.0f / fSlow28);
		float fSlow30 = 1.0f / fSlow27;
		float fSlow31 = (fSlow30 + -0.26105237f) / fSlow27 + 1.0f;
		float fSlow32 = (fSlow30 + 0.26105237f) / fSlow27 + 1.0f;
		float fSlow33 = 1.0f / fSlow32;
		float fSlow34 = (fSlow30 + -0.76536685f) / fSlow27 + 1.0f;
		float fSlow35 = (fSlow30 + 0.76536685f) / fSlow27 + 1.0f;
		float fSlow36 = 1.0f / fSlow35;
		float fSlow37 = (fSlow30 + -1.2175229f) / fSlow27 + 1.0f;
		float fSlow38 = (fSlow30 + 1.2175229f) / fSlow27 + 1.0f;
		float fSlow39 = 1.0f / fSlow38;
		float fSlow40 = (fSlow30 + -1.5867066f) / fSlow27 + 1.0f;
		float fSlow41 = (fSlow30 + 1.5867066f) / fSlow27 + 1.0f;
		float fSlow42 = 1.0f / fSlow41;
		float fSlow43 = (fSlow30 + -1.847759f) / fSlow27 + 1.0f;
		float fSlow44 = (fSlow30 + 1.847759f) / fSlow27 + 1.0f;
		float fSlow45 = 1.0f / fSlow44;
		float fSlow46 = (fSlow30 + -1.9828898f) / fSlow27 + 1.0f;
		float fSlow47 = (fSlow30 + 1.9828898f) / fSlow27 + 1.0f;
		float fSlow48 = 1.0f / fSlow47;
		int iSlow49 = eq_high_sw;
		int iSlow50 = eq_mid_sw;
		float fSlow51 = eq_mid_v;
		int iSlow52 = fSlow51 > 0.0f;
		float fSlow53 = fConst9 * std::pow(1e+01f, 0.05f * std::fabs(fSlow51));
		float fSlow54 = ((iSlow52) ? fConst9 : fSlow53);
		float fSlow55 = fConst11 * (fConst11 + fSlow54) + 1.0f;
		float fSlow56 = ((iSlow52) ? fSlow53 : fConst9);
		float fSlow57 = fConst11 * (fConst11 - fSlow56) + 1.0f;
		float fSlow58 = fConst11 * (fConst11 - fSlow54) + 1.0f;
		int iSlow59 = eq_low_sw;
		int iSlow60 = roll_sw;
		float fSlow61 = roller_p;
		float fSlow62 = 1.0f - fSlow61;
		float fSlow63 = float(1 - iSlow60);
		float fSlow64 = fConst19 / roll_I_BPM;
		float fSlow65 = fSlow64 + -1.0f;
		float fSlow66 = trim * fader * volume_limit;
		float fSlow67 = std::pow(1e+01f, 0.05f * eq_low_v);
		float fSlow68 = fConst11 * (fConst11 + fSlow56) + 1.0f;
		float fSlow69 = fConst4 * std::pow(1e+01f, 0.05f * eq_hi_v);
		float fSlow70 = 1.0f / (fSlow28 * fSlow47);
		float fSlow71 = 1.0f / (fSlow28 * fSlow44);
		float fSlow72 = 1.0f / (fSlow28 * fSlow41);
		float fSlow73 = 1.0f / (fSlow28 * fSlow38);
		float fSlow74 = 1.0f / (fSlow28 * fSlow35);
		float fSlow75 = 1.0f / (fSlow28 * fSlow32);
		float fSlow76 = echo_p;
		float fSlow77 = 1.0f - fSlow76;
		int iSlow78 = int(std::min<float>(fConst24, std::max<float>(0.0f, fConst0 * echo_dur))) + 1;
		float fSlow79 = echo_feedback_var;
		float fSlow80 = std::pow(1e+01f, 2.0f * distortion_gain);
		float fSlow81 = l_f_s_p;
		float fSlow82 = 1.0f - fSlow81;
		float fSlow83 = fConst1 * l_f_s_bps;
		float fSlow84 = float(l_f_s_gain_min_freq);
		float fSlow85 = 1e+04f - 0.5f * fSlow84;
		float fSlow86 = flanger_p;
		float fSlow87 = 1.0f - fSlow86;
		float fSlow88 = 1.0f - fSlow6;
		float fSlow89 = fConst1 * flanger_bps;
		float fSlow90 = flanger_gain;
		float fSlow91 = 0.5f * fSlow86;
		float fSlow92 = phaser_p;
		float fSlow93 = 1.0f - fSlow92;
		float fSlow94 = fConst27 * phaser_bps;
		float fSlow95 = std::cos(fSlow94);
		float fSlow96 = std::sin(fSlow94);
		float fSlow97 = phaser_gain;
		float fSlow98 = 0.5f * fSlow92;
		float fSlow99 = panner_p;
		float fSlow100 = 1.0f - fSlow99;
		float fSlow101 = fConst1 * panner_bps;
		float fSlow102 = panner_gain;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			int iTemp0 = 1 - iVec0[1];
			float fTemp1 = ((iTemp0 | iSlow3) ? 0.0f : fSlow4 + fRec0[1]);
			fRec0[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = std::ceil(fSlow5 - fRec0[0]);
			float fTemp3 = 1.0f - fSlow6 * (1.0f - fTemp2);
			float fTemp4 = fConst12 * fRec15[1];
			float fTemp5 = float(input0[i0]);
			float fTemp6 = ((iSlow60) ? 0.0f : fTemp5);
			fRec18[0] = std::min<float>(fSlow63 * fRec18[1] + 1.0f, fSlow65);
			int iTemp7 = std::max<int>(0, std::min<int>(int(fRec18[0]), 47999));
			ftbl0[iTemp7] = fTemp6;
			fRec19[0] = std::fmod(fSlow63 * fRec19[1] + 1.0f, fSlow64);
			int iTemp8 = std::max<int>(0, std::min<int>(int(fRec19[0]), 47999));
			float fTemp9 = fSlow66 * ((iSlow60) ? fTemp5 : fSlow61 * ftbl0[iTemp8] + fSlow62 * fTemp6);
			float fTemp10 = ((iSlow59) ? 0.0f : fTemp9);
			fVec1[0] = fTemp10;
			fRec17[0] = -(fConst21 * (fConst20 * fRec17[1] - (fTemp10 + fVec1[1])));
			fRec16[0] = fRec17[0] - fConst18 * (fConst17 * fRec16[2] + fConst15 * fRec16[1]);
			float fTemp11 = fSlow67 * (fRec16[2] + fRec16[0] + 2.0f * fRec16[1]);
			fRec21[0] = -(fConst21 * (fConst20 * fRec21[1] - fConst16 * (fTemp10 - fVec1[1])));
			fRec20[0] = fRec21[0] - fConst18 * (fConst17 * fRec20[2] + fConst15 * fRec20[1]);
			float fTemp12 = 2.0f * fRec20[1];
			float fTemp13 = ((iSlow59) ? fTemp9 : fConst18 * (fConst14 * (fRec20[2] + (fRec20[0] - fTemp12)) + fTemp11));
			fRec15[0] = ((iSlow50) ? 0.0f : fTemp13) - (fRec15[2] * fSlow58 + fTemp4) / fSlow55;
			float fTemp14 = (fTemp4 + fRec15[0] * fSlow68 + fRec15[2] * fSlow57) / fSlow55;
			float fTemp15 = ((iSlow50) ? fTemp13 : fTemp14);
			float fTemp16 = ((iSlow49) ? 0.0f : fTemp15);
			fVec2[0] = fTemp16;
			fRec14[0] = -(fConst23 * (fConst22 * fRec14[1] - fConst6 * (fTemp16 - fVec2[1])));
			fRec13[0] = fRec14[0] - fConst8 * (fConst7 * fRec13[2] + fConst5 * fRec13[1]);
			float fTemp17 = 2.0f * fRec13[1];
			fRec23[0] = -(fConst23 * (fConst22 * fRec23[1] - (fTemp16 + fVec2[1])));
			fRec22[0] = fRec23[0] - fConst8 * (fConst7 * fRec22[2] + fConst5 * fRec22[1]);
			float fTemp18 = fRec22[2] + fRec22[0] + 2.0f * fRec22[1];
			float fTemp19 = ((iSlow49) ? fTemp15 : fConst8 * (fTemp18 + fSlow69 * (fRec13[2] + (fRec13[0] - fTemp17))));
			fRec12[0] = ((iSlow26) ? 0.0f : fTemp19) - fSlow48 * (fSlow46 * fRec12[2] + fSlow29 * fRec12[1]);
			fRec11[0] = fSlow70 * (fRec12[2] + (fRec12[0] - 2.0f * fRec12[1])) - fSlow45 * (fSlow43 * fRec11[2] + fSlow29 * fRec11[1]);
			fRec10[0] = fSlow71 * (fRec11[2] + (fRec11[0] - 2.0f * fRec11[1])) - fSlow42 * (fSlow40 * fRec10[2] + fSlow29 * fRec10[1]);
			fRec9[0] = fSlow72 * (fRec10[2] + (fRec10[0] - 2.0f * fRec10[1])) - fSlow39 * (fSlow37 * fRec9[2] + fSlow29 * fRec9[1]);
			fRec8[0] = fSlow73 * (fRec9[2] + (fRec9[0] - 2.0f * fRec9[1])) - fSlow36 * (fSlow34 * fRec8[2] + fSlow29 * fRec8[1]);
			fRec7[0] = fSlow74 * (fRec8[2] + (fRec8[0] - 2.0f * fRec8[1])) - fSlow33 * (fSlow31 * fRec7[2] + fSlow29 * fRec7[1]);
			float fTemp20 = 2.0f * fRec7[1];
			float fTemp21 = ((iSlow26) ? fTemp19 : fSlow75 * (fRec7[2] + (fRec7[0] - fTemp20)));
			fRec6[0] = ((iSlow10) ? 0.0f : fTemp21) - fSlow25 * (fSlow24 * fRec6[2] + fSlow12 * fRec6[1]);
			fRec5[0] = fSlow25 * (fRec6[2] + fRec6[0] + 2.0f * fRec6[1]) - fSlow23 * (fSlow22 * fRec5[2] + fSlow12 * fRec5[1]);
			fRec4[0] = fSlow23 * (fRec5[2] + fRec5[0] + 2.0f * fRec5[1]) - fSlow21 * (fSlow20 * fRec4[2] + fSlow12 * fRec4[1]);
			fRec3[0] = fSlow21 * (fRec4[2] + fRec4[0] + 2.0f * fRec4[1]) - fSlow19 * (fSlow18 * fRec3[2] + fSlow12 * fRec3[1]);
			fRec2[0] = fSlow19 * (fRec3[2] + fRec3[0] + 2.0f * fRec3[1]) - fSlow17 * (fSlow16 * fRec2[2] + fSlow12 * fRec2[1]);
			fRec1[0] = fSlow17 * (fRec2[2] + fRec2[0] + 2.0f * fRec2[1]) - fSlow15 * (fSlow14 * fRec1[2] + fSlow12 * fRec1[1]);
			float fTemp22 = fSlow15 * (fRec1[2] + fRec1[0] + 2.0f * fRec1[1]);
			float fTemp23 = ((iSlow10) ? ((iSlow26) ? ((iSlow49) ? ((iSlow50) ? ((iSlow59) ? fTemp9 : fConst18 * (fTemp11 + fConst14 * (fRec20[0] + fRec20[2] - fTemp12))) : fTemp14) : fConst8 * (fTemp18 + fSlow69 * (fRec13[0] + fRec13[2] - fTemp17))) : fSlow75 * (fRec7[0] + fRec7[2] - fTemp20)) : fTemp22);
			float fTemp24 = ((iSlow10) ? fTemp21 : fTemp22);
			float fTemp25 = ((iSlow9) ? 0.0f : fTemp24);
			fRec24[IOTA0 & 2097151] = fSlow79 * fRec24[(IOTA0 - iSlow78) & 2097151] + fTemp25;
			float fTemp26 = fSlow76 * fRec24[IOTA0 & 2097151];
			float fTemp27 = ((iSlow9) ? fTemp23 : fTemp26 + fSlow77 * ((iSlow9) ? 0.0f : fTemp23));
			float fTemp28 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow80 * ((iSlow8) ? 0.0f : fTemp27)));
			float fTemp29 = ((iSlow8) ? fTemp27 : fTemp28 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp28)));
			float fTemp30 = ((iTemp0 | iSlow7) ? 0.0f : fSlow83 + fRec26[1]);
			fRec26[0] = fTemp30 - std::floor(fTemp30);
			float fTemp31 = std::tan(fConst2 * (fSlow84 + fSlow85 * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec26[0]), 65535))] + 1.0f)));
			float fTemp32 = 1.0f / fTemp31;
			float fTemp33 = (fTemp32 + 0.26105237f) / fTemp31 + 1.0f;
			float fTemp34 = 1.0f - 1.0f / mydsp_faustpower2_f(fTemp31);
			float fTemp35 = (fTemp32 + -0.26105237f) / fTemp31 + 1.0f;
			float fTemp36 = (fTemp32 + 0.76536685f) / fTemp31 + 1.0f;
			float fTemp37 = (fTemp32 + -0.76536685f) / fTemp31 + 1.0f;
			float fTemp38 = (fTemp32 + 1.2175229f) / fTemp31 + 1.0f;
			float fTemp39 = (fTemp32 + -1.2175229f) / fTemp31 + 1.0f;
			float fTemp40 = (fTemp32 + 1.5867066f) / fTemp31 + 1.0f;
			float fTemp41 = (fTemp32 + -1.5867066f) / fTemp31 + 1.0f;
			float fTemp42 = (fTemp32 + 1.847759f) / fTemp31 + 1.0f;
			float fTemp43 = (fTemp32 + -1.847759f) / fTemp31 + 1.0f;
			float fTemp44 = (fTemp32 + 1.9828898f) / fTemp31 + 1.0f;
			float fTemp45 = (fTemp32 + -1.9828898f) / fTemp31 + 1.0f;
			float fTemp46 = ((iSlow9) ? fTemp24 : fTemp26 + fSlow77 * fTemp25);
			float fTemp47 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow80 * ((iSlow8) ? 0.0f : fTemp46)));
			float fTemp48 = ((iSlow8) ? fTemp46 : fTemp47 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp47)));
			float fTemp49 = ((iSlow7) ? 0.0f : fTemp48);
			fRec32[0] = fTemp49 - (fRec32[2] * fTemp45 + 2.0f * fRec32[1] * fTemp34) / fTemp44;
			fRec31[0] = (fRec32[2] + fRec32[0] + 2.0f * fRec32[1]) / fTemp44 - (fRec31[2] * fTemp43 + 2.0f * fTemp34 * fRec31[1]) / fTemp42;
			fRec30[0] = (fRec31[2] + fRec31[0] + 2.0f * fRec31[1]) / fTemp42 - (fRec30[2] * fTemp41 + 2.0f * fTemp34 * fRec30[1]) / fTemp40;
			fRec29[0] = (fRec30[2] + fRec30[0] + 2.0f * fRec30[1]) / fTemp40 - (fRec29[2] * fTemp39 + 2.0f * fTemp34 * fRec29[1]) / fTemp38;
			fRec28[0] = (fRec29[2] + fRec29[0] + 2.0f * fRec29[1]) / fTemp38 - (fRec28[2] * fTemp37 + 2.0f * fTemp34 * fRec28[1]) / fTemp36;
			fRec27[0] = (fRec28[2] + fRec28[0] + 2.0f * fRec28[1]) / fTemp36 - (fRec27[2] * fTemp35 + 2.0f * fTemp34 * fRec27[1]) / fTemp33;
			float fTemp50 = fSlow81 * ((fRec27[2] + fRec27[0] + 2.0f * fRec27[1]) / fTemp33);
			float fTemp51 = ((iSlow7) ? fTemp29 : fTemp50 + fSlow82 * ((iSlow7) ? 0.0f : fTemp29));
			float fTemp52 = ((iSlow3) ? fTemp51 : ((iSlow3) ? 0.0f : fTemp51) * fTemp3);
			float fTemp53 = ((iSlow2) ? 0.0f : fTemp52);
			float fTemp54 = fSlow88 + fSlow6 * fTemp2;
			float fTemp55 = ((iSlow7) ? fTemp48 : fTemp50 + fSlow82 * fTemp49);
			float fTemp56 = ((iSlow3) ? fTemp55 : ((iSlow3) ? 0.0f : fTemp55) * fTemp54);
			float fTemp57 = ((iSlow2) ? 0.0f : fTemp56);
			fVec4[IOTA0 & 63] = fTemp57;
			float fTemp58 = ((iTemp0 | iSlow2) ? 0.0f : fSlow89 + fRec34[1]);
			fRec34[0] = fTemp58 - std::floor(fTemp58);
			float fTemp59 = 3e+01f * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec34[0]), 65535))] + 1.0f);
			float fTemp60 = fTemp59 + 1.0f;
			int iTemp61 = int(fTemp59);
			int iTemp62 = int(std::min<float>(fTemp60, float(std::max<int>(0, iTemp61 + 1))));
			float fTemp63 = std::floor(fTemp59);
			float fTemp64 = fTemp59 - fTemp63;
			float fTemp65 = fTemp63 + (1.0f - fTemp59);
			int iTemp66 = int(std::min<float>(fTemp60, float(std::max<int>(0, iTemp61))));
			float fRec33 = -(fVec4[(IOTA0 - iTemp66) & 63] * fTemp65 + fTemp64 * fVec4[(IOTA0 - iTemp62) & 63]);
			float fTemp67 = fSlow90 * fRec33;
			float fTemp68 = ((iSlow2) ? fTemp52 : fSlow91 * (fTemp67 + fTemp53) + fSlow87 * fTemp53);
			float fTemp69 = ((iSlow1) ? 0.0f : fTemp68);
			fRec44[0] = fSlow96 * fRec45[1] + fSlow95 * fRec44[1];
			fRec45[0] = float(iTemp0) + fSlow95 * fRec45[1] - fSlow96 * fRec44[1];
			float fTemp70 = std::cos(fConst1 * (14137.167f * (1.0f - fRec44[0]) + 3141.5928f));
			fRec43[0] = ((iSlow1) ? 0.0f : ((iSlow2) ? fTemp56 : fSlow91 * (fTemp57 + fTemp67) + fSlow87 * fTemp57)) + fSlow97 * fRec35[1] + fConst28 * fRec43[1] * fTemp70 - fConst26 * fRec43[2];
			fRec42[0] = fRec43[2] + fConst26 * (fRec43[0] - fRec42[2]) - fConst28 * fTemp70 * (fRec43[1] - fRec42[1]);
			fRec41[0] = fRec42[2] + fConst26 * (fRec42[0] - fRec41[2]) - fConst28 * fTemp70 * (fRec42[1] - fRec41[1]);
			fRec40[0] = fRec41[2] + fConst26 * (fRec41[0] - fRec40[2]) - fConst28 * fTemp70 * (fRec41[1] - fRec40[1]);
			fRec39[0] = fRec40[2] + fConst26 * (fRec40[0] - fRec39[2]) - fConst28 * fTemp70 * (fRec40[1] - fRec39[1]);
			fRec38[0] = fRec39[2] + fConst26 * (fRec39[0] - fRec38[2]) - fConst28 * fTemp70 * (fRec39[1] - fRec38[1]);
			fRec37[0] = fRec38[2] + fConst26 * (fRec38[0] - fRec37[2]) - fConst28 * fTemp70 * (fRec38[1] - fRec37[1]);
			fRec36[0] = fRec37[2] + fConst26 * (fRec37[0] - fRec36[2]) - fConst28 * fTemp70 * (fRec37[1] - fRec36[1]);
			fRec35[0] = fRec36[2] + fConst26 * fRec36[0] - fConst28 * fTemp70 * fRec36[1];
			float fTemp71 = ((iSlow1) ? fTemp68 : fSlow98 * (fRec35[0] + fTemp69) + fSlow93 * fTemp69);
			float fTemp72 = ((iSlow0) ? 0.0f : fTemp71);
			float fTemp73 = ((iTemp0 | iSlow0) ? 0.0f : fSlow101 + fRec46[1]);
			fRec46[0] = fTemp73 - std::floor(fTemp73);
			float fTemp74 = fSlow102 * ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec46[0]), 65535))];
			int iTemp75 = int(std::ceil(-fTemp74));
			output0[i0] = FAUSTFLOAT(((iSlow0) ? fTemp71 : fSlow99 * ((iTemp75) ? fTemp72 : (1.0f - fTemp74) * ((iTemp75) ? 0.0f : fTemp72)) + fSlow100 * fTemp72));
			float fTemp76 = fConst12 * fRec61[1];
			float fTemp77 = float(input1[i0]);
			float fTemp78 = ((iSlow60) ? 0.0f : fTemp77);
			ftbl2[iTemp7] = fTemp78;
			float fTemp79 = fSlow66 * ((iSlow60) ? fTemp77 : fSlow61 * ftbl2[iTemp8] + fSlow62 * fTemp78);
			float fTemp80 = ((iSlow59) ? 0.0f : fTemp79);
			fVec5[0] = fTemp80;
			fRec63[0] = -(fConst21 * (fConst20 * fRec63[1] - (fTemp80 + fVec5[1])));
			fRec62[0] = fRec63[0] - fConst18 * (fConst17 * fRec62[2] + fConst15 * fRec62[1]);
			float fTemp81 = fSlow67 * (fRec62[2] + fRec62[0] + 2.0f * fRec62[1]);
			fRec65[0] = -(fConst21 * (fConst20 * fRec65[1] - fConst16 * (fTemp80 - fVec5[1])));
			fRec64[0] = fRec65[0] - fConst18 * (fConst17 * fRec64[2] + fConst15 * fRec64[1]);
			float fTemp82 = 2.0f * fRec64[1];
			float fTemp83 = ((iSlow59) ? fTemp79 : fConst18 * (fConst14 * (fRec64[2] + (fRec64[0] - fTemp82)) + fTemp81));
			fRec61[0] = ((iSlow50) ? 0.0f : fTemp83) - (fSlow58 * fRec61[2] + fTemp76) / fSlow55;
			float fTemp84 = (fTemp76 + fRec61[0] * fSlow68 + fSlow57 * fRec61[2]) / fSlow55;
			float fTemp85 = ((iSlow50) ? fTemp83 : fTemp84);
			float fTemp86 = ((iSlow49) ? 0.0f : fTemp85);
			fVec6[0] = fTemp86;
			fRec60[0] = -(fConst23 * (fConst22 * fRec60[1] - fConst6 * (fTemp86 - fVec6[1])));
			fRec59[0] = fRec60[0] - fConst8 * (fConst7 * fRec59[2] + fConst5 * fRec59[1]);
			float fTemp87 = 2.0f * fRec59[1];
			fRec67[0] = -(fConst23 * (fConst22 * fRec67[1] - (fTemp86 + fVec6[1])));
			fRec66[0] = fRec67[0] - fConst8 * (fConst7 * fRec66[2] + fConst5 * fRec66[1]);
			float fTemp88 = fRec66[2] + fRec66[0] + 2.0f * fRec66[1];
			float fTemp89 = ((iSlow49) ? fTemp85 : fConst8 * (fTemp88 + fSlow69 * (fRec59[2] + (fRec59[0] - fTemp87))));
			fRec58[0] = ((iSlow26) ? 0.0f : fTemp89) - fSlow48 * (fSlow46 * fRec58[2] + fSlow29 * fRec58[1]);
			fRec57[0] = fSlow70 * (fRec58[2] + (fRec58[0] - 2.0f * fRec58[1])) - fSlow45 * (fSlow43 * fRec57[2] + fSlow29 * fRec57[1]);
			fRec56[0] = fSlow71 * (fRec57[2] + (fRec57[0] - 2.0f * fRec57[1])) - fSlow42 * (fSlow40 * fRec56[2] + fSlow29 * fRec56[1]);
			fRec55[0] = fSlow72 * (fRec56[2] + (fRec56[0] - 2.0f * fRec56[1])) - fSlow39 * (fSlow37 * fRec55[2] + fSlow29 * fRec55[1]);
			fRec54[0] = fSlow73 * (fRec55[2] + (fRec55[0] - 2.0f * fRec55[1])) - fSlow36 * (fSlow34 * fRec54[2] + fSlow29 * fRec54[1]);
			fRec53[0] = fSlow74 * (fRec54[2] + (fRec54[0] - 2.0f * fRec54[1])) - fSlow33 * (fSlow31 * fRec53[2] + fSlow29 * fRec53[1]);
			float fTemp90 = 2.0f * fRec53[1];
			float fTemp91 = ((iSlow26) ? fTemp89 : fSlow75 * (fRec53[2] + (fRec53[0] - fTemp90)));
			fRec52[0] = ((iSlow10) ? 0.0f : fTemp91) - fSlow25 * (fSlow24 * fRec52[2] + fSlow12 * fRec52[1]);
			fRec51[0] = fSlow25 * (fRec52[2] + fRec52[0] + 2.0f * fRec52[1]) - fSlow23 * (fSlow22 * fRec51[2] + fSlow12 * fRec51[1]);
			fRec50[0] = fSlow23 * (fRec51[2] + fRec51[0] + 2.0f * fRec51[1]) - fSlow21 * (fSlow20 * fRec50[2] + fSlow12 * fRec50[1]);
			fRec49[0] = fSlow21 * (fRec50[2] + fRec50[0] + 2.0f * fRec50[1]) - fSlow19 * (fSlow18 * fRec49[2] + fSlow12 * fRec49[1]);
			fRec48[0] = fSlow19 * (fRec49[2] + fRec49[0] + 2.0f * fRec49[1]) - fSlow17 * (fSlow16 * fRec48[2] + fSlow12 * fRec48[1]);
			fRec47[0] = fSlow17 * (fRec48[2] + fRec48[0] + 2.0f * fRec48[1]) - fSlow15 * (fSlow14 * fRec47[2] + fSlow12 * fRec47[1]);
			float fTemp92 = fSlow15 * (fRec47[2] + fRec47[0] + 2.0f * fRec47[1]);
			float fTemp93 = ((iSlow10) ? ((iSlow26) ? ((iSlow49) ? ((iSlow50) ? ((iSlow59) ? fTemp79 : fConst18 * (fTemp81 + fConst14 * (fRec64[0] + fRec64[2] - fTemp82))) : fTemp84) : fConst8 * (fTemp88 + fSlow69 * (fRec59[0] + fRec59[2] - fTemp87))) : fSlow75 * (fRec53[0] + fRec53[2] - fTemp90)) : fTemp92);
			float fTemp94 = ((iSlow10) ? fTemp91 : fTemp92);
			float fTemp95 = ((iSlow9) ? 0.0f : fTemp94);
			fRec68[IOTA0 & 2097151] = fSlow79 * fRec68[(IOTA0 - iSlow78) & 2097151] + fTemp95;
			float fTemp96 = fSlow76 * fRec68[IOTA0 & 2097151];
			float fTemp97 = ((iSlow9) ? fTemp93 : fTemp96 + fSlow77 * ((iSlow9) ? 0.0f : fTemp93));
			float fTemp98 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow80 * ((iSlow8) ? 0.0f : fTemp97)));
			float fTemp99 = ((iSlow8) ? fTemp97 : fTemp98 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp98)));
			float fTemp100 = ((iSlow9) ? fTemp94 : fTemp96 + fSlow77 * fTemp95);
			float fTemp101 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow80 * ((iSlow8) ? 0.0f : fTemp100)));
			float fTemp102 = ((iSlow8) ? fTemp100 : fTemp101 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp101)));
			float fTemp103 = ((iSlow7) ? 0.0f : fTemp102);
			fRec74[0] = fTemp103 - (fTemp45 * fRec74[2] + 2.0f * fTemp34 * fRec74[1]) / fTemp44;
			fRec73[0] = (fRec74[2] + fRec74[0] + 2.0f * fRec74[1]) / fTemp44 - (fTemp43 * fRec73[2] + 2.0f * fTemp34 * fRec73[1]) / fTemp42;
			fRec72[0] = (fRec73[2] + fRec73[0] + 2.0f * fRec73[1]) / fTemp42 - (fTemp41 * fRec72[2] + 2.0f * fTemp34 * fRec72[1]) / fTemp40;
			fRec71[0] = (fRec72[2] + fRec72[0] + 2.0f * fRec72[1]) / fTemp40 - (fTemp39 * fRec71[2] + 2.0f * fTemp34 * fRec71[1]) / fTemp38;
			fRec70[0] = (fRec71[2] + fRec71[0] + 2.0f * fRec71[1]) / fTemp38 - (fTemp37 * fRec70[2] + 2.0f * fTemp34 * fRec70[1]) / fTemp36;
			fRec69[0] = (fRec70[2] + fRec70[0] + 2.0f * fRec70[1]) / fTemp36 - (fTemp35 * fRec69[2] + 2.0f * fTemp34 * fRec69[1]) / fTemp33;
			float fTemp104 = fSlow81 * ((fRec69[2] + fRec69[0] + 2.0f * fRec69[1]) / fTemp33);
			float fTemp105 = ((iSlow7) ? fTemp99 : fTemp104 + fSlow82 * ((iSlow7) ? 0.0f : fTemp99));
			float fTemp106 = ((iSlow3) ? fTemp105 : fTemp3 * ((iSlow3) ? 0.0f : fTemp105));
			float fTemp107 = ((iSlow2) ? 0.0f : fTemp106);
			float fTemp108 = ((iSlow7) ? fTemp102 : fTemp104 + fSlow82 * fTemp103);
			float fTemp109 = ((iSlow3) ? fTemp108 : fTemp54 * ((iSlow3) ? 0.0f : fTemp108));
			float fTemp110 = ((iSlow2) ? 0.0f : fTemp109);
			fVec7[IOTA0 & 63] = fTemp110;
			float fRec75 = -(fTemp65 * fVec7[(IOTA0 - iTemp66) & 63] + fTemp64 * fVec7[(IOTA0 - iTemp62) & 63]);
			float fTemp111 = fSlow90 * fRec75;
			float fTemp112 = ((iSlow2) ? fTemp106 : fSlow91 * (fTemp111 + fTemp107) + fSlow87 * fTemp107);
			float fTemp113 = ((iSlow1) ? 0.0f : fTemp112);
			float fTemp114 = std::cos(fConst1 * (14137.167f * (1.0f - fRec45[0]) + 3141.5928f));
			fRec84[0] = ((iSlow1) ? 0.0f : ((iSlow2) ? fTemp109 : fSlow91 * (fTemp110 + fTemp111) + fSlow87 * fTemp110)) + fSlow97 * fRec76[1] + fConst28 * fRec84[1] * fTemp114 - fConst26 * fRec84[2];
			fRec83[0] = fRec84[2] + fConst26 * (fRec84[0] - fRec83[2]) - fConst28 * fTemp114 * (fRec84[1] - fRec83[1]);
			fRec82[0] = fRec83[2] + fConst26 * (fRec83[0] - fRec82[2]) - fConst28 * fTemp114 * (fRec83[1] - fRec82[1]);
			fRec81[0] = fRec82[2] + fConst26 * (fRec82[0] - fRec81[2]) - fConst28 * fTemp114 * (fRec82[1] - fRec81[1]);
			fRec80[0] = fRec81[2] + fConst26 * (fRec81[0] - fRec80[2]) - fConst28 * fTemp114 * (fRec81[1] - fRec80[1]);
			fRec79[0] = fRec80[2] + fConst26 * (fRec80[0] - fRec79[2]) - fConst28 * fTemp114 * (fRec80[1] - fRec79[1]);
			fRec78[0] = fRec79[2] + fConst26 * (fRec79[0] - fRec78[2]) - fConst28 * fTemp114 * (fRec79[1] - fRec78[1]);
			fRec77[0] = fRec78[2] + fConst26 * (fRec78[0] - fRec77[2]) - fConst28 * fTemp114 * (fRec78[1] - fRec77[1]);
			fRec76[0] = fRec77[2] + fConst26 * fRec77[0] - fConst28 * fTemp114 * fRec77[1];
			float fTemp115 = ((iSlow1) ? fTemp112 : fSlow98 * (fRec76[0] + fTemp113) + fSlow93 * fTemp113);
			float fTemp116 = ((iSlow0) ? 0.0f : fTemp115);
			int iTemp117 = int(std::ceil(fTemp74));
			output1[i0] = FAUSTFLOAT(((iSlow0) ? fTemp115 : fSlow99 * ((iTemp117) ? fTemp116 : (fTemp74 + 1.0f) * ((iTemp117) ? 0.0f : fTemp116)) + fSlow100 * fTemp116));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fVec1[1] = fVec1[0];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec2[1] = fVec2[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			IOTA0 = IOTA0 + 1;
			fRec26[1] = fRec26[0];
			fRec32[2] = fRec32[1];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec34[1] = fRec34[0];
			fRec44[1] = fRec44[0];
			fRec45[1] = fRec45[0];
			fRec43[2] = fRec43[1];
			fRec43[1] = fRec43[0];
			fRec42[2] = fRec42[1];
			fRec42[1] = fRec42[0];
			fRec41[2] = fRec41[1];
			fRec41[1] = fRec41[0];
			fRec40[2] = fRec40[1];
			fRec40[1] = fRec40[0];
			fRec39[2] = fRec39[1];
			fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1];
			fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1];
			fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1];
			fRec36[1] = fRec36[0];
			fRec35[1] = fRec35[0];
			fRec46[1] = fRec46[0];
			fVec5[1] = fVec5[0];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec65[1] = fRec65[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fVec6[1] = fVec6[0];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
			fRec67[1] = fRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fRec58[2] = fRec58[1];
			fRec58[1] = fRec58[0];
			fRec57[2] = fRec57[1];
			fRec57[1] = fRec57[0];
			fRec56[2] = fRec56[1];
			fRec56[1] = fRec56[0];
			fRec55[2] = fRec55[1];
			fRec55[1] = fRec55[0];
			fRec54[2] = fRec54[1];
			fRec54[1] = fRec54[0];
			fRec53[2] = fRec53[1];
			fRec53[1] = fRec53[0];
			fRec52[2] = fRec52[1];
			fRec52[1] = fRec52[0];
			fRec51[2] = fRec51[1];
			fRec51[1] = fRec51[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec74[2] = fRec74[1];
			fRec74[1] = fRec74[0];
			fRec73[2] = fRec73[1];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec71[2] = fRec71[1];
			fRec71[1] = fRec71[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec84[2] = fRec84[1];
			fRec84[1] = fRec84[0];
			fRec83[2] = fRec83[1];
			fRec83[1] = fRec83[0];
			fRec82[2] = fRec82[1];
			fRec82[1] = fRec82[0];
			fRec81[2] = fRec81[1];
			fRec81[1] = fRec81[0];
			fRec80[2] = fRec80[1];
			fRec80[1] = fRec80[0];
			fRec79[2] = fRec79[1];
			fRec79[1] = fRec79[0];
			fRec78[2] = fRec78[1];
			fRec78[1] = fRec78[0];
			fRec77[2] = fRec77[1];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
		}
	}

};

#endif
