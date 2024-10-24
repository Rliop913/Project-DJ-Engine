/* ------------------------------------------------------------
name: "faust_v42bug_fix", "faust_v32"
Code generated with Faust 2.60.3 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -single -ftz 0 -vec -lv 0 -vs 48
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
	
	int iVec1[2];
	int iRec43[2];
	
  public:
	
	int getNumInputsmydspSIG1() {
		return 0;
	}
	int getNumOutputsmydspSIG1() {
		return 1;
	}
	
	void instanceInitmydspSIG1(int sample_rate) {
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			iVec1[l31] = 0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			iRec43[l32] = 0;
		}
	}
	
	void fillmydspSIG1(int count, float* table) {
		for (int i2 = 0; i2 < count; i2 = i2 + 1) {
			iVec1[0] = 1;
			iRec43[0] = (iVec1[1] + iRec43[1]) % 65536;
			table[i2] = std::sin(9.58738e-05f * float(iRec43[0]));
			iVec1[1] = iVec1[0];
			iRec43[1] = iRec43[0];
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
	int roll_sw = 0; void _roll_sw(const bool& turn_on) { turn_on ? roll_sw = 1 : roll_sw = 0; }

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
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0_perm[4];
	float fConst2;
	float fRec9_perm[4];
	float fRec10_perm[4];
	int iRec36_perm[4];
	float fConst3;
	float fRec37_perm[4];
	float fConst5;
	float fConst6;
	float ftbl0[48000];
	float fYec0_perm[4];
	float fConst8;
	float fConst9;
	float fRec35_perm[4];
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fRec34_perm[4];
	float fRec39_perm[4];
	float fRec38_perm[4];
	float fConst15;
	float fConst17;
	float fConst18;
	float fConst19;
	float fRec33_perm[4];
	float fYec1_perm[4];
	float fConst21;
	float fConst22;
	float fConst24;
	float fRec32_perm[4];
	float fConst26;
	float fConst27;
	float fConst28;
	float fConst29;
	float fRec31_perm[4];
	float fConst30;
	float fRec41_perm[4];
	float fRec40_perm[4];
	float fConst31;
	float fConst32;
	float fRec30_perm[4];
	float fRec29_perm[4];
	float fRec28_perm[4];
	float fRec27_perm[4];
	float fRec26_perm[4];
	float fRec25_perm[4];
	float fRec24_perm[4];
	float fRec23_perm[4];
	float fRec22_perm[4];
	float fRec21_perm[4];
	float fRec20_perm[4];
	float fRec19_perm[4];
	float fConst33;
	float fRec18[2097152];
	int fRec18_idx;
	int fRec18_idx_save;
	float fRec42_perm[4];
	float fRec17_perm[4];
	float fRec16_perm[4];
	float fRec15_perm[4];
	float fRec14_perm[4];
	float fRec13_perm[4];
	float fRec12_perm[4];
	float fRec44_perm[4];
	float fRec46_perm[4];
	float fYec2[128];
	int fYec2_idx;
	int fYec2_idx_save;
	float fConst35;
	float fConst36;
	float fRec11_perm[4];
	float fRec8_perm[4];
	float fRec7_perm[4];
	float fRec6_perm[4];
	float fRec5_perm[4];
	float fRec4_perm[4];
	float fRec3_perm[4];
	float fRec2_perm[4];
	float fRec1_perm[4];
	float ftbl2[48000];
	float fYec3_perm[4];
	float fRec79_perm[4];
	float fRec78_perm[4];
	float fRec81_perm[4];
	float fRec80_perm[4];
	float fRec77_perm[4];
	float fYec4_perm[4];
	float fRec76_perm[4];
	float fRec75_perm[4];
	float fRec83_perm[4];
	float fRec82_perm[4];
	float fRec74_perm[4];
	float fRec73_perm[4];
	float fRec72_perm[4];
	float fRec71_perm[4];
	float fRec70_perm[4];
	float fRec69_perm[4];
	float fRec68_perm[4];
	float fRec67_perm[4];
	float fRec66_perm[4];
	float fRec65_perm[4];
	float fRec64_perm[4];
	float fRec63_perm[4];
	float fRec62[2097152];
	int fRec62_idx;
	int fRec62_idx_save;
	float fRec61_perm[4];
	float fRec60_perm[4];
	float fRec59_perm[4];
	float fRec58_perm[4];
	float fRec57_perm[4];
	float fRec56_perm[4];
	float fYec5[128];
	int fYec5_idx;
	int fYec5_idx_save;
	float fRec55_perm[4];
	float fRec54_perm[4];
	float fRec53_perm[4];
	float fRec52_perm[4];
	float fRec51_perm[4];
	float fRec50_perm[4];
	float fRec49_perm[4];
	float fRec48_perm[4];
	float fRec47_perm[4];
	
 public:
	 Faust_engine() {}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG1* sig1 = newmydspSIG1();
		sig1->instanceInitmydspSIG1(sample_rate);
		sig1->fillmydspSIG1(65536, ftbl1mydspSIG1);
		deletemydspSIG1(sig1);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 6.2831855f / fConst0;
		fConst3 = 6e+01f * fConst0;
		float fConst4 = std::tan(1570.7964f / fConst0);
		fConst5 = 1.0f / fConst4;
		fConst6 = 1.0f - fConst5;
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(48000, ftbl0);
		float fConst7 = fConst5 + 1.0f;
		fConst8 = 1.0f / fConst7;
		fConst9 = 0.0f - 1.0f / (fConst4 * fConst7);
		float fConst10 = mydsp_faustpower2_f(fConst4);
		fConst11 = 1.0f / fConst10;
		fConst12 = 2.0f * (1.0f - fConst11);
		fConst13 = (fConst5 + -1.0f) / fConst4 + 1.0f;
		fConst14 = 1.0f / ((fConst5 + 1.0f) / fConst4 + 1.0f);
		fConst15 = 7068.5835f / (fConst0 * std::sin(17278.76f / fConst0));
		float fConst16 = std::tan(8639.38f / fConst0);
		fConst17 = 1.0f / fConst16;
		fConst18 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fConst16));
		fConst19 = 0.0f - 2.0f / fConst10;
		float fConst20 = std::tan(15707.963f / fConst0);
		fConst21 = 1.0f / fConst20;
		fConst22 = 1.0f - fConst21;
		float fConst23 = fConst21 + 1.0f;
		fConst24 = 1.0f / fConst23;
		float fConst25 = mydsp_faustpower2_f(fConst20);
		fConst26 = 1.0f / fConst25;
		fConst27 = 2.0f * (1.0f - fConst26);
		fConst28 = (fConst21 + -1.0f) / fConst20 + 1.0f;
		fConst29 = 1.0f / ((fConst21 + 1.0f) / fConst20 + 1.0f);
		fConst30 = 0.0f - 1.0f / (fConst20 * fConst23);
		fConst31 = 3.1415927f / fConst0;
		fConst32 = 0.0f - 2.0f / fConst25;
		fConst33 = 1e+01f * fConst0;
		float fConst34 = std::exp(0.0f - 14137.167f / fConst0);
		fConst35 = mydsp_faustpower2_f(fConst34);
		fConst36 = 0.0f - 2.0f * fConst34;
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(48000, ftbl2);
		deletemydspSIG0(sig0);
	}
	
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iVec0_perm[l0] = 0;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec0_perm[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec9_perm[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec10_perm[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			iRec36_perm[l4] = 0;
		}
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec37_perm[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fYec0_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec35_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec34_perm[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec39_perm[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec38_perm[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec33_perm[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fYec1_perm[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec32_perm[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec31_perm[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec41_perm[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec40_perm[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec30_perm[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec29_perm[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec28_perm[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec27_perm[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 4; l21 = l21 + 1) {
			fRec26_perm[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 4; l22 = l22 + 1) {
			fRec25_perm[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 4; l23 = l23 + 1) {
			fRec24_perm[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
			fRec23_perm[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 4; l25 = l25 + 1) {
			fRec22_perm[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 4; l26 = l26 + 1) {
			fRec21_perm[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 4; l27 = l27 + 1) {
			fRec20_perm[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 4; l28 = l28 + 1) {
			fRec19_perm[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2097152; l29 = l29 + 1) {
			fRec18[l29] = 0.0f;
		}
		fRec18_idx = 0;
		fRec18_idx_save = 0;
		for (int l30 = 0; l30 < 4; l30 = l30 + 1) {
			fRec42_perm[l30] = 0.0f;
		}
		for (int l33 = 0; l33 < 4; l33 = l33 + 1) {
			fRec17_perm[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 4; l34 = l34 + 1) {
			fRec16_perm[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 4; l35 = l35 + 1) {
			fRec15_perm[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 4; l36 = l36 + 1) {
			fRec14_perm[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 4; l37 = l37 + 1) {
			fRec13_perm[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 4; l38 = l38 + 1) {
			fRec12_perm[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 4; l39 = l39 + 1) {
			fRec44_perm[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 4; l40 = l40 + 1) {
			fRec46_perm[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 128; l41 = l41 + 1) {
			fYec2[l41] = 0.0f;
		}
		fYec2_idx = 0;
		fYec2_idx_save = 0;
		for (int l42 = 0; l42 < 4; l42 = l42 + 1) {
			fRec11_perm[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 4; l43 = l43 + 1) {
			fRec8_perm[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 4; l44 = l44 + 1) {
			fRec7_perm[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 4; l45 = l45 + 1) {
			fRec6_perm[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 4; l46 = l46 + 1) {
			fRec5_perm[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 4; l47 = l47 + 1) {
			fRec4_perm[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 4; l48 = l48 + 1) {
			fRec3_perm[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 4; l49 = l49 + 1) {
			fRec2_perm[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 4; l50 = l50 + 1) {
			fRec1_perm[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 4; l51 = l51 + 1) {
			fYec3_perm[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 4; l52 = l52 + 1) {
			fRec79_perm[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 4; l53 = l53 + 1) {
			fRec78_perm[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 4; l54 = l54 + 1) {
			fRec81_perm[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 4; l55 = l55 + 1) {
			fRec80_perm[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 4; l56 = l56 + 1) {
			fRec77_perm[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 4; l57 = l57 + 1) {
			fYec4_perm[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 4; l58 = l58 + 1) {
			fRec76_perm[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 4; l59 = l59 + 1) {
			fRec75_perm[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 4; l60 = l60 + 1) {
			fRec83_perm[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 4; l61 = l61 + 1) {
			fRec82_perm[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 4; l62 = l62 + 1) {
			fRec74_perm[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 4; l63 = l63 + 1) {
			fRec73_perm[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 4; l64 = l64 + 1) {
			fRec72_perm[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 4; l65 = l65 + 1) {
			fRec71_perm[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 4; l66 = l66 + 1) {
			fRec70_perm[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 4; l67 = l67 + 1) {
			fRec69_perm[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 4; l68 = l68 + 1) {
			fRec68_perm[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 4; l69 = l69 + 1) {
			fRec67_perm[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 4; l70 = l70 + 1) {
			fRec66_perm[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 4; l71 = l71 + 1) {
			fRec65_perm[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 4; l72 = l72 + 1) {
			fRec64_perm[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 4; l73 = l73 + 1) {
			fRec63_perm[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2097152; l74 = l74 + 1) {
			fRec62[l74] = 0.0f;
		}
		fRec62_idx = 0;
		fRec62_idx_save = 0;
		for (int l75 = 0; l75 < 4; l75 = l75 + 1) {
			fRec61_perm[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 4; l76 = l76 + 1) {
			fRec60_perm[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 4; l77 = l77 + 1) {
			fRec59_perm[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 4; l78 = l78 + 1) {
			fRec58_perm[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 4; l79 = l79 + 1) {
			fRec57_perm[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 4; l80 = l80 + 1) {
			fRec56_perm[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 128; l81 = l81 + 1) {
			fYec5[l81] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l82 = 0; l82 < 4; l82 = l82 + 1) {
			fRec55_perm[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 4; l83 = l83 + 1) {
			fRec54_perm[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 4; l84 = l84 + 1) {
			fRec53_perm[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 4; l85 = l85 + 1) {
			fRec52_perm[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 4; l86 = l86 + 1) {
			fRec51_perm[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 4; l87 = l87 + 1) {
			fRec50_perm[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 4; l88 = l88 + 1) {
			fRec49_perm[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 4; l89 = l89 + 1) {
			fRec48_perm[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 4; l90 = l90 + 1) {
			fRec47_perm[l90] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceClear();
	}
	
	Faust_engine* clone() {
		return new Faust_engine();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	
	void compute(int count, FAUSTFLOAT* RESTRICT inputfirst, FAUSTFLOAT* RESTRICT inputsecond, FAUSTFLOAT* RESTRICT outputfirst, FAUSTFLOAT* RESTRICT outputsecond) {
		FAUSTFLOAT* input0_ptr = inputfirst;
		FAUSTFLOAT* input1_ptr = inputsecond;
		FAUSTFLOAT* output0_ptr = outputfirst;
		FAUSTFLOAT* output1_ptr = outputsecond;
		int iSlow0 = panner_sw;
		int iVec0_tmp[52];
		int* iVec0 = &iVec0_tmp[4];
		int iZec0[48];
		float fSlow1 = fConst1 * panner_bps;
		float fZec1[48];
		float fRec0_tmp[52];
		float* fRec0 = &fRec0_tmp[4];
		float fSlow2 = fConst2 * phaser_bps;
		float fSlow3 = std::cos(fSlow2);
		float fSlow4 = std::sin(fSlow2);
		float fRec9_tmp[52];
		float* fRec9 = &fRec9_tmp[4];
		float fRec10_tmp[52];
		float* fRec10 = &fRec10_tmp[4];
		int iSlow5 = roll_sw;
		int iRec36_tmp[52];
		int* iRec36 = &iRec36_tmp[4];
		float fSlow6 = fConst3 / roll_I_BPM;
		float fSlow7 = float(iSlow5);
		float fRec37_tmp[52];
		float* fRec37 = &fRec37_tmp[4];
		int iSlow8 = eq_low_sw;
		float fSlow9 = roller_p;
		float fSlow10 = 1.0f - fSlow9;
		int iZec2[48];
		int iZec3[48];
		float fSlow11 = trim * fader * volume_limit;
		float fZec4[48];
		float fYec0_tmp[52];
		float* fYec0 = &fYec0_tmp[4];
		float fRec35_tmp[52];
		float* fRec35 = &fRec35_tmp[4];
		float fRec34_tmp[52];
		float* fRec34 = &fRec34_tmp[4];
		float fRec39_tmp[52];
		float* fRec39 = &fRec39_tmp[4];
		float fRec38_tmp[52];
		float* fRec38 = &fRec38_tmp[4];
		float fSlow12 = eq_mid_v;
		int iSlow13 = fSlow12 > 0.0f;
		float fSlow14 = fConst15 * std::pow(1e+01f, 0.05f * std::fabs(fSlow12));
		float fSlow15 = ((iSlow13) ? fConst15 : fSlow14);
		float fSlow16 = fConst17 * (fConst17 + fSlow15) + 1.0f;
		float fZec5[48];
		float fSlow17 = fConst17 * (fConst17 - fSlow15) + 1.0f;
		int iSlow18 = eq_mid_sw;
		float fSlow19 = std::pow(1e+01f, 0.05f * eq_low_v);
		float fZec6[48];
		float fRec33_tmp[52];
		float* fRec33 = &fRec33_tmp[4];
		int iSlow20 = eq_high_sw;
		float fSlow21 = ((iSlow13) ? fSlow14 : fConst15);
		float fSlow22 = fConst17 * (fConst17 - fSlow21) + 1.0f;
		float fSlow23 = fConst17 * (fConst17 + fSlow21) + 1.0f;
		float fZec7[48];
		float fYec1_tmp[52];
		float* fYec1 = &fYec1_tmp[4];
		float fRec32_tmp[52];
		float* fRec32 = &fRec32_tmp[4];
		float fRec31_tmp[52];
		float* fRec31 = &fRec31_tmp[4];
		float fRec41_tmp[52];
		float* fRec41 = &fRec41_tmp[4];
		float fRec40_tmp[52];
		float* fRec40 = &fRec40_tmp[4];
		float fSlow24 = std::tan(fConst31 * float(high_freq_value));
		float fSlow25 = mydsp_faustpower2_f(fSlow24);
		float fSlow26 = 1.0f / fSlow25;
		float fSlow27 = 2.0f * (1.0f - fSlow26);
		float fSlow28 = 1.0f / fSlow24;
		float fSlow29 = (fSlow28 + -1.9828898f) / fSlow24 + 1.0f;
		float fSlow30 = 1.0f / ((fSlow28 + 1.9828898f) / fSlow24 + 1.0f);
		int iSlow31 = hi_fi_sw;
		float fSlow32 = std::pow(1e+01f, 0.05f * eq_hi_v);
		float fZec8[48];
		float fRec30_tmp[52];
		float* fRec30 = &fRec30_tmp[4];
		float fSlow33 = (fSlow28 + -1.847759f) / fSlow24 + 1.0f;
		float fSlow34 = 1.0f / ((fSlow28 + 1.847759f) / fSlow24 + 1.0f);
		float fSlow35 = 0.0f - 2.0f / fSlow25;
		float fRec29_tmp[52];
		float* fRec29 = &fRec29_tmp[4];
		float fSlow36 = (fSlow28 + -1.5867066f) / fSlow24 + 1.0f;
		float fSlow37 = 1.0f / ((fSlow28 + 1.5867066f) / fSlow24 + 1.0f);
		float fRec28_tmp[52];
		float* fRec28 = &fRec28_tmp[4];
		float fSlow38 = (fSlow28 + -1.2175229f) / fSlow24 + 1.0f;
		float fSlow39 = 1.0f / ((fSlow28 + 1.2175229f) / fSlow24 + 1.0f);
		float fRec27_tmp[52];
		float* fRec27 = &fRec27_tmp[4];
		float fSlow40 = (fSlow28 + -0.76536685f) / fSlow24 + 1.0f;
		float fSlow41 = 1.0f / ((fSlow28 + 0.76536685f) / fSlow24 + 1.0f);
		float fRec26_tmp[52];
		float* fRec26 = &fRec26_tmp[4];
		float fSlow42 = (fSlow28 + -0.26105237f) / fSlow24 + 1.0f;
		float fSlow43 = 1.0f / ((fSlow28 + 0.26105237f) / fSlow24 + 1.0f);
		float fRec25_tmp[52];
		float* fRec25 = &fRec25_tmp[4];
		float fSlow44 = std::tan(fConst31 * float(low_freq_value));
		float fSlow45 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fSlow44));
		float fSlow46 = 1.0f / fSlow44;
		float fSlow47 = (fSlow46 + -1.9828898f) / fSlow44 + 1.0f;
		float fSlow48 = 1.0f / ((fSlow46 + 1.9828898f) / fSlow44 + 1.0f);
		int iSlow49 = lo_fi_sw;
		float fZec9[48];
		float fRec24_tmp[52];
		float* fRec24 = &fRec24_tmp[4];
		float fSlow50 = (fSlow46 + -1.847759f) / fSlow44 + 1.0f;
		float fSlow51 = 1.0f / ((fSlow46 + 1.847759f) / fSlow44 + 1.0f);
		float fRec23_tmp[52];
		float* fRec23 = &fRec23_tmp[4];
		float fSlow52 = (fSlow46 + -1.5867066f) / fSlow44 + 1.0f;
		float fSlow53 = 1.0f / ((fSlow46 + 1.5867066f) / fSlow44 + 1.0f);
		float fRec22_tmp[52];
		float* fRec22 = &fRec22_tmp[4];
		float fSlow54 = (fSlow46 + -1.2175229f) / fSlow44 + 1.0f;
		float fSlow55 = 1.0f / ((fSlow46 + 1.2175229f) / fSlow44 + 1.0f);
		float fRec21_tmp[52];
		float* fRec21 = &fRec21_tmp[4];
		float fSlow56 = (fSlow46 + -0.76536685f) / fSlow44 + 1.0f;
		float fSlow57 = 1.0f / ((fSlow46 + 0.76536685f) / fSlow44 + 1.0f);
		float fRec20_tmp[52];
		float* fRec20 = &fRec20_tmp[4];
		float fSlow58 = (fSlow46 + -0.26105237f) / fSlow44 + 1.0f;
		float fSlow59 = 1.0f / ((fSlow46 + 0.26105237f) / fSlow44 + 1.0f);
		float fRec19_tmp[52];
		float* fRec19 = &fRec19_tmp[4];
		int iSlow60 = echo_sw;
		float fZec10[48];
		float fZec11[48];
		int iSlow61 = int(std::min<float>(fConst33, std::max<float>(0.0f, fConst0 * echo_dur))) + 1;
		float fSlow62 = echo_feedback_var;
		int iSlow63 = l_f_s_sw;
		float fSlow64 = fConst1 * l_f_s_bps;
		float fZec12[48];
		float fRec42_tmp[52];
		float* fRec42 = &fRec42_tmp[4];
		float fSlow65 = float(l_f_s_gain_min_freq);
		float fSlow66 = 1e+04f - 0.5f * fSlow65;
		float fZec13[48];
		float fZec14[48];
		float fZec15[48];
		float fZec16[48];
		float fZec17[48];
		int iSlow67 = distortion_sw;
		float fSlow68 = echo_p;
		float fSlow69 = 1.0f - fSlow68;
		float fZec18[48];
		float fSlow70 = std::pow(1e+01f, 2.0f * distortion_gain);
		float fZec19[48];
		float fZec20[48];
		float fZec21[48];
		float fRec17_tmp[52];
		float* fRec17 = &fRec17_tmp[4];
		float fZec22[48];
		float fZec23[48];
		float fRec16_tmp[52];
		float* fRec16 = &fRec16_tmp[4];
		float fZec24[48];
		float fZec25[48];
		float fRec15_tmp[52];
		float* fRec15 = &fRec15_tmp[4];
		float fZec26[48];
		float fZec27[48];
		float fRec14_tmp[52];
		float* fRec14 = &fRec14_tmp[4];
		float fZec28[48];
		float fZec29[48];
		float fRec13_tmp[52];
		float* fRec13 = &fRec13_tmp[4];
		float fZec30[48];
		float fZec31[48];
		float fRec12_tmp[52];
		float* fRec12 = &fRec12_tmp[4];
		int iSlow71 = trance_sw;
		float fSlow72 = fConst1 * trance_bps;
		float fZec32[48];
		float fRec44_tmp[52];
		float* fRec44 = &fRec44_tmp[4];
		int iSlow73 = flanger_sw;
		float fSlow74 = fConst1 * flanger_bps;
		float fZec33[48];
		float fRec46_tmp[52];
		float* fRec46 = &fRec46_tmp[4];
		float fSlow75 = trance_gain;
		float fSlow76 = trance_p;
		float fSlow77 = 1.0f - fSlow76;
		float fZec34[48];
		float fSlow78 = l_f_s_p;
		float fSlow79 = 1.0f - fSlow78;
		float fZec35[48];
		float fZec36[48];
		float fZec37[48];
		float fZec38[48];
		int iZec39[48];
		int iZec40[48];
		float fZec41[48];
		float fZec42[48];
		float fZec43[48];
		int iZec44[48];
		float fRec45[48];
		float fZec45[48];
		float fSlow80 = phaser_gain;
		int iSlow81 = phaser_sw;
		float fSlow82 = flanger_p;
		float fSlow83 = 1.0f - fSlow82;
		float fSlow84 = flanger_gain;
		float fSlow85 = 0.5f * fSlow82;
		float fZec46[48];
		float fZec47[48];
		float fRec11_tmp[52];
		float* fRec11 = &fRec11_tmp[4];
		float fRec8_tmp[52];
		float* fRec8 = &fRec8_tmp[4];
		float fRec7_tmp[52];
		float* fRec7 = &fRec7_tmp[4];
		float fRec6_tmp[52];
		float* fRec6 = &fRec6_tmp[4];
		float fRec5_tmp[52];
		float* fRec5 = &fRec5_tmp[4];
		float fRec4_tmp[52];
		float* fRec4 = &fRec4_tmp[4];
		float fRec3_tmp[52];
		float* fRec3 = &fRec3_tmp[4];
		float fRec2_tmp[52];
		float* fRec2 = &fRec2_tmp[4];
		float fRec1_tmp[52];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow86 = phaser_p;
		float fSlow87 = 1.0f - fSlow86;
		float fSlow88 = 0.5f * fSlow86;
		float fZec48[48];
		float fZec49[48];
		float fSlow89 = panner_p;
		float fSlow90 = 1.0f - fSlow89;
		float fSlow91 = panner_gain;
		float fZec50[48];
		int iZec51[48];
		float fZec52[48];
		float fYec3_tmp[52];
		float* fYec3 = &fYec3_tmp[4];
		float fRec79_tmp[52];
		float* fRec79 = &fRec79_tmp[4];
		float fRec78_tmp[52];
		float* fRec78 = &fRec78_tmp[4];
		float fRec81_tmp[52];
		float* fRec81 = &fRec81_tmp[4];
		float fRec80_tmp[52];
		float* fRec80 = &fRec80_tmp[4];
		float fZec53[48];
		float fZec54[48];
		float fRec77_tmp[52];
		float* fRec77 = &fRec77_tmp[4];
		float fZec55[48];
		float fYec4_tmp[52];
		float* fYec4 = &fYec4_tmp[4];
		float fRec76_tmp[52];
		float* fRec76 = &fRec76_tmp[4];
		float fRec75_tmp[52];
		float* fRec75 = &fRec75_tmp[4];
		float fRec83_tmp[52];
		float* fRec83 = &fRec83_tmp[4];
		float fRec82_tmp[52];
		float* fRec82 = &fRec82_tmp[4];
		float fZec56[48];
		float fRec74_tmp[52];
		float* fRec74 = &fRec74_tmp[4];
		float fRec73_tmp[52];
		float* fRec73 = &fRec73_tmp[4];
		float fRec72_tmp[52];
		float* fRec72 = &fRec72_tmp[4];
		float fRec71_tmp[52];
		float* fRec71 = &fRec71_tmp[4];
		float fRec70_tmp[52];
		float* fRec70 = &fRec70_tmp[4];
		float fRec69_tmp[52];
		float* fRec69 = &fRec69_tmp[4];
		float fZec57[48];
		float fRec68_tmp[52];
		float* fRec68 = &fRec68_tmp[4];
		float fRec67_tmp[52];
		float* fRec67 = &fRec67_tmp[4];
		float fRec66_tmp[52];
		float* fRec66 = &fRec66_tmp[4];
		float fRec65_tmp[52];
		float* fRec65 = &fRec65_tmp[4];
		float fRec64_tmp[52];
		float* fRec64 = &fRec64_tmp[4];
		float fRec63_tmp[52];
		float* fRec63 = &fRec63_tmp[4];
		float fZec58[48];
		float fZec59[48];
		float fZec60[48];
		float fZec61[48];
		float fZec62[48];
		float fZec63[48];
		float fRec61_tmp[52];
		float* fRec61 = &fRec61_tmp[4];
		float fRec60_tmp[52];
		float* fRec60 = &fRec60_tmp[4];
		float fRec59_tmp[52];
		float* fRec59 = &fRec59_tmp[4];
		float fRec58_tmp[52];
		float* fRec58 = &fRec58_tmp[4];
		float fRec57_tmp[52];
		float* fRec57 = &fRec57_tmp[4];
		float fRec56_tmp[52];
		float* fRec56 = &fRec56_tmp[4];
		float fZec64[48];
		float fZec65[48];
		float fRec84[48];
		float fZec66[48];
		float fZec67[48];
		float fZec68[48];
		float fRec55_tmp[52];
		float* fRec55 = &fRec55_tmp[4];
		float fRec54_tmp[52];
		float* fRec54 = &fRec54_tmp[4];
		float fRec53_tmp[52];
		float* fRec53 = &fRec53_tmp[4];
		float fRec52_tmp[52];
		float* fRec52 = &fRec52_tmp[4];
		float fRec51_tmp[52];
		float* fRec51 = &fRec51_tmp[4];
		float fRec50_tmp[52];
		float* fRec50 = &fRec50_tmp[4];
		float fRec49_tmp[52];
		float* fRec49 = &fRec49_tmp[4];
		float fRec48_tmp[52];
		float* fRec48 = &fRec48_tmp[4];
		float fRec47_tmp[52];
		float* fRec47 = &fRec47_tmp[4];
		float fZec69[48];
		float fZec70[48];
		int iZec71[48];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 48); vindex = vindex + 48) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 48;
			/* Vectorizable loop 0 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iVec0_tmp[j0] = iVec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iVec0_perm[j1] = iVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				iRec36_tmp[j8] = iRec36_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec36[i] = iSlow5 * (iRec36[i - 1] + 1);
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				iRec36_perm[j9] = iRec36_tmp[vsize + j9];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec37_tmp[j10] = fRec37_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec37[i] = fSlow7 * std::fmod(fRec37[i - 1] + 1.0f, fSlow6);
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec37_perm[j11] = fRec37_tmp[vsize + j11];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = std::max<int>(0, std::min<int>(iRec36[i], 47999));
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = std::max<int>(0, std::min<int>(int(fRec37[i]), 47999));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec2[i]] = float(input0[i]);
				fZec4[i] = fSlow11 * (fSlow9 * ftbl0[iZec3[i]] + fSlow10 * float(input0[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl2[iZec2[i]] = float(input1[i]);
				fZec52[i] = fSlow11 * (fSlow9 * ftbl2[iZec3[i]] + fSlow10 * float(input1[i]));
			}
			/* Vectorizable loop 7 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fYec0_tmp[j12] = fYec0_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = ((iSlow8) ? 0.0f : fZec4[i]);
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fYec0_perm[j13] = fYec0_tmp[vsize + j13];
			}
			/* Vectorizable loop 8 */
			/* Pre code */
			for (int j94 = 0; j94 < 4; j94 = j94 + 1) {
				fYec3_tmp[j94] = fYec3_perm[j94];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec3[i] = ((iSlow8) ? 0.0f : fZec52[i]);
			}
			/* Post code */
			for (int j95 = 0; j95 < 4; j95 = j95 + 1) {
				fYec3_perm[j95] = fYec3_tmp[vsize + j95];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec39_tmp[j18] = fRec39_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec39[i] = 0.0f - fConst8 * (fConst6 * fRec39[i - 1] - (fYec0[i] + fYec0[i - 1]));
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec39_perm[j19] = fRec39_tmp[vsize + j19];
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec35_tmp[j14] = fRec35_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec35[i] = fConst9 * fYec0[i - 1] + fConst8 * (fConst5 * fYec0[i] - fConst6 * fRec35[i - 1]);
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec35_perm[j15] = fRec35_tmp[vsize + j15];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j96 = 0; j96 < 4; j96 = j96 + 1) {
				fRec79_tmp[j96] = fRec79_perm[j96];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec79[i] = fConst9 * fYec3[i - 1] + fConst8 * (fConst5 * fYec3[i] - fConst6 * fRec79[i - 1]);
			}
			/* Post code */
			for (int j97 = 0; j97 < 4; j97 = j97 + 1) {
				fRec79_perm[j97] = fRec79_tmp[vsize + j97];
			}
			/* Recursive loop 12 */
			/* Pre code */
			for (int j100 = 0; j100 < 4; j100 = j100 + 1) {
				fRec81_tmp[j100] = fRec81_perm[j100];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec81[i] = fConst8 * (fYec3[i] + fYec3[i - 1] - fConst6 * fRec81[i - 1]);
			}
			/* Post code */
			for (int j101 = 0; j101 < 4; j101 = j101 + 1) {
				fRec81_perm[j101] = fRec81_tmp[vsize + j101];
			}
			/* Recursive loop 13 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec38_tmp[j20] = fRec38_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec38[i] = fRec39[i] - fConst14 * (fConst13 * fRec38[i - 2] + fConst12 * fRec38[i - 1]);
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec38_perm[j21] = fRec38_tmp[vsize + j21];
			}
			/* Recursive loop 14 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec34_tmp[j16] = fRec34_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec34[i] = fRec35[i] - fConst14 * (fConst13 * fRec34[i - 2] + fConst12 * fRec34[i - 1]);
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec34_perm[j17] = fRec34_tmp[vsize + j17];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j98 = 0; j98 < 4; j98 = j98 + 1) {
				fRec78_tmp[j98] = fRec78_perm[j98];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec78[i] = fRec79[i] - fConst14 * (fConst13 * fRec78[i - 2] + fConst12 * fRec78[i - 1]);
			}
			/* Post code */
			for (int j99 = 0; j99 < 4; j99 = j99 + 1) {
				fRec78_perm[j99] = fRec78_tmp[vsize + j99];
			}
			/* Recursive loop 16 */
			/* Pre code */
			for (int j102 = 0; j102 < 4; j102 = j102 + 1) {
				fRec80_tmp[j102] = fRec80_perm[j102];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec80[i] = fRec81[i] - fConst14 * (fConst13 * fRec80[i - 2] + fConst12 * fRec80[i - 1]);
			}
			/* Post code */
			for (int j103 = 0; j103 < 4; j103 = j103 + 1) {
				fRec80_perm[j103] = fRec80_tmp[vsize + j103];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iSlow8) ? fZec4[i] : fConst14 * (fConst11 * fRec34[i] + fConst19 * fRec34[i - 1] + fConst11 * fRec34[i - 2] + fSlow19 * (fRec38[i - 2] + fRec38[i] + 2.0f * fRec38[i - 1])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec54[i] = ((iSlow8) ? fZec52[i] : fConst14 * (fConst19 * fRec78[i - 1] + fConst11 * fRec78[i] + fConst11 * fRec78[i - 2] + fSlow19 * (fRec80[i - 2] + fRec80[i] + 2.0f * fRec80[i - 1])));
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec33_tmp[j22] = fRec33_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fConst18 * fRec33[i - 1];
				fRec33[i] = ((iSlow18) ? 0.0f : fZec6[i]) - (fRec33[i - 2] * fSlow17 + fZec5[i]) / fSlow16;
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec33_perm[j23] = fRec33_tmp[vsize + j23];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j104 = 0; j104 < 4; j104 = j104 + 1) {
				fRec77_tmp[j104] = fRec77_perm[j104];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec53[i] = fConst18 * fRec77[i - 1];
				fRec77[i] = ((iSlow18) ? 0.0f : fZec54[i]) - (fSlow17 * fRec77[i - 2] + fZec53[i]) / fSlow16;
			}
			/* Post code */
			for (int j105 = 0; j105 < 4; j105 = j105 + 1) {
				fRec77_perm[j105] = fRec77_tmp[vsize + j105];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = ((iSlow18) ? fZec6[i] : (fZec5[i] + fRec33[i] * fSlow23 + fRec33[i - 2] * fSlow22) / fSlow16);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec55[i] = ((iSlow18) ? fZec54[i] : (fZec53[i] + fRec77[i] * fSlow23 + fSlow22 * fRec77[i - 2]) / fSlow16);
			}
			/* Vectorizable loop 23 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fYec1_tmp[j24] = fYec1_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = ((iSlow20) ? 0.0f : fZec7[i]);
			}
			/* Post code */
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fYec1_perm[j25] = fYec1_tmp[vsize + j25];
			}
			/* Vectorizable loop 24 */
			/* Pre code */
			for (int j106 = 0; j106 < 4; j106 = j106 + 1) {
				fYec4_tmp[j106] = fYec4_perm[j106];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec4[i] = ((iSlow20) ? 0.0f : fZec55[i]);
			}
			/* Post code */
			for (int j107 = 0; j107 < 4; j107 = j107 + 1) {
				fYec4_perm[j107] = fYec4_tmp[vsize + j107];
			}
			/* Recursive loop 25 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec32_tmp[j26] = fRec32_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec32[i] = 0.0f - fConst24 * (fConst22 * fRec32[i - 1] - (fYec1[i] + fYec1[i - 1]));
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec32_perm[j27] = fRec32_tmp[vsize + j27];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec41_tmp[j30] = fRec41_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec41[i] = fConst30 * fYec1[i - 1] - fConst24 * (fConst22 * fRec41[i - 1] - fConst21 * fYec1[i]);
			}
			/* Post code */
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec41_perm[j31] = fRec41_tmp[vsize + j31];
			}
			/* Recursive loop 27 */
			/* Pre code */
			for (int j108 = 0; j108 < 4; j108 = j108 + 1) {
				fRec76_tmp[j108] = fRec76_perm[j108];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec76[i] = fConst24 * (fYec4[i] + fYec4[i - 1] - fConst22 * fRec76[i - 1]);
			}
			/* Post code */
			for (int j109 = 0; j109 < 4; j109 = j109 + 1) {
				fRec76_perm[j109] = fRec76_tmp[vsize + j109];
			}
			/* Recursive loop 28 */
			/* Pre code */
			for (int j112 = 0; j112 < 4; j112 = j112 + 1) {
				fRec83_tmp[j112] = fRec83_perm[j112];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec83[i] = fConst30 * fYec4[i - 1] - fConst24 * (fConst22 * fRec83[i - 1] - fConst21 * fYec4[i]);
			}
			/* Post code */
			for (int j113 = 0; j113 < 4; j113 = j113 + 1) {
				fRec83_perm[j113] = fRec83_tmp[vsize + j113];
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec40_tmp[j32] = fRec40_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec40[i] = fRec41[i] - fConst29 * (fConst28 * fRec40[i - 2] + fConst27 * fRec40[i - 1]);
			}
			/* Post code */
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec40_perm[j33] = fRec40_tmp[vsize + j33];
			}
			/* Recursive loop 30 */
			/* Pre code */
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec31_tmp[j28] = fRec31_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec31[i] = fRec32[i] - fConst29 * (fConst28 * fRec31[i - 2] + fConst27 * fRec31[i - 1]);
			}
			/* Post code */
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec31_perm[j29] = fRec31_tmp[vsize + j29];
			}
			/* Recursive loop 31 */
			/* Pre code */
			for (int j114 = 0; j114 < 4; j114 = j114 + 1) {
				fRec82_tmp[j114] = fRec82_perm[j114];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec82[i] = fRec83[i] - fConst29 * (fConst28 * fRec82[i - 2] + fConst27 * fRec82[i - 1]);
			}
			/* Post code */
			for (int j115 = 0; j115 < 4; j115 = j115 + 1) {
				fRec82_perm[j115] = fRec82_tmp[vsize + j115];
			}
			/* Recursive loop 32 */
			/* Pre code */
			for (int j110 = 0; j110 < 4; j110 = j110 + 1) {
				fRec75_tmp[j110] = fRec75_perm[j110];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec75[i] = fRec76[i] - fConst29 * (fConst28 * fRec75[i - 2] + fConst27 * fRec75[i - 1]);
			}
			/* Post code */
			for (int j111 = 0; j111 < 4; j111 = j111 + 1) {
				fRec75_perm[j111] = fRec75_tmp[vsize + j111];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = ((iSlow20) ? fZec7[i] : fConst29 * (fRec31[i - 2] + fRec31[i] + 2.0f * fRec31[i - 1] + fSlow32 * (fConst32 * fRec40[i - 1] + fConst26 * fRec40[i] + fConst26 * fRec40[i - 2])));
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec56[i] = ((iSlow20) ? fZec55[i] : fConst29 * (fRec75[i - 2] + fRec75[i] + 2.0f * fRec75[i - 1] + fSlow32 * (fConst26 * fRec82[i] + fConst32 * fRec82[i - 1] + fConst26 * fRec82[i - 2])));
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec30_tmp[j34] = fRec30_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec30[i] = ((iSlow31) ? 0.0f : fZec8[i]) - fSlow30 * (fSlow29 * fRec30[i - 2] + fSlow27 * fRec30[i - 1]);
			}
			/* Post code */
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec30_perm[j35] = fRec30_tmp[vsize + j35];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j116 = 0; j116 < 4; j116 = j116 + 1) {
				fRec74_tmp[j116] = fRec74_perm[j116];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec74[i] = ((iSlow31) ? 0.0f : fZec56[i]) - fSlow30 * (fSlow29 * fRec74[i - 2] + fSlow27 * fRec74[i - 1]);
			}
			/* Post code */
			for (int j117 = 0; j117 < 4; j117 = j117 + 1) {
				fRec74_perm[j117] = fRec74_tmp[vsize + j117];
			}
			/* Recursive loop 37 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec29_tmp[j36] = fRec29_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec29[i] = fSlow30 * (fSlow26 * fRec30[i] + fSlow35 * fRec30[i - 1] + fSlow26 * fRec30[i - 2]) - fSlow34 * (fSlow33 * fRec29[i - 2] + fSlow27 * fRec29[i - 1]);
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec29_perm[j37] = fRec29_tmp[vsize + j37];
			}
			/* Recursive loop 38 */
			/* Pre code */
			for (int j118 = 0; j118 < 4; j118 = j118 + 1) {
				fRec73_tmp[j118] = fRec73_perm[j118];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec73[i] = fSlow30 * (fSlow35 * fRec74[i - 1] + fSlow26 * fRec74[i] + fSlow26 * fRec74[i - 2]) - fSlow34 * (fSlow33 * fRec73[i - 2] + fSlow27 * fRec73[i - 1]);
			}
			/* Post code */
			for (int j119 = 0; j119 < 4; j119 = j119 + 1) {
				fRec73_perm[j119] = fRec73_tmp[vsize + j119];
			}
			/* Recursive loop 39 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec28_tmp[j38] = fRec28_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec28[i] = fSlow34 * (fSlow35 * fRec29[i - 1] + fSlow26 * fRec29[i] + fSlow26 * fRec29[i - 2]) - fSlow37 * (fSlow36 * fRec28[i - 2] + fSlow27 * fRec28[i - 1]);
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec28_perm[j39] = fRec28_tmp[vsize + j39];
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j120 = 0; j120 < 4; j120 = j120 + 1) {
				fRec72_tmp[j120] = fRec72_perm[j120];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec72[i] = fSlow34 * (fSlow26 * fRec73[i] + fSlow35 * fRec73[i - 1] + fSlow26 * fRec73[i - 2]) - fSlow37 * (fSlow36 * fRec72[i - 2] + fSlow27 * fRec72[i - 1]);
			}
			/* Post code */
			for (int j121 = 0; j121 < 4; j121 = j121 + 1) {
				fRec72_perm[j121] = fRec72_tmp[vsize + j121];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec27_tmp[j40] = fRec27_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec27[i] = fSlow37 * (fSlow26 * fRec28[i] + fSlow35 * fRec28[i - 1] + fSlow26 * fRec28[i - 2]) - fSlow39 * (fSlow38 * fRec27[i - 2] + fSlow27 * fRec27[i - 1]);
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec27_perm[j41] = fRec27_tmp[vsize + j41];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j122 = 0; j122 < 4; j122 = j122 + 1) {
				fRec71_tmp[j122] = fRec71_perm[j122];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec71[i] = fSlow37 * (fSlow26 * fRec72[i] + fSlow35 * fRec72[i - 1] + fSlow26 * fRec72[i - 2]) - fSlow39 * (fSlow38 * fRec71[i - 2] + fSlow27 * fRec71[i - 1]);
			}
			/* Post code */
			for (int j123 = 0; j123 < 4; j123 = j123 + 1) {
				fRec71_perm[j123] = fRec71_tmp[vsize + j123];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec26_tmp[j42] = fRec26_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = fSlow39 * (fSlow35 * fRec27[i - 1] + fSlow26 * fRec27[i] + fSlow26 * fRec27[i - 2]) - fSlow41 * (fSlow40 * fRec26[i - 2] + fSlow27 * fRec26[i - 1]);
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec26_perm[j43] = fRec26_tmp[vsize + j43];
			}
			/* Recursive loop 44 */
			/* Pre code */
			for (int j124 = 0; j124 < 4; j124 = j124 + 1) {
				fRec70_tmp[j124] = fRec70_perm[j124];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec70[i] = fSlow39 * (fSlow35 * fRec71[i - 1] + fSlow26 * fRec71[i] + fSlow26 * fRec71[i - 2]) - fSlow41 * (fSlow40 * fRec70[i - 2] + fSlow27 * fRec70[i - 1]);
			}
			/* Post code */
			for (int j125 = 0; j125 < 4; j125 = j125 + 1) {
				fRec70_perm[j125] = fRec70_tmp[vsize + j125];
			}
			/* Recursive loop 45 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec25_tmp[j44] = fRec25_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = fSlow41 * (fSlow35 * fRec26[i - 1] + fSlow26 * fRec26[i] + fSlow26 * fRec26[i - 2]) - fSlow43 * (fSlow42 * fRec25[i - 2] + fSlow27 * fRec25[i - 1]);
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec25_perm[j45] = fRec25_tmp[vsize + j45];
			}
			/* Recursive loop 46 */
			/* Pre code */
			for (int j126 = 0; j126 < 4; j126 = j126 + 1) {
				fRec69_tmp[j126] = fRec69_perm[j126];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec69[i] = fSlow41 * (fSlow26 * fRec70[i] + fSlow35 * fRec70[i - 1] + fSlow26 * fRec70[i - 2]) - fSlow43 * (fSlow42 * fRec69[i - 2] + fSlow27 * fRec69[i - 1]);
			}
			/* Post code */
			for (int j127 = 0; j127 < 4; j127 = j127 + 1) {
				fRec69_perm[j127] = fRec69_tmp[vsize + j127];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iSlow31) ? fZec8[i] : fSlow43 * (fSlow26 * fRec25[i] + fSlow35 * fRec25[i - 1] + fSlow26 * fRec25[i - 2]));
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec57[i] = ((iSlow31) ? fZec56[i] : fSlow43 * (fSlow26 * fRec69[i] + fSlow35 * fRec69[i - 1] + fSlow26 * fRec69[i - 2]));
			}
			/* Recursive loop 49 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec24_tmp[j46] = fRec24_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = ((iSlow49) ? 0.0f : fZec9[i]) - fSlow48 * (fSlow47 * fRec24[i - 2] + fSlow45 * fRec24[i - 1]);
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec24_perm[j47] = fRec24_tmp[vsize + j47];
			}
			/* Recursive loop 50 */
			/* Pre code */
			for (int j128 = 0; j128 < 4; j128 = j128 + 1) {
				fRec68_tmp[j128] = fRec68_perm[j128];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec68[i] = ((iSlow49) ? 0.0f : fZec57[i]) - fSlow48 * (fSlow47 * fRec68[i - 2] + fSlow45 * fRec68[i - 1]);
			}
			/* Post code */
			for (int j129 = 0; j129 < 4; j129 = j129 + 1) {
				fRec68_perm[j129] = fRec68_tmp[vsize + j129];
			}
			/* Recursive loop 51 */
			/* Pre code */
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec23_tmp[j48] = fRec23_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = fSlow48 * (fRec24[i - 2] + fRec24[i] + 2.0f * fRec24[i - 1]) - fSlow51 * (fSlow50 * fRec23[i - 2] + fSlow45 * fRec23[i - 1]);
			}
			/* Post code */
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec23_perm[j49] = fRec23_tmp[vsize + j49];
			}
			/* Recursive loop 52 */
			/* Pre code */
			for (int j130 = 0; j130 < 4; j130 = j130 + 1) {
				fRec67_tmp[j130] = fRec67_perm[j130];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec67[i] = fSlow48 * (fRec68[i - 2] + fRec68[i] + 2.0f * fRec68[i - 1]) - fSlow51 * (fSlow50 * fRec67[i - 2] + fSlow45 * fRec67[i - 1]);
			}
			/* Post code */
			for (int j131 = 0; j131 < 4; j131 = j131 + 1) {
				fRec67_perm[j131] = fRec67_tmp[vsize + j131];
			}
			/* Recursive loop 53 */
			/* Pre code */
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec22_tmp[j50] = fRec22_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = fSlow51 * (fRec23[i - 2] + fRec23[i] + 2.0f * fRec23[i - 1]) - fSlow53 * (fSlow52 * fRec22[i - 2] + fSlow45 * fRec22[i - 1]);
			}
			/* Post code */
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec22_perm[j51] = fRec22_tmp[vsize + j51];
			}
			/* Recursive loop 54 */
			/* Pre code */
			for (int j132 = 0; j132 < 4; j132 = j132 + 1) {
				fRec66_tmp[j132] = fRec66_perm[j132];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec66[i] = fSlow51 * (fRec67[i - 2] + fRec67[i] + 2.0f * fRec67[i - 1]) - fSlow53 * (fSlow52 * fRec66[i - 2] + fSlow45 * fRec66[i - 1]);
			}
			/* Post code */
			for (int j133 = 0; j133 < 4; j133 = j133 + 1) {
				fRec66_perm[j133] = fRec66_tmp[vsize + j133];
			}
			/* Recursive loop 55 */
			/* Pre code */
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec21_tmp[j52] = fRec21_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = fSlow53 * (fRec22[i - 2] + fRec22[i] + 2.0f * fRec22[i - 1]) - fSlow55 * (fSlow54 * fRec21[i - 2] + fSlow45 * fRec21[i - 1]);
			}
			/* Post code */
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec21_perm[j53] = fRec21_tmp[vsize + j53];
			}
			/* Recursive loop 56 */
			/* Pre code */
			for (int j134 = 0; j134 < 4; j134 = j134 + 1) {
				fRec65_tmp[j134] = fRec65_perm[j134];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec65[i] = fSlow53 * (fRec66[i - 2] + fRec66[i] + 2.0f * fRec66[i - 1]) - fSlow55 * (fSlow54 * fRec65[i - 2] + fSlow45 * fRec65[i - 1]);
			}
			/* Post code */
			for (int j135 = 0; j135 < 4; j135 = j135 + 1) {
				fRec65_perm[j135] = fRec65_tmp[vsize + j135];
			}
			/* Recursive loop 57 */
			/* Pre code */
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec20_tmp[j54] = fRec20_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = fSlow55 * (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) - fSlow57 * (fSlow56 * fRec20[i - 2] + fSlow45 * fRec20[i - 1]);
			}
			/* Post code */
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec20_perm[j55] = fRec20_tmp[vsize + j55];
			}
			/* Recursive loop 58 */
			/* Pre code */
			for (int j136 = 0; j136 < 4; j136 = j136 + 1) {
				fRec64_tmp[j136] = fRec64_perm[j136];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec64[i] = fSlow55 * (fRec65[i - 2] + fRec65[i] + 2.0f * fRec65[i - 1]) - fSlow57 * (fSlow56 * fRec64[i - 2] + fSlow45 * fRec64[i - 1]);
			}
			/* Post code */
			for (int j137 = 0; j137 < 4; j137 = j137 + 1) {
				fRec64_perm[j137] = fRec64_tmp[vsize + j137];
			}
			/* Recursive loop 59 */
			/* Pre code */
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec19_tmp[j56] = fRec19_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = fSlow57 * (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) - fSlow59 * (fSlow58 * fRec19[i - 2] + fSlow45 * fRec19[i - 1]);
			}
			/* Post code */
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec19_perm[j57] = fRec19_tmp[vsize + j57];
			}
			/* Recursive loop 60 */
			/* Pre code */
			for (int j138 = 0; j138 < 4; j138 = j138 + 1) {
				fRec63_tmp[j138] = fRec63_perm[j138];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec63[i] = fSlow57 * (fRec64[i - 2] + fRec64[i] + 2.0f * fRec64[i - 1]) - fSlow59 * (fSlow58 * fRec63[i - 2] + fSlow45 * fRec63[i - 1]);
			}
			/* Post code */
			for (int j139 = 0; j139 < 4; j139 = j139 + 1) {
				fRec63_perm[j139] = fRec63_tmp[vsize + j139];
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = ((iSlow49) ? fZec9[i] : fSlow59 * (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]));
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec58[i] = ((iSlow49) ? fZec57[i] : fSlow59 * (fRec63[i - 2] + fRec63[i] + 2.0f * fRec63[i - 1]));
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iSlow60) ? 0.0f : fZec10[i]);
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec59[i] = ((iSlow60) ? 0.0f : fZec58[i]);
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 66 */
			/* Pre code */
			fRec18_idx = (fRec18_idx + fRec18_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[(i + fRec18_idx) & 2097151] = fSlow62 * fRec18[(i + fRec18_idx - iSlow61) & 2097151] + fZec11[i];
			}
			/* Post code */
			fRec18_idx_save = vsize;
			/* Recursive loop 67 */
			/* Pre code */
			fRec62_idx = (fRec62_idx + fRec62_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec62[(i + fRec62_idx) & 2097151] = fSlow62 * fRec62[(i + fRec62_idx - iSlow61) & 2097151] + fZec59[i];
			}
			/* Post code */
			fRec62_idx_save = vsize;
			/* Recursive loop 68 */
			/* Pre code */
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec42_tmp[j58] = fRec42_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((iZec0[i] | iSlow63) ? 0.0f : fSlow64 + fRec42[i - 1]);
				fRec42[i] = fZec12[i] - std::floor(fZec12[i]);
			}
			/* Post code */
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec42_perm[j59] = fRec42_tmp[vsize + j59];
			}
			/* Vectorizable loop 69 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = ((iSlow60) ? fZec10[i] : fSlow68 * fRec18[(i + fRec18_idx) & 2097151] + fSlow69 * fZec11[i]);
			}
			/* Vectorizable loop 70 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec60[i] = ((iSlow60) ? fZec58[i] : fSlow68 * fRec62[(i + fRec62_idx) & 2097151] + fSlow69 * fZec59[i]);
			}
			/* Vectorizable loop 71 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow70 * ((iSlow67) ? 0.0f : fZec18[i])));
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = std::tan(fConst31 * (fSlow65 + fSlow66 * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec42[i]), 65535))] + 1.0f)));
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow70 * ((iSlow67) ? 0.0f : fZec60[i])));
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = ((iSlow67) ? fZec18[i] : fZec19[i] * (1.0f - 0.33333334f * mydsp_faustpower2_f(fZec19[i])));
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = 1.0f / fZec13[i];
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec62[i] = ((iSlow67) ? fZec60[i] : fZec61[i] * (1.0f - 0.33333334f * mydsp_faustpower2_f(fZec61[i])));
			}
			/* Vectorizable loop 77 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = ((iSlow63) ? 0.0f : fZec20[i]);
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - 1.0f / mydsp_faustpower2_f(fZec13[i]);
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = (fZec14[i] + -1.9828898f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = (fZec14[i] + 1.9828898f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 81 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec63[i] = ((iSlow63) ? 0.0f : fZec62[i]);
			}
			/* Vectorizable loop 82 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = (fZec14[i] + 1.847759f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = (fZec14[i] + -1.847759f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 84 */
			/* Pre code */
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec17_tmp[j60] = fRec17_perm[j60];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fZec21[i] - (fRec17[i - 2] * fZec17[i] + 2.0f * fRec17[i - 1] * fZec16[i]) / fZec15[i];
			}
			/* Post code */
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec17_perm[j61] = fRec17_tmp[vsize + j61];
			}
			/* Recursive loop 85 */
			/* Pre code */
			for (int j140 = 0; j140 < 4; j140 = j140 + 1) {
				fRec61_tmp[j140] = fRec61_perm[j140];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec61[i] = fZec63[i] - (fZec17[i] * fRec61[i - 2] + 2.0f * fZec16[i] * fRec61[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j141 = 0; j141 < 4; j141 = j141 + 1) {
				fRec61_perm[j141] = fRec61_tmp[vsize + j141];
			}
			/* Recursive loop 86 */
			/* Pre code */
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec16_tmp[j62] = fRec16_perm[j62];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + fRec17[i] + 2.0f * fRec17[i - 1]) / fZec15[i] - (fRec16[i - 2] * fZec23[i] + 2.0f * fZec16[i] * fRec16[i - 1]) / fZec22[i];
			}
			/* Post code */
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec16_perm[j63] = fRec16_tmp[vsize + j63];
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = (fZec14[i] + 1.5867066f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 88 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = (fZec14[i] + -1.5867066f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 89 */
			/* Pre code */
			for (int j142 = 0; j142 < 4; j142 = j142 + 1) {
				fRec60_tmp[j142] = fRec60_perm[j142];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec60[i] = (fRec61[i - 2] + fRec61[i] + 2.0f * fRec61[i - 1]) / fZec15[i] - (fZec23[i] * fRec60[i - 2] + 2.0f * fZec16[i] * fRec60[i - 1]) / fZec22[i];
			}
			/* Post code */
			for (int j143 = 0; j143 < 4; j143 = j143 + 1) {
				fRec60_perm[j143] = fRec60_tmp[vsize + j143];
			}
			/* Recursive loop 90 */
			/* Pre code */
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec15_tmp[j64] = fRec15_perm[j64];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = (fRec16[i - 2] + fRec16[i] + 2.0f * fRec16[i - 1]) / fZec22[i] - (fRec15[i - 2] * fZec25[i] + 2.0f * fZec16[i] * fRec15[i - 1]) / fZec24[i];
			}
			/* Post code */
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec15_perm[j65] = fRec15_tmp[vsize + j65];
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = (fZec14[i] + 1.2175229f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = (fZec14[i] + -1.2175229f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 93 */
			/* Pre code */
			for (int j144 = 0; j144 < 4; j144 = j144 + 1) {
				fRec59_tmp[j144] = fRec59_perm[j144];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec59[i] = (fRec60[i - 2] + fRec60[i] + 2.0f * fRec60[i - 1]) / fZec22[i] - (fZec25[i] * fRec59[i - 2] + 2.0f * fZec16[i] * fRec59[i - 1]) / fZec24[i];
			}
			/* Post code */
			for (int j145 = 0; j145 < 4; j145 = j145 + 1) {
				fRec59_perm[j145] = fRec59_tmp[vsize + j145];
			}
			/* Recursive loop 94 */
			/* Pre code */
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec14_tmp[j66] = fRec14_perm[j66];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = (fRec15[i - 2] + fRec15[i] + 2.0f * fRec15[i - 1]) / fZec24[i] - (fRec14[i - 2] * fZec27[i] + 2.0f * fZec16[i] * fRec14[i - 1]) / fZec26[i];
			}
			/* Post code */
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec14_perm[j67] = fRec14_tmp[vsize + j67];
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = (fZec14[i] + -0.76536685f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = (fZec14[i] + 0.76536685f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 97 */
			/* Pre code */
			for (int j146 = 0; j146 < 4; j146 = j146 + 1) {
				fRec58_tmp[j146] = fRec58_perm[j146];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec58[i] = (fRec59[i - 2] + fRec59[i] + 2.0f * fRec59[i - 1]) / fZec24[i] - (fZec27[i] * fRec58[i - 2] + 2.0f * fZec16[i] * fRec58[i - 1]) / fZec26[i];
			}
			/* Post code */
			for (int j147 = 0; j147 < 4; j147 = j147 + 1) {
				fRec58_perm[j147] = fRec58_tmp[vsize + j147];
			}
			/* Recursive loop 98 */
			/* Pre code */
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec13_tmp[j68] = fRec13_perm[j68];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = (fRec14[i - 2] + fRec14[i] + 2.0f * fRec14[i - 1]) / fZec26[i] - (fRec13[i - 2] * fZec29[i] + 2.0f * fZec16[i] * fRec13[i - 1]) / fZec28[i];
			}
			/* Post code */
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec13_perm[j69] = fRec13_tmp[vsize + j69];
			}
			/* Vectorizable loop 99 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = (fZec14[i] + -0.26105237f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 100 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec14[i] + 0.26105237f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 101 */
			/* Pre code */
			for (int j148 = 0; j148 < 4; j148 = j148 + 1) {
				fRec57_tmp[j148] = fRec57_perm[j148];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec57[i] = (fRec58[i - 2] + fRec58[i] + 2.0f * fRec58[i - 1]) / fZec26[i] - (fZec29[i] * fRec57[i - 2] + 2.0f * fZec16[i] * fRec57[i - 1]) / fZec28[i];
			}
			/* Post code */
			for (int j149 = 0; j149 < 4; j149 = j149 + 1) {
				fRec57_perm[j149] = fRec57_tmp[vsize + j149];
			}
			/* Recursive loop 102 */
			/* Pre code */
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec12_tmp[j70] = fRec12_perm[j70];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = (fRec13[i - 2] + fRec13[i] + 2.0f * fRec13[i - 1]) / fZec28[i] - (fRec12[i - 2] * fZec31[i] + 2.0f * fZec16[i] * fRec12[i - 1]) / fZec30[i];
			}
			/* Post code */
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec12_perm[j71] = fRec12_tmp[vsize + j71];
			}
			/* Recursive loop 103 */
			/* Pre code */
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec46_tmp[j74] = fRec46_perm[j74];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = ((iZec0[i] | iSlow73) ? 0.0f : fSlow74 + fRec46[i - 1]);
				fRec46[i] = fZec33[i] - std::floor(fZec33[i]);
			}
			/* Post code */
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec46_perm[j75] = fRec46_tmp[vsize + j75];
			}
			/* Recursive loop 104 */
			/* Pre code */
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec44_tmp[j72] = fRec44_perm[j72];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = ((iZec0[i] | iSlow71) ? 0.0f : fSlow72 + fRec44[i - 1]);
				fRec44[i] = fZec32[i] - std::floor(fZec32[i]);
			}
			/* Post code */
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec44_perm[j73] = fRec44_tmp[vsize + j73];
			}
			/* Recursive loop 105 */
			/* Pre code */
			for (int j150 = 0; j150 < 4; j150 = j150 + 1) {
				fRec56_tmp[j150] = fRec56_perm[j150];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec56[i] = (fRec57[i - 2] + fRec57[i] + 2.0f * fRec57[i - 1]) / fZec28[i] - (fZec31[i] * fRec56[i - 2] + 2.0f * fZec16[i] * fRec56[i - 1]) / fZec30[i];
			}
			/* Post code */
			for (int j151 = 0; j151 < 4; j151 = j151 + 1) {
				fRec56_perm[j151] = fRec56_tmp[vsize + j151];
			}
			/* Vectorizable loop 106 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = fSlow77 + fSlow76 * std::ceil(0.0f - (fRec44[i] - fSlow75));
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = ((iSlow63) ? fZec20[i] : fSlow78 * ((fRec12[i - 2] + fRec12[i] + 2.0f * fRec12[i - 1]) / fZec30[i]) + fSlow79 * fZec21[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = 3e+01f * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec46[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec64[i] = ((iSlow63) ? fZec62[i] : fSlow78 * ((fRec56[i - 2] + fRec56[i] + 2.0f * fRec56[i - 1]) / fZec30[i]) + fSlow79 * fZec63[i]);
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = ((iSlow71) ? fZec35[i] : ((iSlow71) ? 0.0f : fZec35[i]) * fZec34[i]);
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::floor(fZec37[i]);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec39[i] = int(fZec37[i]);
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = fZec37[i] + 1.0f;
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec65[i] = ((iSlow71) ? fZec64[i] : fZec34[i] * ((iSlow71) ? 0.0f : fZec64[i]));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec44[i] = int(std::min<float>(fZec38[i], float(std::max<int>(0, iZec39[i]))));
			}
			/* Vectorizable loop 116 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec43[i] = fZec41[i] + (1.0f - fZec37[i]);
			}
			/* Vectorizable loop 117 */
			/* Pre code */
			fYec2_idx = (fYec2_idx + fYec2_idx_save) & 127;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec2[(i + fYec2_idx) & 127] = ((iSlow73) ? 0.0f : fZec36[i]);
			}
			/* Post code */
			fYec2_idx_save = vsize;
			/* Vectorizable loop 118 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec40[i] = int(std::min<float>(fZec38[i], float(std::max<int>(0, iZec39[i] + 1))));
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = fZec37[i] - fZec41[i];
			}
			/* Vectorizable loop 120 */
			/* Pre code */
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 127;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec5[(i + fYec5_idx) & 127] = ((iSlow73) ? 0.0f : fZec65[i]);
			}
			/* Post code */
			fYec5_idx_save = vsize;
			/* Recursive loop 121 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec45[i] = 0.0f - (fYec2[(i + fYec2_idx - iZec44[i]) & 127] * fZec43[i] + fZec42[i] * fYec2[(i + fYec2_idx - iZec40[i]) & 127]);
			}
			/* Recursive loop 122 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec84[i] = 0.0f - (fZec43[i] * fYec5[(i + fYec5_idx - iZec44[i]) & 127] + fZec42[i] * fYec5[(i + fYec5_idx - iZec40[i]) & 127]);
			}
			/* Recursive loop 123 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec9_tmp[j4] = fRec9_perm[j4];
			}
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec10_tmp[j6] = fRec10_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = fSlow4 * fRec10[i - 1] + fSlow3 * fRec9[i - 1];
				fRec10[i] = float(iZec0[i]) + fSlow3 * fRec10[i - 1] - fSlow4 * fRec9[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec9_perm[j5] = fRec9_tmp[vsize + j5];
			}
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec10_perm[j7] = fRec10_tmp[vsize + j7];
			}
			/* Vectorizable loop 124 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = ((iSlow73) ? fZec36[i] : fSlow85 * (fYec2[(i + fYec2_idx) & 127] + fSlow84 * fRec45[i]) + fSlow83 * fYec2[(i + fYec2_idx) & 127]);
			}
			/* Vectorizable loop 125 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec67[i] = ((iSlow73) ? fZec65[i] : fSlow85 * (fYec5[(i + fYec5_idx) & 127] + fSlow84 * fRec84[i]) + fSlow83 * fYec5[(i + fYec5_idx) & 127]);
			}
			/* Vectorizable loop 126 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = std::cos(fConst1 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Vectorizable loop 127 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = ((iSlow81) ? 0.0f : fZec46[i]);
			}
			/* Vectorizable loop 128 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec66[i] = std::cos(fConst1 * (14137.167f * (1.0f - fRec10[i]) + 3141.5928f));
			}
			/* Vectorizable loop 129 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec68[i] = ((iSlow81) ? 0.0f : fZec67[i]);
			}
			/* Recursive loop 130 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((iZec0[i] | iSlow0) ? 0.0f : fSlow1 + fRec0[i - 1]);
				fRec0[i] = fZec1[i] - std::floor(fZec1[i]);
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Recursive loop 131 */
			/* Pre code */
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec11_tmp[j76] = fRec11_perm[j76];
			}
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec8_tmp[j78] = fRec8_perm[j78];
			}
			for (int j80 = 0; j80 < 4; j80 = j80 + 1) {
				fRec7_tmp[j80] = fRec7_perm[j80];
			}
			for (int j82 = 0; j82 < 4; j82 = j82 + 1) {
				fRec6_tmp[j82] = fRec6_perm[j82];
			}
			for (int j84 = 0; j84 < 4; j84 = j84 + 1) {
				fRec5_tmp[j84] = fRec5_perm[j84];
			}
			for (int j86 = 0; j86 < 4; j86 = j86 + 1) {
				fRec4_tmp[j86] = fRec4_perm[j86];
			}
			for (int j88 = 0; j88 < 4; j88 = j88 + 1) {
				fRec3_tmp[j88] = fRec3_perm[j88];
			}
			for (int j90 = 0; j90 < 4; j90 = j90 + 1) {
				fRec2_tmp[j90] = fRec2_perm[j90];
			}
			for (int j92 = 0; j92 < 4; j92 = j92 + 1) {
				fRec1_tmp[j92] = fRec1_perm[j92];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = fZec47[i] + fSlow80 * fRec1[i - 1] - (fConst36 * fRec11[i - 1] * fZec45[i] + fConst35 * fRec11[i - 2]);
				fRec8[i] = fConst36 * fZec45[i] * (fRec11[i - 1] - fRec8[i - 1]) + fRec11[i - 2] + fConst35 * (fRec11[i] - fRec8[i - 2]);
				fRec7[i] = fRec8[i - 2] + fConst36 * fZec45[i] * (fRec8[i - 1] - fRec7[i - 1]) - fConst35 * (fRec7[i - 2] - fRec8[i]);
				fRec6[i] = fRec7[i - 2] + fConst35 * (fRec7[i] - fRec6[i - 2]) - fConst36 * fZec45[i] * (fRec6[i - 1] - fRec7[i - 1]);
				fRec5[i] = fRec6[i - 2] + fConst36 * fZec45[i] * (fRec6[i - 1] - fRec5[i - 1]) - fConst35 * (fRec5[i - 2] - fRec6[i]);
				fRec4[i] = fRec5[i - 2] - (fConst36 * fZec45[i] * (fRec4[i - 1] - fRec5[i - 1]) + fConst35 * (fRec4[i - 2] - fRec5[i]));
				fRec3[i] = fRec4[i - 2] + fConst36 * fZec45[i] * (fRec4[i - 1] - fRec3[i - 1]) - fConst35 * (fRec3[i - 2] - fRec4[i]);
				fRec2[i] = fRec3[i - 2] + fConst36 * fZec45[i] * (fRec3[i - 1] - fRec2[i - 1]) - fConst35 * (fRec2[i - 2] - fRec3[i]);
				fRec1[i] = fRec2[i - 2] + fConst36 * fZec45[i] * fRec2[i - 1] + fConst35 * fRec2[i];
			}
			/* Post code */
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec11_perm[j77] = fRec11_tmp[vsize + j77];
			}
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec8_perm[j79] = fRec8_tmp[vsize + j79];
			}
			for (int j81 = 0; j81 < 4; j81 = j81 + 1) {
				fRec7_perm[j81] = fRec7_tmp[vsize + j81];
			}
			for (int j83 = 0; j83 < 4; j83 = j83 + 1) {
				fRec6_perm[j83] = fRec6_tmp[vsize + j83];
			}
			for (int j85 = 0; j85 < 4; j85 = j85 + 1) {
				fRec5_perm[j85] = fRec5_tmp[vsize + j85];
			}
			for (int j87 = 0; j87 < 4; j87 = j87 + 1) {
				fRec4_perm[j87] = fRec4_tmp[vsize + j87];
			}
			for (int j89 = 0; j89 < 4; j89 = j89 + 1) {
				fRec3_perm[j89] = fRec3_tmp[vsize + j89];
			}
			for (int j91 = 0; j91 < 4; j91 = j91 + 1) {
				fRec2_perm[j91] = fRec2_tmp[vsize + j91];
			}
			for (int j93 = 0; j93 < 4; j93 = j93 + 1) {
				fRec1_perm[j93] = fRec1_tmp[vsize + j93];
			}
			/* Recursive loop 132 */
			/* Pre code */
			for (int j152 = 0; j152 < 4; j152 = j152 + 1) {
				fRec55_tmp[j152] = fRec55_perm[j152];
			}
			for (int j154 = 0; j154 < 4; j154 = j154 + 1) {
				fRec54_tmp[j154] = fRec54_perm[j154];
			}
			for (int j156 = 0; j156 < 4; j156 = j156 + 1) {
				fRec53_tmp[j156] = fRec53_perm[j156];
			}
			for (int j158 = 0; j158 < 4; j158 = j158 + 1) {
				fRec52_tmp[j158] = fRec52_perm[j158];
			}
			for (int j160 = 0; j160 < 4; j160 = j160 + 1) {
				fRec51_tmp[j160] = fRec51_perm[j160];
			}
			for (int j162 = 0; j162 < 4; j162 = j162 + 1) {
				fRec50_tmp[j162] = fRec50_perm[j162];
			}
			for (int j164 = 0; j164 < 4; j164 = j164 + 1) {
				fRec49_tmp[j164] = fRec49_perm[j164];
			}
			for (int j166 = 0; j166 < 4; j166 = j166 + 1) {
				fRec48_tmp[j166] = fRec48_perm[j166];
			}
			for (int j168 = 0; j168 < 4; j168 = j168 + 1) {
				fRec47_tmp[j168] = fRec47_perm[j168];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec55[i] = fZec68[i] + fSlow80 * fRec47[i - 1] - (fConst36 * fRec55[i - 1] * fZec66[i] + fConst35 * fRec55[i - 2]);
				fRec54[i] = fRec55[i - 2] + fConst36 * fZec66[i] * (fRec55[i - 1] - fRec54[i - 1]) - fConst35 * (fRec54[i - 2] - fRec55[i]);
				fRec53[i] = fConst36 * fZec66[i] * (fRec54[i - 1] - fRec53[i - 1]) + fRec54[i - 2] + fConst35 * (fRec54[i] - fRec53[i - 2]);
				fRec52[i] = fConst36 * fZec66[i] * (fRec53[i - 1] - fRec52[i - 1]) + fRec53[i - 2] + fConst35 * (fRec53[i] - fRec52[i - 2]);
				fRec51[i] = fRec52[i - 2] - (fConst36 * fZec66[i] * (fRec51[i - 1] - fRec52[i - 1]) + fConst35 * (fRec51[i - 2] - fRec52[i]));
				fRec50[i] = fConst35 * (fRec51[i] - fRec50[i - 2]) + fRec51[i - 2] + fConst36 * fZec66[i] * (fRec51[i - 1] - fRec50[i - 1]);
				fRec49[i] = fConst35 * (fRec50[i] - fRec49[i - 2]) + fRec50[i - 2] + fConst36 * fZec66[i] * (fRec50[i - 1] - fRec49[i - 1]);
				fRec48[i] = fRec49[i - 2] + fConst35 * (fRec49[i] - fRec48[i - 2]) - fConst36 * fZec66[i] * (fRec48[i - 1] - fRec49[i - 1]);
				fRec47[i] = fConst35 * fRec48[i] + fConst36 * fZec66[i] * fRec48[i - 1] + fRec48[i - 2];
			}
			/* Post code */
			for (int j153 = 0; j153 < 4; j153 = j153 + 1) {
				fRec55_perm[j153] = fRec55_tmp[vsize + j153];
			}
			for (int j155 = 0; j155 < 4; j155 = j155 + 1) {
				fRec54_perm[j155] = fRec54_tmp[vsize + j155];
			}
			for (int j157 = 0; j157 < 4; j157 = j157 + 1) {
				fRec53_perm[j157] = fRec53_tmp[vsize + j157];
			}
			for (int j159 = 0; j159 < 4; j159 = j159 + 1) {
				fRec52_perm[j159] = fRec52_tmp[vsize + j159];
			}
			for (int j161 = 0; j161 < 4; j161 = j161 + 1) {
				fRec51_perm[j161] = fRec51_tmp[vsize + j161];
			}
			for (int j163 = 0; j163 < 4; j163 = j163 + 1) {
				fRec50_perm[j163] = fRec50_tmp[vsize + j163];
			}
			for (int j165 = 0; j165 < 4; j165 = j165 + 1) {
				fRec49_perm[j165] = fRec49_tmp[vsize + j165];
			}
			for (int j167 = 0; j167 < 4; j167 = j167 + 1) {
				fRec48_perm[j167] = fRec48_tmp[vsize + j167];
			}
			for (int j169 = 0; j169 < 4; j169 = j169 + 1) {
				fRec47_perm[j169] = fRec47_tmp[vsize + j169];
			}
			/* Vectorizable loop 133 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = fSlow91 * ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec48[i] = ((iSlow81) ? fZec46[i] : fSlow88 * (fRec1[i] + fZec47[i]) + fSlow87 * fZec47[i]);
			}
			/* Vectorizable loop 135 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec69[i] = ((iSlow81) ? fZec67[i] : fSlow88 * (fRec47[i] + fZec68[i]) + fSlow87 * fZec68[i]);
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = ((iSlow0) ? 0.0f : fZec48[i]);
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec51[i] = int(std::ceil(0.0f - fZec50[i]));
			}
			/* Vectorizable loop 138 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec71[i] = int(std::ceil(fZec50[i]));
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec70[i] = ((iSlow0) ? 0.0f : fZec69[i]);
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow0) ? fZec48[i] : fSlow89 * ((iZec51[i]) ? fZec49[i] : ((iZec51[i]) ? 0.0f : fZec49[i]) * (1.0f - fZec50[i])) + fSlow90 * fZec49[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow0) ? fZec69[i] : fSlow89 * ((iZec71[i]) ? fZec70[i] : ((iZec71[i]) ? 0.0f : fZec70[i]) * (fZec50[i] + 1.0f)) + fSlow90 * fZec70[i]));
			}
		}
		/* Remaining frames */
		if (vindex < count) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = count - vindex;
			/* Vectorizable loop 0 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iVec0_tmp[j0] = iVec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iVec0_perm[j1] = iVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				iRec36_tmp[j8] = iRec36_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec36[i] = iSlow5 * (iRec36[i - 1] + 1);
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				iRec36_perm[j9] = iRec36_tmp[vsize + j9];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec37_tmp[j10] = fRec37_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec37[i] = fSlow7 * std::fmod(fRec37[i - 1] + 1.0f, fSlow6);
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec37_perm[j11] = fRec37_tmp[vsize + j11];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = std::max<int>(0, std::min<int>(iRec36[i], 47999));
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = std::max<int>(0, std::min<int>(int(fRec37[i]), 47999));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec2[i]] = float(input0[i]);
				fZec4[i] = fSlow11 * (fSlow9 * ftbl0[iZec3[i]] + fSlow10 * float(input0[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl2[iZec2[i]] = float(input1[i]);
				fZec52[i] = fSlow11 * (fSlow9 * ftbl2[iZec3[i]] + fSlow10 * float(input1[i]));
			}
			/* Vectorizable loop 7 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fYec0_tmp[j12] = fYec0_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = ((iSlow8) ? 0.0f : fZec4[i]);
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fYec0_perm[j13] = fYec0_tmp[vsize + j13];
			}
			/* Vectorizable loop 8 */
			/* Pre code */
			for (int j94 = 0; j94 < 4; j94 = j94 + 1) {
				fYec3_tmp[j94] = fYec3_perm[j94];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec3[i] = ((iSlow8) ? 0.0f : fZec52[i]);
			}
			/* Post code */
			for (int j95 = 0; j95 < 4; j95 = j95 + 1) {
				fYec3_perm[j95] = fYec3_tmp[vsize + j95];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec39_tmp[j18] = fRec39_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec39[i] = 0.0f - fConst8 * (fConst6 * fRec39[i - 1] - (fYec0[i] + fYec0[i - 1]));
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec39_perm[j19] = fRec39_tmp[vsize + j19];
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec35_tmp[j14] = fRec35_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec35[i] = fConst9 * fYec0[i - 1] + fConst8 * (fConst5 * fYec0[i] - fConst6 * fRec35[i - 1]);
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec35_perm[j15] = fRec35_tmp[vsize + j15];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j96 = 0; j96 < 4; j96 = j96 + 1) {
				fRec79_tmp[j96] = fRec79_perm[j96];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec79[i] = fConst9 * fYec3[i - 1] + fConst8 * (fConst5 * fYec3[i] - fConst6 * fRec79[i - 1]);
			}
			/* Post code */
			for (int j97 = 0; j97 < 4; j97 = j97 + 1) {
				fRec79_perm[j97] = fRec79_tmp[vsize + j97];
			}
			/* Recursive loop 12 */
			/* Pre code */
			for (int j100 = 0; j100 < 4; j100 = j100 + 1) {
				fRec81_tmp[j100] = fRec81_perm[j100];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec81[i] = fConst8 * (fYec3[i] + fYec3[i - 1] - fConst6 * fRec81[i - 1]);
			}
			/* Post code */
			for (int j101 = 0; j101 < 4; j101 = j101 + 1) {
				fRec81_perm[j101] = fRec81_tmp[vsize + j101];
			}
			/* Recursive loop 13 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec38_tmp[j20] = fRec38_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec38[i] = fRec39[i] - fConst14 * (fConst13 * fRec38[i - 2] + fConst12 * fRec38[i - 1]);
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec38_perm[j21] = fRec38_tmp[vsize + j21];
			}
			/* Recursive loop 14 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec34_tmp[j16] = fRec34_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec34[i] = fRec35[i] - fConst14 * (fConst13 * fRec34[i - 2] + fConst12 * fRec34[i - 1]);
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec34_perm[j17] = fRec34_tmp[vsize + j17];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j98 = 0; j98 < 4; j98 = j98 + 1) {
				fRec78_tmp[j98] = fRec78_perm[j98];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec78[i] = fRec79[i] - fConst14 * (fConst13 * fRec78[i - 2] + fConst12 * fRec78[i - 1]);
			}
			/* Post code */
			for (int j99 = 0; j99 < 4; j99 = j99 + 1) {
				fRec78_perm[j99] = fRec78_tmp[vsize + j99];
			}
			/* Recursive loop 16 */
			/* Pre code */
			for (int j102 = 0; j102 < 4; j102 = j102 + 1) {
				fRec80_tmp[j102] = fRec80_perm[j102];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec80[i] = fRec81[i] - fConst14 * (fConst13 * fRec80[i - 2] + fConst12 * fRec80[i - 1]);
			}
			/* Post code */
			for (int j103 = 0; j103 < 4; j103 = j103 + 1) {
				fRec80_perm[j103] = fRec80_tmp[vsize + j103];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iSlow8) ? fZec4[i] : fConst14 * (fConst11 * fRec34[i] + fConst19 * fRec34[i - 1] + fConst11 * fRec34[i - 2] + fSlow19 * (fRec38[i - 2] + fRec38[i] + 2.0f * fRec38[i - 1])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec54[i] = ((iSlow8) ? fZec52[i] : fConst14 * (fConst19 * fRec78[i - 1] + fConst11 * fRec78[i] + fConst11 * fRec78[i - 2] + fSlow19 * (fRec80[i - 2] + fRec80[i] + 2.0f * fRec80[i - 1])));
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec33_tmp[j22] = fRec33_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fConst18 * fRec33[i - 1];
				fRec33[i] = ((iSlow18) ? 0.0f : fZec6[i]) - (fRec33[i - 2] * fSlow17 + fZec5[i]) / fSlow16;
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec33_perm[j23] = fRec33_tmp[vsize + j23];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j104 = 0; j104 < 4; j104 = j104 + 1) {
				fRec77_tmp[j104] = fRec77_perm[j104];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec53[i] = fConst18 * fRec77[i - 1];
				fRec77[i] = ((iSlow18) ? 0.0f : fZec54[i]) - (fSlow17 * fRec77[i - 2] + fZec53[i]) / fSlow16;
			}
			/* Post code */
			for (int j105 = 0; j105 < 4; j105 = j105 + 1) {
				fRec77_perm[j105] = fRec77_tmp[vsize + j105];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = ((iSlow18) ? fZec6[i] : (fZec5[i] + fRec33[i] * fSlow23 + fRec33[i - 2] * fSlow22) / fSlow16);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec55[i] = ((iSlow18) ? fZec54[i] : (fZec53[i] + fRec77[i] * fSlow23 + fSlow22 * fRec77[i - 2]) / fSlow16);
			}
			/* Vectorizable loop 23 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fYec1_tmp[j24] = fYec1_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = ((iSlow20) ? 0.0f : fZec7[i]);
			}
			/* Post code */
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fYec1_perm[j25] = fYec1_tmp[vsize + j25];
			}
			/* Vectorizable loop 24 */
			/* Pre code */
			for (int j106 = 0; j106 < 4; j106 = j106 + 1) {
				fYec4_tmp[j106] = fYec4_perm[j106];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec4[i] = ((iSlow20) ? 0.0f : fZec55[i]);
			}
			/* Post code */
			for (int j107 = 0; j107 < 4; j107 = j107 + 1) {
				fYec4_perm[j107] = fYec4_tmp[vsize + j107];
			}
			/* Recursive loop 25 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec32_tmp[j26] = fRec32_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec32[i] = 0.0f - fConst24 * (fConst22 * fRec32[i - 1] - (fYec1[i] + fYec1[i - 1]));
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec32_perm[j27] = fRec32_tmp[vsize + j27];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec41_tmp[j30] = fRec41_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec41[i] = fConst30 * fYec1[i - 1] - fConst24 * (fConst22 * fRec41[i - 1] - fConst21 * fYec1[i]);
			}
			/* Post code */
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec41_perm[j31] = fRec41_tmp[vsize + j31];
			}
			/* Recursive loop 27 */
			/* Pre code */
			for (int j108 = 0; j108 < 4; j108 = j108 + 1) {
				fRec76_tmp[j108] = fRec76_perm[j108];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec76[i] = fConst24 * (fYec4[i] + fYec4[i - 1] - fConst22 * fRec76[i - 1]);
			}
			/* Post code */
			for (int j109 = 0; j109 < 4; j109 = j109 + 1) {
				fRec76_perm[j109] = fRec76_tmp[vsize + j109];
			}
			/* Recursive loop 28 */
			/* Pre code */
			for (int j112 = 0; j112 < 4; j112 = j112 + 1) {
				fRec83_tmp[j112] = fRec83_perm[j112];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec83[i] = fConst30 * fYec4[i - 1] - fConst24 * (fConst22 * fRec83[i - 1] - fConst21 * fYec4[i]);
			}
			/* Post code */
			for (int j113 = 0; j113 < 4; j113 = j113 + 1) {
				fRec83_perm[j113] = fRec83_tmp[vsize + j113];
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec40_tmp[j32] = fRec40_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec40[i] = fRec41[i] - fConst29 * (fConst28 * fRec40[i - 2] + fConst27 * fRec40[i - 1]);
			}
			/* Post code */
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec40_perm[j33] = fRec40_tmp[vsize + j33];
			}
			/* Recursive loop 30 */
			/* Pre code */
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec31_tmp[j28] = fRec31_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec31[i] = fRec32[i] - fConst29 * (fConst28 * fRec31[i - 2] + fConst27 * fRec31[i - 1]);
			}
			/* Post code */
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec31_perm[j29] = fRec31_tmp[vsize + j29];
			}
			/* Recursive loop 31 */
			/* Pre code */
			for (int j114 = 0; j114 < 4; j114 = j114 + 1) {
				fRec82_tmp[j114] = fRec82_perm[j114];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec82[i] = fRec83[i] - fConst29 * (fConst28 * fRec82[i - 2] + fConst27 * fRec82[i - 1]);
			}
			/* Post code */
			for (int j115 = 0; j115 < 4; j115 = j115 + 1) {
				fRec82_perm[j115] = fRec82_tmp[vsize + j115];
			}
			/* Recursive loop 32 */
			/* Pre code */
			for (int j110 = 0; j110 < 4; j110 = j110 + 1) {
				fRec75_tmp[j110] = fRec75_perm[j110];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec75[i] = fRec76[i] - fConst29 * (fConst28 * fRec75[i - 2] + fConst27 * fRec75[i - 1]);
			}
			/* Post code */
			for (int j111 = 0; j111 < 4; j111 = j111 + 1) {
				fRec75_perm[j111] = fRec75_tmp[vsize + j111];
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = ((iSlow20) ? fZec7[i] : fConst29 * (fRec31[i - 2] + fRec31[i] + 2.0f * fRec31[i - 1] + fSlow32 * (fConst32 * fRec40[i - 1] + fConst26 * fRec40[i] + fConst26 * fRec40[i - 2])));
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec56[i] = ((iSlow20) ? fZec55[i] : fConst29 * (fRec75[i - 2] + fRec75[i] + 2.0f * fRec75[i - 1] + fSlow32 * (fConst26 * fRec82[i] + fConst32 * fRec82[i - 1] + fConst26 * fRec82[i - 2])));
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec30_tmp[j34] = fRec30_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec30[i] = ((iSlow31) ? 0.0f : fZec8[i]) - fSlow30 * (fSlow29 * fRec30[i - 2] + fSlow27 * fRec30[i - 1]);
			}
			/* Post code */
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec30_perm[j35] = fRec30_tmp[vsize + j35];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j116 = 0; j116 < 4; j116 = j116 + 1) {
				fRec74_tmp[j116] = fRec74_perm[j116];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec74[i] = ((iSlow31) ? 0.0f : fZec56[i]) - fSlow30 * (fSlow29 * fRec74[i - 2] + fSlow27 * fRec74[i - 1]);
			}
			/* Post code */
			for (int j117 = 0; j117 < 4; j117 = j117 + 1) {
				fRec74_perm[j117] = fRec74_tmp[vsize + j117];
			}
			/* Recursive loop 37 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec29_tmp[j36] = fRec29_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec29[i] = fSlow30 * (fSlow26 * fRec30[i] + fSlow35 * fRec30[i - 1] + fSlow26 * fRec30[i - 2]) - fSlow34 * (fSlow33 * fRec29[i - 2] + fSlow27 * fRec29[i - 1]);
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec29_perm[j37] = fRec29_tmp[vsize + j37];
			}
			/* Recursive loop 38 */
			/* Pre code */
			for (int j118 = 0; j118 < 4; j118 = j118 + 1) {
				fRec73_tmp[j118] = fRec73_perm[j118];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec73[i] = fSlow30 * (fSlow35 * fRec74[i - 1] + fSlow26 * fRec74[i] + fSlow26 * fRec74[i - 2]) - fSlow34 * (fSlow33 * fRec73[i - 2] + fSlow27 * fRec73[i - 1]);
			}
			/* Post code */
			for (int j119 = 0; j119 < 4; j119 = j119 + 1) {
				fRec73_perm[j119] = fRec73_tmp[vsize + j119];
			}
			/* Recursive loop 39 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec28_tmp[j38] = fRec28_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec28[i] = fSlow34 * (fSlow35 * fRec29[i - 1] + fSlow26 * fRec29[i] + fSlow26 * fRec29[i - 2]) - fSlow37 * (fSlow36 * fRec28[i - 2] + fSlow27 * fRec28[i - 1]);
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec28_perm[j39] = fRec28_tmp[vsize + j39];
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j120 = 0; j120 < 4; j120 = j120 + 1) {
				fRec72_tmp[j120] = fRec72_perm[j120];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec72[i] = fSlow34 * (fSlow26 * fRec73[i] + fSlow35 * fRec73[i - 1] + fSlow26 * fRec73[i - 2]) - fSlow37 * (fSlow36 * fRec72[i - 2] + fSlow27 * fRec72[i - 1]);
			}
			/* Post code */
			for (int j121 = 0; j121 < 4; j121 = j121 + 1) {
				fRec72_perm[j121] = fRec72_tmp[vsize + j121];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec27_tmp[j40] = fRec27_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec27[i] = fSlow37 * (fSlow26 * fRec28[i] + fSlow35 * fRec28[i - 1] + fSlow26 * fRec28[i - 2]) - fSlow39 * (fSlow38 * fRec27[i - 2] + fSlow27 * fRec27[i - 1]);
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec27_perm[j41] = fRec27_tmp[vsize + j41];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j122 = 0; j122 < 4; j122 = j122 + 1) {
				fRec71_tmp[j122] = fRec71_perm[j122];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec71[i] = fSlow37 * (fSlow26 * fRec72[i] + fSlow35 * fRec72[i - 1] + fSlow26 * fRec72[i - 2]) - fSlow39 * (fSlow38 * fRec71[i - 2] + fSlow27 * fRec71[i - 1]);
			}
			/* Post code */
			for (int j123 = 0; j123 < 4; j123 = j123 + 1) {
				fRec71_perm[j123] = fRec71_tmp[vsize + j123];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec26_tmp[j42] = fRec26_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = fSlow39 * (fSlow35 * fRec27[i - 1] + fSlow26 * fRec27[i] + fSlow26 * fRec27[i - 2]) - fSlow41 * (fSlow40 * fRec26[i - 2] + fSlow27 * fRec26[i - 1]);
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec26_perm[j43] = fRec26_tmp[vsize + j43];
			}
			/* Recursive loop 44 */
			/* Pre code */
			for (int j124 = 0; j124 < 4; j124 = j124 + 1) {
				fRec70_tmp[j124] = fRec70_perm[j124];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec70[i] = fSlow39 * (fSlow35 * fRec71[i - 1] + fSlow26 * fRec71[i] + fSlow26 * fRec71[i - 2]) - fSlow41 * (fSlow40 * fRec70[i - 2] + fSlow27 * fRec70[i - 1]);
			}
			/* Post code */
			for (int j125 = 0; j125 < 4; j125 = j125 + 1) {
				fRec70_perm[j125] = fRec70_tmp[vsize + j125];
			}
			/* Recursive loop 45 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec25_tmp[j44] = fRec25_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = fSlow41 * (fSlow35 * fRec26[i - 1] + fSlow26 * fRec26[i] + fSlow26 * fRec26[i - 2]) - fSlow43 * (fSlow42 * fRec25[i - 2] + fSlow27 * fRec25[i - 1]);
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec25_perm[j45] = fRec25_tmp[vsize + j45];
			}
			/* Recursive loop 46 */
			/* Pre code */
			for (int j126 = 0; j126 < 4; j126 = j126 + 1) {
				fRec69_tmp[j126] = fRec69_perm[j126];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec69[i] = fSlow41 * (fSlow26 * fRec70[i] + fSlow35 * fRec70[i - 1] + fSlow26 * fRec70[i - 2]) - fSlow43 * (fSlow42 * fRec69[i - 2] + fSlow27 * fRec69[i - 1]);
			}
			/* Post code */
			for (int j127 = 0; j127 < 4; j127 = j127 + 1) {
				fRec69_perm[j127] = fRec69_tmp[vsize + j127];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iSlow31) ? fZec8[i] : fSlow43 * (fSlow26 * fRec25[i] + fSlow35 * fRec25[i - 1] + fSlow26 * fRec25[i - 2]));
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec57[i] = ((iSlow31) ? fZec56[i] : fSlow43 * (fSlow26 * fRec69[i] + fSlow35 * fRec69[i - 1] + fSlow26 * fRec69[i - 2]));
			}
			/* Recursive loop 49 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec24_tmp[j46] = fRec24_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = ((iSlow49) ? 0.0f : fZec9[i]) - fSlow48 * (fSlow47 * fRec24[i - 2] + fSlow45 * fRec24[i - 1]);
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec24_perm[j47] = fRec24_tmp[vsize + j47];
			}
			/* Recursive loop 50 */
			/* Pre code */
			for (int j128 = 0; j128 < 4; j128 = j128 + 1) {
				fRec68_tmp[j128] = fRec68_perm[j128];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec68[i] = ((iSlow49) ? 0.0f : fZec57[i]) - fSlow48 * (fSlow47 * fRec68[i - 2] + fSlow45 * fRec68[i - 1]);
			}
			/* Post code */
			for (int j129 = 0; j129 < 4; j129 = j129 + 1) {
				fRec68_perm[j129] = fRec68_tmp[vsize + j129];
			}
			/* Recursive loop 51 */
			/* Pre code */
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec23_tmp[j48] = fRec23_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = fSlow48 * (fRec24[i - 2] + fRec24[i] + 2.0f * fRec24[i - 1]) - fSlow51 * (fSlow50 * fRec23[i - 2] + fSlow45 * fRec23[i - 1]);
			}
			/* Post code */
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec23_perm[j49] = fRec23_tmp[vsize + j49];
			}
			/* Recursive loop 52 */
			/* Pre code */
			for (int j130 = 0; j130 < 4; j130 = j130 + 1) {
				fRec67_tmp[j130] = fRec67_perm[j130];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec67[i] = fSlow48 * (fRec68[i - 2] + fRec68[i] + 2.0f * fRec68[i - 1]) - fSlow51 * (fSlow50 * fRec67[i - 2] + fSlow45 * fRec67[i - 1]);
			}
			/* Post code */
			for (int j131 = 0; j131 < 4; j131 = j131 + 1) {
				fRec67_perm[j131] = fRec67_tmp[vsize + j131];
			}
			/* Recursive loop 53 */
			/* Pre code */
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec22_tmp[j50] = fRec22_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = fSlow51 * (fRec23[i - 2] + fRec23[i] + 2.0f * fRec23[i - 1]) - fSlow53 * (fSlow52 * fRec22[i - 2] + fSlow45 * fRec22[i - 1]);
			}
			/* Post code */
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec22_perm[j51] = fRec22_tmp[vsize + j51];
			}
			/* Recursive loop 54 */
			/* Pre code */
			for (int j132 = 0; j132 < 4; j132 = j132 + 1) {
				fRec66_tmp[j132] = fRec66_perm[j132];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec66[i] = fSlow51 * (fRec67[i - 2] + fRec67[i] + 2.0f * fRec67[i - 1]) - fSlow53 * (fSlow52 * fRec66[i - 2] + fSlow45 * fRec66[i - 1]);
			}
			/* Post code */
			for (int j133 = 0; j133 < 4; j133 = j133 + 1) {
				fRec66_perm[j133] = fRec66_tmp[vsize + j133];
			}
			/* Recursive loop 55 */
			/* Pre code */
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec21_tmp[j52] = fRec21_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = fSlow53 * (fRec22[i - 2] + fRec22[i] + 2.0f * fRec22[i - 1]) - fSlow55 * (fSlow54 * fRec21[i - 2] + fSlow45 * fRec21[i - 1]);
			}
			/* Post code */
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec21_perm[j53] = fRec21_tmp[vsize + j53];
			}
			/* Recursive loop 56 */
			/* Pre code */
			for (int j134 = 0; j134 < 4; j134 = j134 + 1) {
				fRec65_tmp[j134] = fRec65_perm[j134];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec65[i] = fSlow53 * (fRec66[i - 2] + fRec66[i] + 2.0f * fRec66[i - 1]) - fSlow55 * (fSlow54 * fRec65[i - 2] + fSlow45 * fRec65[i - 1]);
			}
			/* Post code */
			for (int j135 = 0; j135 < 4; j135 = j135 + 1) {
				fRec65_perm[j135] = fRec65_tmp[vsize + j135];
			}
			/* Recursive loop 57 */
			/* Pre code */
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec20_tmp[j54] = fRec20_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = fSlow55 * (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) - fSlow57 * (fSlow56 * fRec20[i - 2] + fSlow45 * fRec20[i - 1]);
			}
			/* Post code */
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec20_perm[j55] = fRec20_tmp[vsize + j55];
			}
			/* Recursive loop 58 */
			/* Pre code */
			for (int j136 = 0; j136 < 4; j136 = j136 + 1) {
				fRec64_tmp[j136] = fRec64_perm[j136];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec64[i] = fSlow55 * (fRec65[i - 2] + fRec65[i] + 2.0f * fRec65[i - 1]) - fSlow57 * (fSlow56 * fRec64[i - 2] + fSlow45 * fRec64[i - 1]);
			}
			/* Post code */
			for (int j137 = 0; j137 < 4; j137 = j137 + 1) {
				fRec64_perm[j137] = fRec64_tmp[vsize + j137];
			}
			/* Recursive loop 59 */
			/* Pre code */
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec19_tmp[j56] = fRec19_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = fSlow57 * (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) - fSlow59 * (fSlow58 * fRec19[i - 2] + fSlow45 * fRec19[i - 1]);
			}
			/* Post code */
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec19_perm[j57] = fRec19_tmp[vsize + j57];
			}
			/* Recursive loop 60 */
			/* Pre code */
			for (int j138 = 0; j138 < 4; j138 = j138 + 1) {
				fRec63_tmp[j138] = fRec63_perm[j138];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec63[i] = fSlow57 * (fRec64[i - 2] + fRec64[i] + 2.0f * fRec64[i - 1]) - fSlow59 * (fSlow58 * fRec63[i - 2] + fSlow45 * fRec63[i - 1]);
			}
			/* Post code */
			for (int j139 = 0; j139 < 4; j139 = j139 + 1) {
				fRec63_perm[j139] = fRec63_tmp[vsize + j139];
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = ((iSlow49) ? fZec9[i] : fSlow59 * (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]));
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec58[i] = ((iSlow49) ? fZec57[i] : fSlow59 * (fRec63[i - 2] + fRec63[i] + 2.0f * fRec63[i - 1]));
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iSlow60) ? 0.0f : fZec10[i]);
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec59[i] = ((iSlow60) ? 0.0f : fZec58[i]);
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 66 */
			/* Pre code */
			fRec18_idx = (fRec18_idx + fRec18_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[(i + fRec18_idx) & 2097151] = fSlow62 * fRec18[(i + fRec18_idx - iSlow61) & 2097151] + fZec11[i];
			}
			/* Post code */
			fRec18_idx_save = vsize;
			/* Recursive loop 67 */
			/* Pre code */
			fRec62_idx = (fRec62_idx + fRec62_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec62[(i + fRec62_idx) & 2097151] = fSlow62 * fRec62[(i + fRec62_idx - iSlow61) & 2097151] + fZec59[i];
			}
			/* Post code */
			fRec62_idx_save = vsize;
			/* Recursive loop 68 */
			/* Pre code */
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec42_tmp[j58] = fRec42_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((iZec0[i] | iSlow63) ? 0.0f : fSlow64 + fRec42[i - 1]);
				fRec42[i] = fZec12[i] - std::floor(fZec12[i]);
			}
			/* Post code */
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec42_perm[j59] = fRec42_tmp[vsize + j59];
			}
			/* Vectorizable loop 69 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = ((iSlow60) ? fZec10[i] : fSlow68 * fRec18[(i + fRec18_idx) & 2097151] + fSlow69 * fZec11[i]);
			}
			/* Vectorizable loop 70 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec60[i] = ((iSlow60) ? fZec58[i] : fSlow68 * fRec62[(i + fRec62_idx) & 2097151] + fSlow69 * fZec59[i]);
			}
			/* Vectorizable loop 71 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow70 * ((iSlow67) ? 0.0f : fZec18[i])));
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = std::tan(fConst31 * (fSlow65 + fSlow66 * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec42[i]), 65535))] + 1.0f)));
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec61[i] = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow70 * ((iSlow67) ? 0.0f : fZec60[i])));
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = ((iSlow67) ? fZec18[i] : fZec19[i] * (1.0f - 0.33333334f * mydsp_faustpower2_f(fZec19[i])));
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = 1.0f / fZec13[i];
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec62[i] = ((iSlow67) ? fZec60[i] : fZec61[i] * (1.0f - 0.33333334f * mydsp_faustpower2_f(fZec61[i])));
			}
			/* Vectorizable loop 77 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = ((iSlow63) ? 0.0f : fZec20[i]);
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - 1.0f / mydsp_faustpower2_f(fZec13[i]);
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = (fZec14[i] + -1.9828898f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = (fZec14[i] + 1.9828898f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 81 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec63[i] = ((iSlow63) ? 0.0f : fZec62[i]);
			}
			/* Vectorizable loop 82 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = (fZec14[i] + 1.847759f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = (fZec14[i] + -1.847759f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 84 */
			/* Pre code */
			for (int j60 = 0; j60 < 4; j60 = j60 + 1) {
				fRec17_tmp[j60] = fRec17_perm[j60];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fZec21[i] - (fRec17[i - 2] * fZec17[i] + 2.0f * fRec17[i - 1] * fZec16[i]) / fZec15[i];
			}
			/* Post code */
			for (int j61 = 0; j61 < 4; j61 = j61 + 1) {
				fRec17_perm[j61] = fRec17_tmp[vsize + j61];
			}
			/* Recursive loop 85 */
			/* Pre code */
			for (int j140 = 0; j140 < 4; j140 = j140 + 1) {
				fRec61_tmp[j140] = fRec61_perm[j140];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec61[i] = fZec63[i] - (fZec17[i] * fRec61[i - 2] + 2.0f * fZec16[i] * fRec61[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j141 = 0; j141 < 4; j141 = j141 + 1) {
				fRec61_perm[j141] = fRec61_tmp[vsize + j141];
			}
			/* Recursive loop 86 */
			/* Pre code */
			for (int j62 = 0; j62 < 4; j62 = j62 + 1) {
				fRec16_tmp[j62] = fRec16_perm[j62];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + fRec17[i] + 2.0f * fRec17[i - 1]) / fZec15[i] - (fRec16[i - 2] * fZec23[i] + 2.0f * fZec16[i] * fRec16[i - 1]) / fZec22[i];
			}
			/* Post code */
			for (int j63 = 0; j63 < 4; j63 = j63 + 1) {
				fRec16_perm[j63] = fRec16_tmp[vsize + j63];
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = (fZec14[i] + 1.5867066f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 88 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = (fZec14[i] + -1.5867066f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 89 */
			/* Pre code */
			for (int j142 = 0; j142 < 4; j142 = j142 + 1) {
				fRec60_tmp[j142] = fRec60_perm[j142];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec60[i] = (fRec61[i - 2] + fRec61[i] + 2.0f * fRec61[i - 1]) / fZec15[i] - (fZec23[i] * fRec60[i - 2] + 2.0f * fZec16[i] * fRec60[i - 1]) / fZec22[i];
			}
			/* Post code */
			for (int j143 = 0; j143 < 4; j143 = j143 + 1) {
				fRec60_perm[j143] = fRec60_tmp[vsize + j143];
			}
			/* Recursive loop 90 */
			/* Pre code */
			for (int j64 = 0; j64 < 4; j64 = j64 + 1) {
				fRec15_tmp[j64] = fRec15_perm[j64];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = (fRec16[i - 2] + fRec16[i] + 2.0f * fRec16[i - 1]) / fZec22[i] - (fRec15[i - 2] * fZec25[i] + 2.0f * fZec16[i] * fRec15[i - 1]) / fZec24[i];
			}
			/* Post code */
			for (int j65 = 0; j65 < 4; j65 = j65 + 1) {
				fRec15_perm[j65] = fRec15_tmp[vsize + j65];
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = (fZec14[i] + 1.2175229f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = (fZec14[i] + -1.2175229f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 93 */
			/* Pre code */
			for (int j144 = 0; j144 < 4; j144 = j144 + 1) {
				fRec59_tmp[j144] = fRec59_perm[j144];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec59[i] = (fRec60[i - 2] + fRec60[i] + 2.0f * fRec60[i - 1]) / fZec22[i] - (fZec25[i] * fRec59[i - 2] + 2.0f * fZec16[i] * fRec59[i - 1]) / fZec24[i];
			}
			/* Post code */
			for (int j145 = 0; j145 < 4; j145 = j145 + 1) {
				fRec59_perm[j145] = fRec59_tmp[vsize + j145];
			}
			/* Recursive loop 94 */
			/* Pre code */
			for (int j66 = 0; j66 < 4; j66 = j66 + 1) {
				fRec14_tmp[j66] = fRec14_perm[j66];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = (fRec15[i - 2] + fRec15[i] + 2.0f * fRec15[i - 1]) / fZec24[i] - (fRec14[i - 2] * fZec27[i] + 2.0f * fZec16[i] * fRec14[i - 1]) / fZec26[i];
			}
			/* Post code */
			for (int j67 = 0; j67 < 4; j67 = j67 + 1) {
				fRec14_perm[j67] = fRec14_tmp[vsize + j67];
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = (fZec14[i] + -0.76536685f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = (fZec14[i] + 0.76536685f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 97 */
			/* Pre code */
			for (int j146 = 0; j146 < 4; j146 = j146 + 1) {
				fRec58_tmp[j146] = fRec58_perm[j146];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec58[i] = (fRec59[i - 2] + fRec59[i] + 2.0f * fRec59[i - 1]) / fZec24[i] - (fZec27[i] * fRec58[i - 2] + 2.0f * fZec16[i] * fRec58[i - 1]) / fZec26[i];
			}
			/* Post code */
			for (int j147 = 0; j147 < 4; j147 = j147 + 1) {
				fRec58_perm[j147] = fRec58_tmp[vsize + j147];
			}
			/* Recursive loop 98 */
			/* Pre code */
			for (int j68 = 0; j68 < 4; j68 = j68 + 1) {
				fRec13_tmp[j68] = fRec13_perm[j68];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = (fRec14[i - 2] + fRec14[i] + 2.0f * fRec14[i - 1]) / fZec26[i] - (fRec13[i - 2] * fZec29[i] + 2.0f * fZec16[i] * fRec13[i - 1]) / fZec28[i];
			}
			/* Post code */
			for (int j69 = 0; j69 < 4; j69 = j69 + 1) {
				fRec13_perm[j69] = fRec13_tmp[vsize + j69];
			}
			/* Vectorizable loop 99 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = (fZec14[i] + -0.26105237f) / fZec13[i] + 1.0f;
			}
			/* Vectorizable loop 100 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec14[i] + 0.26105237f) / fZec13[i] + 1.0f;
			}
			/* Recursive loop 101 */
			/* Pre code */
			for (int j148 = 0; j148 < 4; j148 = j148 + 1) {
				fRec57_tmp[j148] = fRec57_perm[j148];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec57[i] = (fRec58[i - 2] + fRec58[i] + 2.0f * fRec58[i - 1]) / fZec26[i] - (fZec29[i] * fRec57[i - 2] + 2.0f * fZec16[i] * fRec57[i - 1]) / fZec28[i];
			}
			/* Post code */
			for (int j149 = 0; j149 < 4; j149 = j149 + 1) {
				fRec57_perm[j149] = fRec57_tmp[vsize + j149];
			}
			/* Recursive loop 102 */
			/* Pre code */
			for (int j70 = 0; j70 < 4; j70 = j70 + 1) {
				fRec12_tmp[j70] = fRec12_perm[j70];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = (fRec13[i - 2] + fRec13[i] + 2.0f * fRec13[i - 1]) / fZec28[i] - (fRec12[i - 2] * fZec31[i] + 2.0f * fZec16[i] * fRec12[i - 1]) / fZec30[i];
			}
			/* Post code */
			for (int j71 = 0; j71 < 4; j71 = j71 + 1) {
				fRec12_perm[j71] = fRec12_tmp[vsize + j71];
			}
			/* Recursive loop 103 */
			/* Pre code */
			for (int j74 = 0; j74 < 4; j74 = j74 + 1) {
				fRec46_tmp[j74] = fRec46_perm[j74];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = ((iZec0[i] | iSlow73) ? 0.0f : fSlow74 + fRec46[i - 1]);
				fRec46[i] = fZec33[i] - std::floor(fZec33[i]);
			}
			/* Post code */
			for (int j75 = 0; j75 < 4; j75 = j75 + 1) {
				fRec46_perm[j75] = fRec46_tmp[vsize + j75];
			}
			/* Recursive loop 104 */
			/* Pre code */
			for (int j72 = 0; j72 < 4; j72 = j72 + 1) {
				fRec44_tmp[j72] = fRec44_perm[j72];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = ((iZec0[i] | iSlow71) ? 0.0f : fSlow72 + fRec44[i - 1]);
				fRec44[i] = fZec32[i] - std::floor(fZec32[i]);
			}
			/* Post code */
			for (int j73 = 0; j73 < 4; j73 = j73 + 1) {
				fRec44_perm[j73] = fRec44_tmp[vsize + j73];
			}
			/* Recursive loop 105 */
			/* Pre code */
			for (int j150 = 0; j150 < 4; j150 = j150 + 1) {
				fRec56_tmp[j150] = fRec56_perm[j150];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec56[i] = (fRec57[i - 2] + fRec57[i] + 2.0f * fRec57[i - 1]) / fZec28[i] - (fZec31[i] * fRec56[i - 2] + 2.0f * fZec16[i] * fRec56[i - 1]) / fZec30[i];
			}
			/* Post code */
			for (int j151 = 0; j151 < 4; j151 = j151 + 1) {
				fRec56_perm[j151] = fRec56_tmp[vsize + j151];
			}
			/* Vectorizable loop 106 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = fSlow77 + fSlow76 * std::ceil(0.0f - (fRec44[i] - fSlow75));
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = ((iSlow63) ? fZec20[i] : fSlow78 * ((fRec12[i - 2] + fRec12[i] + 2.0f * fRec12[i - 1]) / fZec30[i]) + fSlow79 * fZec21[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = 3e+01f * (ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec46[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec64[i] = ((iSlow63) ? fZec62[i] : fSlow78 * ((fRec56[i - 2] + fRec56[i] + 2.0f * fRec56[i - 1]) / fZec30[i]) + fSlow79 * fZec63[i]);
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = ((iSlow71) ? fZec35[i] : ((iSlow71) ? 0.0f : fZec35[i]) * fZec34[i]);
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec41[i] = std::floor(fZec37[i]);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec39[i] = int(fZec37[i]);
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = fZec37[i] + 1.0f;
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec65[i] = ((iSlow71) ? fZec64[i] : fZec34[i] * ((iSlow71) ? 0.0f : fZec64[i]));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec44[i] = int(std::min<float>(fZec38[i], float(std::max<int>(0, iZec39[i]))));
			}
			/* Vectorizable loop 116 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec43[i] = fZec41[i] + (1.0f - fZec37[i]);
			}
			/* Vectorizable loop 117 */
			/* Pre code */
			fYec2_idx = (fYec2_idx + fYec2_idx_save) & 127;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec2[(i + fYec2_idx) & 127] = ((iSlow73) ? 0.0f : fZec36[i]);
			}
			/* Post code */
			fYec2_idx_save = vsize;
			/* Vectorizable loop 118 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec40[i] = int(std::min<float>(fZec38[i], float(std::max<int>(0, iZec39[i] + 1))));
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec42[i] = fZec37[i] - fZec41[i];
			}
			/* Vectorizable loop 120 */
			/* Pre code */
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 127;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec5[(i + fYec5_idx) & 127] = ((iSlow73) ? 0.0f : fZec65[i]);
			}
			/* Post code */
			fYec5_idx_save = vsize;
			/* Recursive loop 121 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec45[i] = 0.0f - (fYec2[(i + fYec2_idx - iZec44[i]) & 127] * fZec43[i] + fZec42[i] * fYec2[(i + fYec2_idx - iZec40[i]) & 127]);
			}
			/* Recursive loop 122 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec84[i] = 0.0f - (fZec43[i] * fYec5[(i + fYec5_idx - iZec44[i]) & 127] + fZec42[i] * fYec5[(i + fYec5_idx - iZec40[i]) & 127]);
			}
			/* Recursive loop 123 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec9_tmp[j4] = fRec9_perm[j4];
			}
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec10_tmp[j6] = fRec10_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = fSlow4 * fRec10[i - 1] + fSlow3 * fRec9[i - 1];
				fRec10[i] = float(iZec0[i]) + fSlow3 * fRec10[i - 1] - fSlow4 * fRec9[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec9_perm[j5] = fRec9_tmp[vsize + j5];
			}
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec10_perm[j7] = fRec10_tmp[vsize + j7];
			}
			/* Vectorizable loop 124 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec46[i] = ((iSlow73) ? fZec36[i] : fSlow85 * (fYec2[(i + fYec2_idx) & 127] + fSlow84 * fRec45[i]) + fSlow83 * fYec2[(i + fYec2_idx) & 127]);
			}
			/* Vectorizable loop 125 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec67[i] = ((iSlow73) ? fZec65[i] : fSlow85 * (fYec5[(i + fYec5_idx) & 127] + fSlow84 * fRec84[i]) + fSlow83 * fYec5[(i + fYec5_idx) & 127]);
			}
			/* Vectorizable loop 126 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = std::cos(fConst1 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Vectorizable loop 127 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = ((iSlow81) ? 0.0f : fZec46[i]);
			}
			/* Vectorizable loop 128 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec66[i] = std::cos(fConst1 * (14137.167f * (1.0f - fRec10[i]) + 3141.5928f));
			}
			/* Vectorizable loop 129 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec68[i] = ((iSlow81) ? 0.0f : fZec67[i]);
			}
			/* Recursive loop 130 */
			/* Pre code */
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((iZec0[i] | iSlow0) ? 0.0f : fSlow1 + fRec0[i - 1]);
				fRec0[i] = fZec1[i] - std::floor(fZec1[i]);
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Recursive loop 131 */
			/* Pre code */
			for (int j76 = 0; j76 < 4; j76 = j76 + 1) {
				fRec11_tmp[j76] = fRec11_perm[j76];
			}
			for (int j78 = 0; j78 < 4; j78 = j78 + 1) {
				fRec8_tmp[j78] = fRec8_perm[j78];
			}
			for (int j80 = 0; j80 < 4; j80 = j80 + 1) {
				fRec7_tmp[j80] = fRec7_perm[j80];
			}
			for (int j82 = 0; j82 < 4; j82 = j82 + 1) {
				fRec6_tmp[j82] = fRec6_perm[j82];
			}
			for (int j84 = 0; j84 < 4; j84 = j84 + 1) {
				fRec5_tmp[j84] = fRec5_perm[j84];
			}
			for (int j86 = 0; j86 < 4; j86 = j86 + 1) {
				fRec4_tmp[j86] = fRec4_perm[j86];
			}
			for (int j88 = 0; j88 < 4; j88 = j88 + 1) {
				fRec3_tmp[j88] = fRec3_perm[j88];
			}
			for (int j90 = 0; j90 < 4; j90 = j90 + 1) {
				fRec2_tmp[j90] = fRec2_perm[j90];
			}
			for (int j92 = 0; j92 < 4; j92 = j92 + 1) {
				fRec1_tmp[j92] = fRec1_perm[j92];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = fZec47[i] + fSlow80 * fRec1[i - 1] - (fConst36 * fRec11[i - 1] * fZec45[i] + fConst35 * fRec11[i - 2]);
				fRec8[i] = fConst36 * fZec45[i] * (fRec11[i - 1] - fRec8[i - 1]) + fRec11[i - 2] + fConst35 * (fRec11[i] - fRec8[i - 2]);
				fRec7[i] = fRec8[i - 2] + fConst36 * fZec45[i] * (fRec8[i - 1] - fRec7[i - 1]) - fConst35 * (fRec7[i - 2] - fRec8[i]);
				fRec6[i] = fRec7[i - 2] + fConst35 * (fRec7[i] - fRec6[i - 2]) - fConst36 * fZec45[i] * (fRec6[i - 1] - fRec7[i - 1]);
				fRec5[i] = fRec6[i - 2] + fConst36 * fZec45[i] * (fRec6[i - 1] - fRec5[i - 1]) - fConst35 * (fRec5[i - 2] - fRec6[i]);
				fRec4[i] = fRec5[i - 2] - (fConst36 * fZec45[i] * (fRec4[i - 1] - fRec5[i - 1]) + fConst35 * (fRec4[i - 2] - fRec5[i]));
				fRec3[i] = fRec4[i - 2] + fConst36 * fZec45[i] * (fRec4[i - 1] - fRec3[i - 1]) - fConst35 * (fRec3[i - 2] - fRec4[i]);
				fRec2[i] = fRec3[i - 2] + fConst36 * fZec45[i] * (fRec3[i - 1] - fRec2[i - 1]) - fConst35 * (fRec2[i - 2] - fRec3[i]);
				fRec1[i] = fRec2[i - 2] + fConst36 * fZec45[i] * fRec2[i - 1] + fConst35 * fRec2[i];
			}
			/* Post code */
			for (int j77 = 0; j77 < 4; j77 = j77 + 1) {
				fRec11_perm[j77] = fRec11_tmp[vsize + j77];
			}
			for (int j79 = 0; j79 < 4; j79 = j79 + 1) {
				fRec8_perm[j79] = fRec8_tmp[vsize + j79];
			}
			for (int j81 = 0; j81 < 4; j81 = j81 + 1) {
				fRec7_perm[j81] = fRec7_tmp[vsize + j81];
			}
			for (int j83 = 0; j83 < 4; j83 = j83 + 1) {
				fRec6_perm[j83] = fRec6_tmp[vsize + j83];
			}
			for (int j85 = 0; j85 < 4; j85 = j85 + 1) {
				fRec5_perm[j85] = fRec5_tmp[vsize + j85];
			}
			for (int j87 = 0; j87 < 4; j87 = j87 + 1) {
				fRec4_perm[j87] = fRec4_tmp[vsize + j87];
			}
			for (int j89 = 0; j89 < 4; j89 = j89 + 1) {
				fRec3_perm[j89] = fRec3_tmp[vsize + j89];
			}
			for (int j91 = 0; j91 < 4; j91 = j91 + 1) {
				fRec2_perm[j91] = fRec2_tmp[vsize + j91];
			}
			for (int j93 = 0; j93 < 4; j93 = j93 + 1) {
				fRec1_perm[j93] = fRec1_tmp[vsize + j93];
			}
			/* Recursive loop 132 */
			/* Pre code */
			for (int j152 = 0; j152 < 4; j152 = j152 + 1) {
				fRec55_tmp[j152] = fRec55_perm[j152];
			}
			for (int j154 = 0; j154 < 4; j154 = j154 + 1) {
				fRec54_tmp[j154] = fRec54_perm[j154];
			}
			for (int j156 = 0; j156 < 4; j156 = j156 + 1) {
				fRec53_tmp[j156] = fRec53_perm[j156];
			}
			for (int j158 = 0; j158 < 4; j158 = j158 + 1) {
				fRec52_tmp[j158] = fRec52_perm[j158];
			}
			for (int j160 = 0; j160 < 4; j160 = j160 + 1) {
				fRec51_tmp[j160] = fRec51_perm[j160];
			}
			for (int j162 = 0; j162 < 4; j162 = j162 + 1) {
				fRec50_tmp[j162] = fRec50_perm[j162];
			}
			for (int j164 = 0; j164 < 4; j164 = j164 + 1) {
				fRec49_tmp[j164] = fRec49_perm[j164];
			}
			for (int j166 = 0; j166 < 4; j166 = j166 + 1) {
				fRec48_tmp[j166] = fRec48_perm[j166];
			}
			for (int j168 = 0; j168 < 4; j168 = j168 + 1) {
				fRec47_tmp[j168] = fRec47_perm[j168];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec55[i] = fZec68[i] + fSlow80 * fRec47[i - 1] - (fConst36 * fRec55[i - 1] * fZec66[i] + fConst35 * fRec55[i - 2]);
				fRec54[i] = fRec55[i - 2] + fConst36 * fZec66[i] * (fRec55[i - 1] - fRec54[i - 1]) - fConst35 * (fRec54[i - 2] - fRec55[i]);
				fRec53[i] = fConst36 * fZec66[i] * (fRec54[i - 1] - fRec53[i - 1]) + fRec54[i - 2] + fConst35 * (fRec54[i] - fRec53[i - 2]);
				fRec52[i] = fConst36 * fZec66[i] * (fRec53[i - 1] - fRec52[i - 1]) + fRec53[i - 2] + fConst35 * (fRec53[i] - fRec52[i - 2]);
				fRec51[i] = fRec52[i - 2] - (fConst36 * fZec66[i] * (fRec51[i - 1] - fRec52[i - 1]) + fConst35 * (fRec51[i - 2] - fRec52[i]));
				fRec50[i] = fConst35 * (fRec51[i] - fRec50[i - 2]) + fRec51[i - 2] + fConst36 * fZec66[i] * (fRec51[i - 1] - fRec50[i - 1]);
				fRec49[i] = fConst35 * (fRec50[i] - fRec49[i - 2]) + fRec50[i - 2] + fConst36 * fZec66[i] * (fRec50[i - 1] - fRec49[i - 1]);
				fRec48[i] = fRec49[i - 2] + fConst35 * (fRec49[i] - fRec48[i - 2]) - fConst36 * fZec66[i] * (fRec48[i - 1] - fRec49[i - 1]);
				fRec47[i] = fConst35 * fRec48[i] + fConst36 * fZec66[i] * fRec48[i - 1] + fRec48[i - 2];
			}
			/* Post code */
			for (int j153 = 0; j153 < 4; j153 = j153 + 1) {
				fRec55_perm[j153] = fRec55_tmp[vsize + j153];
			}
			for (int j155 = 0; j155 < 4; j155 = j155 + 1) {
				fRec54_perm[j155] = fRec54_tmp[vsize + j155];
			}
			for (int j157 = 0; j157 < 4; j157 = j157 + 1) {
				fRec53_perm[j157] = fRec53_tmp[vsize + j157];
			}
			for (int j159 = 0; j159 < 4; j159 = j159 + 1) {
				fRec52_perm[j159] = fRec52_tmp[vsize + j159];
			}
			for (int j161 = 0; j161 < 4; j161 = j161 + 1) {
				fRec51_perm[j161] = fRec51_tmp[vsize + j161];
			}
			for (int j163 = 0; j163 < 4; j163 = j163 + 1) {
				fRec50_perm[j163] = fRec50_tmp[vsize + j163];
			}
			for (int j165 = 0; j165 < 4; j165 = j165 + 1) {
				fRec49_perm[j165] = fRec49_tmp[vsize + j165];
			}
			for (int j167 = 0; j167 < 4; j167 = j167 + 1) {
				fRec48_perm[j167] = fRec48_tmp[vsize + j167];
			}
			for (int j169 = 0; j169 < 4; j169 = j169 + 1) {
				fRec47_perm[j169] = fRec47_tmp[vsize + j169];
			}
			/* Vectorizable loop 133 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec50[i] = fSlow91 * ftbl1mydspSIG1[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec48[i] = ((iSlow81) ? fZec46[i] : fSlow88 * (fRec1[i] + fZec47[i]) + fSlow87 * fZec47[i]);
			}
			/* Vectorizable loop 135 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec69[i] = ((iSlow81) ? fZec67[i] : fSlow88 * (fRec47[i] + fZec68[i]) + fSlow87 * fZec68[i]);
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = ((iSlow0) ? 0.0f : fZec48[i]);
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec51[i] = int(std::ceil(0.0f - fZec50[i]));
			}
			/* Vectorizable loop 138 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec71[i] = int(std::ceil(fZec50[i]));
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec70[i] = ((iSlow0) ? 0.0f : fZec69[i]);
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow0) ? fZec48[i] : fSlow89 * ((iZec51[i]) ? fZec49[i] : ((iZec51[i]) ? 0.0f : fZec49[i]) * (1.0f - fZec50[i])) + fSlow90 * fZec49[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow0) ? fZec69[i] : fSlow89 * ((iZec71[i]) ? fZec70[i] : ((iZec71[i]) ? 0.0f : fZec70[i]) * (fZec50[i] + 1.0f)) + fSlow90 * fZec70[i]));
			}
		}
	}

};

#endif
