
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
	
	int iVec1[2];
	int iRec1[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec1[l3] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[0] = 1;
			iRec1[0] = (iVec1[1] + iRec1[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec1[0]));
			iVec1[1] = iVec1[0];
			iRec1[1] = iRec1[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class Faust_engine {
public:
	//---------fader&trim---------//
	float volume_limit = 0.7f;
	float trim = 1.0f;
	float fader = 1.0f;
	void set_trim(float value) { trim = value; }
	void set_fader(float value) { fader = value; };
	//----------switches----------//
	int lo_fi_sw = 1; void filter_low_sw(bool turn_on) { turn_on ? lo_fi_sw = 0 : lo_fi_sw = 1; }
	int hi_fi_sw = 1; void filter_high_sw(bool turn_on) { turn_on ? hi_fi_sw = 0 : hi_fi_sw = 1; }
	int eq_mid_sw = 1; void EQ_mid_sw(bool turn_on) { turn_on ? eq_mid_sw = 0 : eq_mid_sw = 1; }
	int eq_low_sw = 1; void EQ_low_sw(bool turn_on) { turn_on ? eq_low_sw = 0 : eq_low_sw = 1; }
	int eq_high_sw = 1; void EQ_high_sw(bool turn_on) { turn_on ? eq_high_sw = 0 : eq_high_sw = 1; }
	int echo_sw = 1; void _echo_sw(bool turn_on) { turn_on ? echo_sw = 0 : echo_sw = 1; }
	int l_f_s_sw = 1; void low_siren_sw(bool turn_on) { turn_on ? l_f_s_sw = 0 : l_f_s_sw = 1; }
	int phaser_sw = 1; void _phaser_sw(bool turn_on) { turn_on ? phaser_sw = 0 : phaser_sw = 1; }
	int flanger_sw = 1; void _flanger_sw(bool turn_on) { turn_on ? flanger_sw = 0 : flanger_sw = 1; }
	int trance_sw = 1; void _trance_sw(bool turn_on) { turn_on ? trance_sw = 0 : trance_sw = 1; }
	int panner_sw = 1; void _panner_sw(bool turn_on) { turn_on ? panner_sw = 0 : panner_sw = 1; }
	int distortion_sw = 1; void _distortion_sw(bool turn_on) { turn_on ? distortion_sw = 0 : distortion_sw = 1; }

	//--------values------------//

	int low_freq_value = 500; void set_filter_low_freq_value(int value) { low_freq_value = value; }
	int high_freq_value = 500; void set_filter_high_freq_value(int value) { high_freq_value = value; }
	float eq_mid_v = 0; void set_eq_mid_value(float value) { eq_mid_v = value; }
	float eq_low_v = 0; void set_eq_low_value(float value) { eq_low_v = value; }
	float eq_hi_v = 0; void set_eq_high_value(float value) { eq_hi_v = value; }
	float echo_dur = 0.5f; void set_echo_dur_value(float value) { echo_dur = 1.0f / value; }
	float echo_feedback_var = 0.1f; void set_echo_feedback_value(float value) { echo_feedback_var = value; }
	float l_f_s_bps = 1.0f; void set_l_f_s_bps_value(float bps) { l_f_s_bps = bps; }
	float flanger_bps = 1.0f; void set_flanger_bps_value(float bps) { flanger_bps = bps; }
	float flanger_gain = 1.0f; void set_flanger_gain_value(float value) { value > 1.0f ? flanger_gain = 1.0f : (value < 0 ? flanger_gain = 0 : flanger_gain = value); }
	float phaser_bps = 1.0f; void set_phaser_bps_value(float bps) { phaser_bps = bps; }
	float phaser_gain = 1.0f; void set_phaser_gain_value(float value) { value > 1.0f ? phaser_gain = 1.0f : (value < -1.0 ? phaser_gain = -1.0f : phaser_gain = value); }
	float trance_bps = 1.0f; void set_trance_bps(float bps) { trance_bps = bps; }
	float panner_bps = 1.0f; void set_panner_bps(float bps) { panner_bps = bps; }
	float distortion_gain = 1.0f; void set_distortion_gain(float value) { value > 1.0f ? distortion_gain = 1.0f : (value < 0 ? distortion_gain = 0.0f : distortion_gain = value); }
	float trance_gain = 1.0f; void set_trance_gain(float value) { value > 1.0f ? trance_gain = 1.0f : (value < 0 ? trance_gain = 0.0f : trance_gain = value); }
	float l_f_s_gain_min_freq = 1.0f; void set_l_f_s_gain_min_freq(int value) { value < 0 ? l_f_s_gain_min_freq = 0 : l_f_s_gain_min_freq = float(value); }
	float panner_gain = 1.0f; void set_panner_gain(float value) { value > 1.0f ? panner_gain = 1.0f : (value < 0 ? panner_gain = 0.0f : panner_gain = value); }
	//-------value_getter------//

 private:
	
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0[2];
	float fRec2[2];
	float fConst2;
	float fRec9[2];
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fConst12;
	float fConst13;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fVec2[2];
	float fConst21;
	float fConst23;
	float fRec28[2];
	float fRec27[3];
	float fConst24;
	float fRec30[2];
	float fRec29[3];
	float fConst25;
	float fRec26[3];
	float fVec3[2];
	float fConst26;
	float fConst28;
	float fConst29;
	float fRec25[2];
	float fRec24[3];
	float fConst30;
	float fRec32[2];
	float fRec31[3];
	float fRec23[3];
	float fRec22[3];
	float fRec21[3];
	float fRec20[3];
	float fRec19[3];
	float fRec18[3];
	float fRec17[3];
	float fRec16[3];
	float fRec15[3];
	float fRec14[3];
	float fRec13[3];
	float fRec12[3];
	int IOTA0;
	float fConst31;
	float fRec11[2097152];
	float fVec4[64];
	float fRec33[2];
	float fConst33;
	float fConst34;
	float fRec43[2];
	float fRec44[2];
	float fConst35;
	float fRec42[3];
	float fRec41[3];
	float fRec40[3];
	float fRec39[3];
	float fRec38[3];
	float fRec37[3];
	float fRec36[3];
	float fRec35[3];
	float fRec34[2];
	float fRec8[3];
	float fRec7[3];
	float fRec6[3];
	float fRec5[3];
	float fRec4[3];
	float fRec3[3];
	float fVec5[2];
	float fRec69[2];
	float fRec68[3];
	float fRec71[2];
	float fRec70[3];
	float fRec67[3];
	float fVec6[2];
	float fRec66[2];
	float fRec65[3];
	float fRec73[2];
	float fRec72[3];
	float fRec64[3];
	float fRec63[3];
	float fRec62[3];
	float fRec61[3];
	float fRec60[3];
	float fRec59[3];
	float fRec58[3];
	float fRec57[3];
	float fRec56[3];
	float fRec55[3];
	float fRec54[3];
	float fRec53[3];
	float fRec52[2097152];
	float fVec7[64];
	float fRec82[3];
	float fRec81[3];
	float fRec80[3];
	float fRec79[3];
	float fRec78[3];
	float fRec77[3];
	float fRec76[3];
	float fRec75[3];
	float fRec74[2];
	float fRec50[3];
	float fRec49[3];
	float fRec48[3];
	float fRec47[3];
	float fRec46[3];
	float fRec45[3];
	
 public:
	 Faust_engine() {}


	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 3.1415927f / fConst0;
		float fConst3 = std::tan(15707.963f / fConst0);
		float fConst4 = mydsp_faustpower2_f(fConst3);
		fConst5 = 1.0f / fConst4;
		fConst6 = 2.0f * (1.0f - fConst5);
		fConst7 = 1.0f / fConst3;
		fConst8 = (fConst7 + -1.0f) / fConst3 + 1.0f;
		fConst9 = 1.0f / ((fConst7 + 1.0f) / fConst3 + 1.0f);
		fConst10 = 7068.5835f / (fConst0 * std::sin(17278.76f / fConst0));
		float fConst11 = std::tan(8639.38f / fConst0);
		fConst12 = 1.0f / fConst11;
		fConst13 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fConst11));
		float fConst14 = std::tan(1570.7964f / fConst0);
		float fConst15 = mydsp_faustpower2_f(fConst14);
		fConst16 = 1.0f / fConst15;
		fConst17 = 2.0f * (1.0f - fConst16);
		fConst18 = 1.0f / fConst14;
		fConst19 = (fConst18 + -1.0f) / fConst14 + 1.0f;
		fConst20 = 1.0f / ((fConst18 + 1.0f) / fConst14 + 1.0f);
		fConst21 = 1.0f - fConst18;
		float fConst22 = fConst18 + 1.0f;
		fConst23 = 1.0f / fConst22;
		fConst24 = 0.0f - 1.0f / (fConst14 * fConst22);
		fConst25 = 0.0f - 2.0f / fConst15;
		fConst26 = 1.0f - fConst7;
		float fConst27 = fConst7 + 1.0f;
		fConst28 = 1.0f / fConst27;
		fConst29 = 0.0f - 1.0f / (fConst3 * fConst27);
		fConst30 = 0.0f - 2.0f / fConst4;
		fConst31 = 1e+01f * fConst0;
		float fConst32 = std::exp(0.0f - 14137.167f / fConst0);
		fConst33 = mydsp_faustpower2_f(fConst32);
		fConst34 = 6.2831855f / fConst0;
		fConst35 = 0.0f - 2.0f * fConst32;
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec9[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec2[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec28[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec27[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec30[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec29[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = l11 + 1) {
			fRec26[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fVec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec25[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec24[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec32[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec31[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = l17 + 1) {
			fRec23[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec22[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec20[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec19[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec18[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = l23 + 1) {
			fRec17[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec16[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec15[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec14[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec13[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fRec12[l28] = 0.0f;
		}
		IOTA0 = 0;
		for (int l29 = 0; l29 < 2097152; l29 = l29 + 1) {
			fRec11[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 64; l30 = l30 + 1) {
			fVec4[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec33[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec43[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec44[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec42[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = l35 + 1) {
			fRec41[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec40[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = l37 + 1) {
			fRec39[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec38[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = l39 + 1) {
			fRec37[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec36[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = l41 + 1) {
			fRec35[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec34[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = l43 + 1) {
			fRec8[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec7[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = l45 + 1) {
			fRec6[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec5[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = l47 + 1) {
			fRec4[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec3[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fVec5[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec69[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = l51 + 1) {
			fRec68[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fRec71[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = l53 + 1) {
			fRec70[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec67[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fVec6[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec66[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3; l57 = l57 + 1) {
			fRec65[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec73[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3; l59 = l59 + 1) {
			fRec72[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec64[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3; l61 = l61 + 1) {
			fRec63[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec62[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3; l63 = l63 + 1) {
			fRec61[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 3; l64 = l64 + 1) {
			fRec60[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 3; l65 = l65 + 1) {
			fRec59[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 3; l66 = l66 + 1) {
			fRec58[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 3; l67 = l67 + 1) {
			fRec57[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 3; l68 = l68 + 1) {
			fRec56[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 3; l69 = l69 + 1) {
			fRec55[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 3; l70 = l70 + 1) {
			fRec54[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec53[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2097152; l72 = l72 + 1) {
			fRec52[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 64; l73 = l73 + 1) {
			fVec7[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 3; l74 = l74 + 1) {
			fRec82[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec81[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fRec80[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 3; l77 = l77 + 1) {
			fRec79[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 3; l78 = l78 + 1) {
			fRec78[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 3; l79 = l79 + 1) {
			fRec77[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 3; l80 = l80 + 1) {
			fRec76[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 3; l81 = l81 + 1) {
			fRec75[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec74[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 3; l83 = l83 + 1) {
			fRec50[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 3; l84 = l84 + 1) {
			fRec49[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 3; l85 = l85 + 1) {
			fRec48[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
			fRec47[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
			fRec46[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 3; l88 = l88 + 1) {
			fRec45[l88] = 0.0f;
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
	
	virtual Faust_engine* clone() {
		return new Faust_engine();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	void compute(int count, FAUSTFLOAT* RESTRICT inputfirst, FAUSTFLOAT* RESTRICT inputsecond, FAUSTFLOAT* RESTRICT outputfirst, FAUSTFLOAT* RESTRICT outputsecond) {
		FAUSTFLOAT* input0 = inputfirst;
		FAUSTFLOAT* input1 = inputsecond;
		FAUSTFLOAT* output0 = outputfirst;
		FAUSTFLOAT* output1 = outputsecond;
		int iSlow0 = trance_sw;
		float fSlow1 = trance_gain;
		float fSlow2 = fConst1 * trance_bps;
		int iSlow3 = l_f_s_sw;
		float fSlow4 = fConst1 * l_f_s_bps;
		float fSlow5 = float(l_f_s_gain_min_freq);
		float fSlow6 = 1e+04f - 0.5f * fSlow5;
		int iSlow7 = panner_sw;
		float fSlow8 = fConst1 * panner_bps;
		float fSlow9 = panner_gain;
		int iSlow10 = phaser_sw;
		int iSlow11 = flanger_sw;
		int iSlow12 = distortion_sw;
		int iSlow13 = echo_sw;
		int iSlow14 = lo_fi_sw;
		float fSlow15 = std::tan(fConst2 * float(low_freq_value));
		float fSlow16 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fSlow15));
		float fSlow17 = 1.0f / fSlow15;
		float fSlow18 = (fSlow17 + -0.26105237f) / fSlow15 + 1.0f;
		float fSlow19 = 1.0f / ((fSlow17 + 0.26105237f) / fSlow15 + 1.0f);
		float fSlow20 = (fSlow17 + -0.76536685f) / fSlow15 + 1.0f;
		float fSlow21 = 1.0f / ((fSlow17 + 0.76536685f) / fSlow15 + 1.0f);
		float fSlow22 = (fSlow17 + -1.2175229f) / fSlow15 + 1.0f;
		float fSlow23 = 1.0f / ((fSlow17 + 1.2175229f) / fSlow15 + 1.0f);
		float fSlow24 = (fSlow17 + -1.5867066f) / fSlow15 + 1.0f;
		float fSlow25 = 1.0f / ((fSlow17 + 1.5867066f) / fSlow15 + 1.0f);
		float fSlow26 = (fSlow17 + -1.847759f) / fSlow15 + 1.0f;
		float fSlow27 = 1.0f / ((fSlow17 + 1.847759f) / fSlow15 + 1.0f);
		float fSlow28 = (fSlow17 + -1.9828898f) / fSlow15 + 1.0f;
		float fSlow29 = 1.0f / ((fSlow17 + 1.9828898f) / fSlow15 + 1.0f);
		int iSlow30 = hi_fi_sw;
		float fSlow31 = std::tan(fConst2 * float(high_freq_value));
		float fSlow32 = mydsp_faustpower2_f(fSlow31);
		float fSlow33 = 1.0f / fSlow32;
		float fSlow34 = 2.0f * (1.0f - fSlow33);
		float fSlow35 = 1.0f / fSlow31;
		float fSlow36 = (fSlow35 + -0.26105237f) / fSlow31 + 1.0f;
		float fSlow37 = 1.0f / ((fSlow35 + 0.26105237f) / fSlow31 + 1.0f);
		float fSlow38 = (fSlow35 + -0.76536685f) / fSlow31 + 1.0f;
		float fSlow39 = 1.0f / ((fSlow35 + 0.76536685f) / fSlow31 + 1.0f);
		float fSlow40 = (fSlow35 + -1.2175229f) / fSlow31 + 1.0f;
		float fSlow41 = 1.0f / ((fSlow35 + 1.2175229f) / fSlow31 + 1.0f);
		float fSlow42 = (fSlow35 + -1.5867066f) / fSlow31 + 1.0f;
		float fSlow43 = 1.0f / ((fSlow35 + 1.5867066f) / fSlow31 + 1.0f);
		float fSlow44 = (fSlow35 + -1.847759f) / fSlow31 + 1.0f;
		float fSlow45 = 1.0f / ((fSlow35 + 1.847759f) / fSlow31 + 1.0f);
		float fSlow46 = (fSlow35 + -1.9828898f) / fSlow31 + 1.0f;
		float fSlow47 = 1.0f / ((fSlow35 + 1.9828898f) / fSlow31 + 1.0f);
		int iSlow48 = eq_high_sw;
		int iSlow49 = eq_mid_sw;
		float fSlow50 = eq_mid_v;
		int iSlow51 = fSlow50 > 0.0f;
		float fSlow52 = fConst10 * std::pow(1e+01f, 0.05f * std::fabs(fSlow50));
		float fSlow53 = ((iSlow51) ? fConst10 : fSlow52);
		float fSlow54 = fConst12 * (fConst12 + fSlow53) + 1.0f;
		float fSlow55 = ((iSlow51) ? fSlow52 : fConst10);
		float fSlow56 = fConst12 * (fConst12 - fSlow55) + 1.0f;
		float fSlow57 = fConst12 * (fConst12 - fSlow53) + 1.0f;
		int iSlow58 = eq_low_sw;
		float fSlow59 = std::pow(1e+01f, 0.05f * eq_low_v);
		float fSlow60 = fConst12 * (fConst12 + fSlow55) + 1.0f;
		float fSlow61 = std::pow(1e+01f, 0.05f * eq_hi_v);
		float fSlow62 = 0.0f - 2.0f / fSlow32;
		int iSlow63 = int(std::min<float>(fConst31, std::max<float>(0.0f, fConst0 * echo_dur))) + 1;
		float fSlow64 = echo_feedback_var;
		float fSlow65 = std::pow(1e+01f, 2.0f * distortion_gain);
		float fSlow66 = fConst1 * flanger_bps;
		float fSlow67 = flanger_gain;
		float fSlow68 = fConst34 * phaser_bps;
		float fSlow69 = std::cos(fSlow68);
		float fSlow70 = std::sin(fSlow68);
		float fSlow71 = phaser_gain;
		float fSlow72 = trim * fader * volume_limit;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			int iTemp0 = 1 - iVec0[1];
			float fTemp1 = ((iTemp0 | iSlow0) ? 0.0f : fSlow2 + fRec0[1]);
			fRec0[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = std::ceil(0.0f - (fRec0[0] - fSlow1));
			float fTemp3 = ((iTemp0 | iSlow3) ? 0.0f : fSlow4 + fRec2[1]);
			fRec2[0] = fTemp3 - std::floor(fTemp3);
			float fTemp4 = std::tan(fConst2 * (fSlow5 + fSlow6 * (ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec2[0]), 65535))] + 1.0f)));
			float fTemp5 = 1.0f / fTemp4;
			float fTemp6 = (fTemp5 + 0.26105237f) / fTemp4 + 1.0f;
			float fTemp7 = 1.0f - 1.0f / mydsp_faustpower2_f(fTemp4);
			float fTemp8 = (fTemp5 + -0.26105237f) / fTemp4 + 1.0f;
			float fTemp9 = (fTemp5 + 0.76536685f) / fTemp4 + 1.0f;
			float fTemp10 = (fTemp5 + -0.76536685f) / fTemp4 + 1.0f;
			float fTemp11 = (fTemp5 + 1.2175229f) / fTemp4 + 1.0f;
			float fTemp12 = (fTemp5 + -1.2175229f) / fTemp4 + 1.0f;
			float fTemp13 = (fTemp5 + 1.5867066f) / fTemp4 + 1.0f;
			float fTemp14 = (fTemp5 + -1.5867066f) / fTemp4 + 1.0f;
			float fTemp15 = (fTemp5 + 1.847759f) / fTemp4 + 1.0f;
			float fTemp16 = (fTemp5 + -1.847759f) / fTemp4 + 1.0f;
			float fTemp17 = (fTemp5 + 1.9828898f) / fTemp4 + 1.0f;
			float fTemp18 = (fTemp5 + -1.9828898f) / fTemp4 + 1.0f;
			float fTemp19 = ((iTemp0 | iSlow7) ? 0.0f : fSlow8 + fRec9[1]);
			fRec9[0] = fTemp19 - std::floor(fTemp19);
			float fTemp20 = fSlow9 * ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec9[0]), 65535))];
			int iTemp21 = int(std::ceil(0.0f - fTemp20));
			float fTemp22 = fConst13 * fRec26[1];
			float fTemp23 = float(input0[i0]);
			float fTemp24 = ((iSlow58) ? 0.0f : fTemp23);
			fVec2[0] = fTemp24;
			fRec28[0] = 0.0f - fConst23 * (fConst21 * fRec28[1] - (fTemp24 + fVec2[1]));
			fRec27[0] = fRec28[0] - fConst20 * (fConst19 * fRec27[2] + fConst17 * fRec27[1]);
			fRec30[0] = fConst24 * fVec2[1] - fConst23 * (fConst21 * fRec30[1] - fConst18 * fTemp24);
			fRec29[0] = fRec30[0] - fConst20 * (fConst19 * fRec29[2] + fConst17 * fRec29[1]);
			float fTemp25 = ((iSlow58) ? fTemp23 : fConst20 * (fConst16 * fRec29[0] + fConst25 * fRec29[1] + fConst16 * fRec29[2] + fSlow59 * (fRec27[2] + fRec27[0] + 2.0f * fRec27[1])));
			fRec26[0] = ((iSlow49) ? 0.0f : fTemp25) - (fRec26[2] * fSlow57 + fTemp22) / fSlow54;
			float fTemp26 = ((iSlow49) ? fTemp25 : (fTemp22 + fRec26[0] * fSlow60 + fRec26[2] * fSlow56) / fSlow54);
			float fTemp27 = ((iSlow48) ? 0.0f : fTemp26);
			fVec3[0] = fTemp27;
			fRec25[0] = fConst29 * fVec3[1] - fConst28 * (fConst26 * fRec25[1] - fConst7 * fTemp27);
			fRec24[0] = fRec25[0] - fConst9 * (fConst8 * fRec24[2] + fConst6 * fRec24[1]);
			fRec32[0] = 0.0f - fConst28 * (fConst26 * fRec32[1] - (fTemp27 + fVec3[1]));
			fRec31[0] = fRec32[0] - fConst9 * (fConst8 * fRec31[2] + fConst6 * fRec31[1]);
			float fTemp28 = ((iSlow48) ? fTemp26 : fConst9 * (fRec31[2] + fRec31[0] + 2.0f * fRec31[1] + fSlow61 * (fConst5 * fRec24[0] + fConst30 * fRec24[1] + fConst5 * fRec24[2])));
			fRec23[0] = ((iSlow30) ? 0.0f : fTemp28) - fSlow47 * (fSlow46 * fRec23[2] + fSlow34 * fRec23[1]);
			fRec22[0] = fSlow47 * (fSlow33 * fRec23[0] + fSlow62 * fRec23[1] + fSlow33 * fRec23[2]) - fSlow45 * (fSlow44 * fRec22[2] + fSlow34 * fRec22[1]);
			fRec21[0] = fSlow45 * (fSlow33 * fRec22[0] + fSlow62 * fRec22[1] + fSlow33 * fRec22[2]) - fSlow43 * (fSlow42 * fRec21[2] + fSlow34 * fRec21[1]);
			fRec20[0] = fSlow43 * (fSlow33 * fRec21[0] + fSlow62 * fRec21[1] + fSlow33 * fRec21[2]) - fSlow41 * (fSlow40 * fRec20[2] + fSlow34 * fRec20[1]);
			fRec19[0] = fSlow41 * (fSlow33 * fRec20[0] + fSlow62 * fRec20[1] + fSlow33 * fRec20[2]) - fSlow39 * (fSlow38 * fRec19[2] + fSlow34 * fRec19[1]);
			fRec18[0] = fSlow39 * (fSlow33 * fRec19[0] + fSlow62 * fRec19[1] + fSlow33 * fRec19[2]) - fSlow37 * (fSlow36 * fRec18[2] + fSlow34 * fRec18[1]);
			float fTemp29 = ((iSlow30) ? fTemp28 : fSlow37 * (fSlow33 * fRec18[0] + fSlow62 * fRec18[1] + fSlow33 * fRec18[2]));
			fRec17[0] = ((iSlow14) ? 0.0f : fTemp29) - fSlow29 * (fSlow28 * fRec17[2] + fSlow16 * fRec17[1]);
			fRec16[0] = fSlow29 * (fRec17[2] + fRec17[0] + 2.0f * fRec17[1]) - fSlow27 * (fSlow26 * fRec16[2] + fSlow16 * fRec16[1]);
			fRec15[0] = fSlow27 * (fRec16[2] + fRec16[0] + 2.0f * fRec16[1]) - fSlow25 * (fSlow24 * fRec15[2] + fSlow16 * fRec15[1]);
			fRec14[0] = fSlow25 * (fRec15[2] + fRec15[0] + 2.0f * fRec15[1]) - fSlow23 * (fSlow22 * fRec14[2] + fSlow16 * fRec14[1]);
			fRec13[0] = fSlow23 * (fRec14[2] + fRec14[0] + 2.0f * fRec14[1]) - fSlow21 * (fSlow20 * fRec13[2] + fSlow16 * fRec13[1]);
			fRec12[0] = fSlow21 * (fRec13[2] + fRec13[0] + 2.0f * fRec13[1]) - fSlow19 * (fSlow18 * fRec12[2] + fSlow16 * fRec12[1]);
			float fTemp30 = ((iSlow14) ? fTemp29 : fSlow19 * (fRec12[2] + fRec12[0] + 2.0f * fRec12[1]));
			fRec11[IOTA0 & 2097151] = fSlow64 * fRec11[(IOTA0 - iSlow63) & 2097151] + ((iSlow13) ? 0.0f : fTemp30);
			float fTemp31 = ((iSlow13) ? fTemp30 : fRec11[IOTA0 & 2097151]);
			float fTemp32 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow65 * ((iSlow12) ? 0.0f : fTemp31)));
			float fTemp33 = ((iSlow12) ? fTemp31 : fTemp32 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp32)));
			float fTemp34 = ((iSlow11) ? 0.0f : fTemp33);
			fVec4[IOTA0 & 63] = fTemp34;
			float fTemp35 = ((iTemp0 | iSlow11) ? 0.0f : fSlow66 + fRec33[1]);
			fRec33[0] = fTemp35 - std::floor(fTemp35);
			float fTemp36 = 3e+01f * (ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec33[0]), 65535))] + 1.0f);
			float fTemp37 = fTemp36 + 1.0f;
			int iTemp38 = int(fTemp36);
			int iTemp39 = int(std::min<float>(fTemp37, float(std::max<int>(0, iTemp38 + 1))));
			float fTemp40 = std::floor(fTemp36);
			float fTemp41 = fTemp36 - fTemp40;
			float fTemp42 = fTemp40 + (1.0f - fTemp36);
			int iTemp43 = int(std::min<float>(fTemp37, float(std::max<int>(0, iTemp38))));
			float fRec10 = 0.0f - (fVec4[(IOTA0 - iTemp43) & 63] * fTemp42 + fTemp41 * fVec4[(IOTA0 - iTemp39) & 63]);
			float fTemp44 = ((iSlow11) ? fTemp33 : 0.5f * (fTemp34 + fSlow67 * fRec10));
			float fTemp45 = ((iSlow10) ? 0.0f : fTemp44);
			fRec43[0] = fSlow70 * fRec44[1] + fSlow69 * fRec43[1];
			fRec44[0] = float(iTemp0) + fSlow69 * fRec44[1] - fSlow70 * fRec43[1];
			float fTemp46 = std::cos(fConst1 * (14137.167f * (1.0f - fRec43[0]) + 3141.5928f));
			fRec42[0] = fTemp45 + fSlow71 * fRec34[1] - (fConst35 * fRec42[1] * fTemp46 + fConst33 * fRec42[2]);
			fRec41[0] = fConst33 * (fRec42[0] - fRec41[2]) + fRec42[2] + fConst35 * fTemp46 * (fRec42[1] - fRec41[1]);
			fRec40[0] = fConst33 * (fRec41[0] - fRec40[2]) + fRec41[2] + fConst35 * fTemp46 * (fRec41[1] - fRec40[1]);
			fRec39[0] = fConst33 * (fRec40[0] - fRec39[2]) + fRec40[2] + fConst35 * fTemp46 * (fRec40[1] - fRec39[1]);
			fRec38[0] = fConst33 * (fRec39[0] - fRec38[2]) + fRec39[2] + fConst35 * fTemp46 * (fRec39[1] - fRec38[1]);
			fRec37[0] = fConst33 * (fRec38[0] - fRec37[2]) + fRec38[2] + fConst35 * fTemp46 * (fRec38[1] - fRec37[1]);
			fRec36[0] = fConst33 * (fRec37[0] - fRec36[2]) + fRec37[2] + fConst35 * fTemp46 * (fRec37[1] - fRec36[1]);
			fRec35[0] = fConst33 * (fRec36[0] - fRec35[2]) + fRec36[2] + fConst35 * fTemp46 * (fRec36[1] - fRec35[1]);
			fRec34[0] = fConst33 * fRec35[0] + fConst35 * fTemp46 * fRec35[1] + fRec35[2];
			float fTemp47 = ((iSlow10) ? fTemp44 : 0.5f * (fRec34[0] + fTemp45));
			float fTemp48 = ((iSlow7) ? 0.0f : fTemp47);
			float fTemp49 = ((iSlow7) ? fTemp47 : ((iTemp21) ? fTemp48 : ((iTemp21) ? 0.0f : fTemp48) * (1.0f - fTemp20)));
			fRec8[0] = ((iSlow3) ? 0.0f : fTemp49) - (fRec8[2] * fTemp18 + 2.0f * fRec8[1] * fTemp7) / fTemp17;
			fRec7[0] = (fRec8[2] + fRec8[0] + 2.0f * fRec8[1]) / fTemp17 - (fRec7[2] * fTemp16 + 2.0f * fTemp7 * fRec7[1]) / fTemp15;
			fRec6[0] = (fRec7[2] + fRec7[0] + 2.0f * fRec7[1]) / fTemp15 - (fRec6[2] * fTemp14 + 2.0f * fTemp7 * fRec6[1]) / fTemp13;
			fRec5[0] = (fRec6[2] + fRec6[0] + 2.0f * fRec6[1]) / fTemp13 - (fRec5[2] * fTemp12 + 2.0f * fTemp7 * fRec5[1]) / fTemp11;
			fRec4[0] = (fRec5[2] + fRec5[0] + 2.0f * fRec5[1]) / fTemp11 - (fRec4[2] * fTemp10 + 2.0f * fTemp7 * fRec4[1]) / fTemp9;
			fRec3[0] = (fRec4[2] + fRec4[0] + 2.0f * fRec4[1]) / fTemp9 - (fRec3[2] * fTemp8 + 2.0f * fTemp7 * fRec3[1]) / fTemp6;
			float fTemp50 = ((iSlow3) ? fTemp49 : (fRec3[2] + fRec3[0] + 2.0f * fRec3[1]) / fTemp6);
			output0[i0] = FAUSTFLOAT(fSlow72 * ((iSlow0) ? fTemp50 : ((iSlow0) ? 0.0f : fTemp50) * fTemp2));
			int iTemp51 = int(std::ceil(fTemp20));
			float fTemp52 = fConst13 * fRec67[1];
			float fTemp53 = float(input1[i0]);
			float fTemp54 = ((iSlow58) ? 0.0f : fTemp53);
			fVec5[0] = fTemp54;
			fRec69[0] = 0.0f - fConst23 * (fConst21 * fRec69[1] - (fTemp54 + fVec5[1]));
			fRec68[0] = fRec69[0] - fConst20 * (fConst19 * fRec68[2] + fConst17 * fRec68[1]);
			fRec71[0] = fConst24 * fVec5[1] - fConst23 * (fConst21 * fRec71[1] - fConst18 * fTemp54);
			fRec70[0] = fRec71[0] - fConst20 * (fConst19 * fRec70[2] + fConst17 * fRec70[1]);
			float fTemp55 = ((iSlow58) ? fTemp53 : fConst20 * (fConst16 * fRec70[0] + fConst25 * fRec70[1] + fConst16 * fRec70[2] + fSlow59 * (fRec68[2] + fRec68[0] + 2.0f * fRec68[1])));
			fRec67[0] = ((iSlow49) ? 0.0f : fTemp55) - (fSlow57 * fRec67[2] + fTemp52) / fSlow54;
			float fTemp56 = ((iSlow49) ? fTemp55 : (fTemp52 + fRec67[0] * fSlow60 + fSlow56 * fRec67[2]) / fSlow54);
			float fTemp57 = ((iSlow48) ? 0.0f : fTemp56);
			fVec6[0] = fTemp57;
			fRec66[0] = fConst29 * fVec6[1] - fConst28 * (fConst26 * fRec66[1] - fConst7 * fTemp57);
			fRec65[0] = fRec66[0] - fConst9 * (fConst8 * fRec65[2] + fConst6 * fRec65[1]);
			fRec73[0] = 0.0f - fConst28 * (fConst26 * fRec73[1] - (fTemp57 + fVec6[1]));
			fRec72[0] = fRec73[0] - fConst9 * (fConst8 * fRec72[2] + fConst6 * fRec72[1]);
			float fTemp58 = ((iSlow48) ? fTemp56 : fConst9 * (fRec72[2] + fRec72[0] + 2.0f * fRec72[1] + fSlow61 * (fConst5 * fRec65[0] + fConst30 * fRec65[1] + fConst5 * fRec65[2])));
			fRec64[0] = ((iSlow30) ? 0.0f : fTemp58) - fSlow47 * (fSlow46 * fRec64[2] + fSlow34 * fRec64[1]);
			fRec63[0] = fSlow47 * (fSlow33 * fRec64[0] + fSlow62 * fRec64[1] + fSlow33 * fRec64[2]) - fSlow45 * (fSlow44 * fRec63[2] + fSlow34 * fRec63[1]);
			fRec62[0] = fSlow45 * (fSlow33 * fRec63[0] + fSlow62 * fRec63[1] + fSlow33 * fRec63[2]) - fSlow43 * (fSlow42 * fRec62[2] + fSlow34 * fRec62[1]);
			fRec61[0] = fSlow43 * (fSlow33 * fRec62[0] + fSlow62 * fRec62[1] + fSlow33 * fRec62[2]) - fSlow41 * (fSlow40 * fRec61[2] + fSlow34 * fRec61[1]);
			fRec60[0] = fSlow41 * (fSlow33 * fRec61[0] + fSlow62 * fRec61[1] + fSlow33 * fRec61[2]) - fSlow39 * (fSlow38 * fRec60[2] + fSlow34 * fRec60[1]);
			fRec59[0] = fSlow39 * (fSlow33 * fRec60[0] + fSlow62 * fRec60[1] + fSlow33 * fRec60[2]) - fSlow37 * (fSlow36 * fRec59[2] + fSlow34 * fRec59[1]);
			float fTemp59 = ((iSlow30) ? fTemp58 : fSlow37 * (fSlow33 * fRec59[0] + fSlow62 * fRec59[1] + fSlow33 * fRec59[2]));
			fRec58[0] = ((iSlow14) ? 0.0f : fTemp59) - fSlow29 * (fSlow28 * fRec58[2] + fSlow16 * fRec58[1]);
			fRec57[0] = fSlow29 * (fRec58[2] + fRec58[0] + 2.0f * fRec58[1]) - fSlow27 * (fSlow26 * fRec57[2] + fSlow16 * fRec57[1]);
			fRec56[0] = fSlow27 * (fRec57[2] + fRec57[0] + 2.0f * fRec57[1]) - fSlow25 * (fSlow24 * fRec56[2] + fSlow16 * fRec56[1]);
			fRec55[0] = fSlow25 * (fRec56[2] + fRec56[0] + 2.0f * fRec56[1]) - fSlow23 * (fSlow22 * fRec55[2] + fSlow16 * fRec55[1]);
			fRec54[0] = fSlow23 * (fRec55[2] + fRec55[0] + 2.0f * fRec55[1]) - fSlow21 * (fSlow20 * fRec54[2] + fSlow16 * fRec54[1]);
			fRec53[0] = fSlow21 * (fRec54[2] + fRec54[0] + 2.0f * fRec54[1]) - fSlow19 * (fSlow18 * fRec53[2] + fSlow16 * fRec53[1]);
			float fTemp60 = ((iSlow14) ? fTemp59 : fSlow19 * (fRec53[2] + fRec53[0] + 2.0f * fRec53[1]));
			fRec52[IOTA0 & 2097151] = fSlow64 * fRec52[(IOTA0 - iSlow63) & 2097151] + ((iSlow13) ? 0.0f : fTemp60);
			float fTemp61 = ((iSlow13) ? fTemp60 : fRec52[IOTA0 & 2097151]);
			float fTemp62 = std::max<float>(-1.0f, std::min<float>(1.0f, fSlow65 * ((iSlow12) ? 0.0f : fTemp61)));
			float fTemp63 = ((iSlow12) ? fTemp61 : fTemp62 * (1.0f - 0.33333334f * mydsp_faustpower2_f(fTemp62)));
			float fTemp64 = ((iSlow11) ? 0.0f : fTemp63);
			fVec7[IOTA0 & 63] = fTemp64;
			float fRec51 = 0.0f - (fTemp42 * fVec7[(IOTA0 - iTemp43) & 63] + fTemp41 * fVec7[(IOTA0 - iTemp39) & 63]);
			float fTemp65 = ((iSlow11) ? fTemp63 : 0.5f * (fTemp64 + fSlow67 * fRec51));
			float fTemp66 = ((iSlow10) ? 0.0f : fTemp65);
			float fTemp67 = std::cos(fConst1 * (14137.167f * (1.0f - fRec44[0]) + 3141.5928f));
			fRec82[0] = fTemp66 + fSlow71 * fRec74[1] - (fConst35 * fRec82[1] * fTemp67 + fConst33 * fRec82[2]);
			fRec81[0] = fConst33 * (fRec82[0] - fRec81[2]) + fRec82[2] + fConst35 * fTemp67 * (fRec82[1] - fRec81[1]);
			fRec80[0] = fConst33 * (fRec81[0] - fRec80[2]) + fRec81[2] + fConst35 * fTemp67 * (fRec81[1] - fRec80[1]);
			fRec79[0] = fConst33 * (fRec80[0] - fRec79[2]) + fRec80[2] + fConst35 * fTemp67 * (fRec80[1] - fRec79[1]);
			fRec78[0] = fConst33 * (fRec79[0] - fRec78[2]) + fRec79[2] + fConst35 * fTemp67 * (fRec79[1] - fRec78[1]);
			fRec77[0] = fConst33 * (fRec78[0] - fRec77[2]) + fRec78[2] + fConst35 * fTemp67 * (fRec78[1] - fRec77[1]);
			fRec76[0] = fConst33 * (fRec77[0] - fRec76[2]) + fRec77[2] + fConst35 * fTemp67 * (fRec77[1] - fRec76[1]);
			fRec75[0] = fConst33 * (fRec76[0] - fRec75[2]) + fRec76[2] + fConst35 * fTemp67 * (fRec76[1] - fRec75[1]);
			fRec74[0] = fConst33 * fRec75[0] + fConst35 * fTemp67 * fRec75[1] + fRec75[2];
			float fTemp68 = ((iSlow10) ? fTemp65 : 0.5f * (fRec74[0] + fTemp66));
			float fTemp69 = ((iSlow7) ? 0.0f : fTemp68);
			float fTemp70 = ((iSlow7) ? fTemp68 : ((iTemp51) ? fTemp69 : ((iTemp51) ? 0.0f : fTemp69) * (fTemp20 + 1.0f)));
			fRec50[0] = ((iSlow3) ? 0.0f : fTemp70) - (fTemp18 * fRec50[2] + 2.0f * fTemp7 * fRec50[1]) / fTemp17;
			fRec49[0] = (fRec50[2] + fRec50[0] + 2.0f * fRec50[1]) / fTemp17 - (fTemp16 * fRec49[2] + 2.0f * fTemp7 * fRec49[1]) / fTemp15;
			fRec48[0] = (fRec49[2] + fRec49[0] + 2.0f * fRec49[1]) / fTemp15 - (fTemp14 * fRec48[2] + 2.0f * fTemp7 * fRec48[1]) / fTemp13;
			fRec47[0] = (fRec48[2] + fRec48[0] + 2.0f * fRec48[1]) / fTemp13 - (fTemp12 * fRec47[2] + 2.0f * fTemp7 * fRec47[1]) / fTemp11;
			fRec46[0] = (fRec47[2] + fRec47[0] + 2.0f * fRec47[1]) / fTemp11 - (fTemp10 * fRec46[2] + 2.0f * fTemp7 * fRec46[1]) / fTemp9;
			fRec45[0] = (fRec46[2] + fRec46[0] + 2.0f * fRec46[1]) / fTemp9 - (fTemp8 * fRec45[2] + 2.0f * fTemp7 * fRec45[1]) / fTemp6;
			float fTemp71 = ((iSlow3) ? fTemp70 : (fRec45[2] + fRec45[0] + 2.0f * fRec45[1]) / fTemp6);
			output1[i0] = FAUSTFLOAT(fSlow72 * ((iSlow0) ? fTemp71 : fTemp2 * ((iSlow0) ? 0.0f : fTemp71)));
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec9[1] = fRec9[0];
			fVec2[1] = fVec2[0];
			fRec28[1] = fRec28[0];
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fRec30[1] = fRec30[0];
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fVec3[1] = fVec3[0];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fRec32[1] = fRec32[0];
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			IOTA0 = IOTA0 + 1;
			fRec33[1] = fRec33[0];
			fRec43[1] = fRec43[0];
			fRec44[1] = fRec44[0];
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
			fRec35[2] = fRec35[1];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
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
			fVec5[1] = fVec5[0];
			fRec69[1] = fRec69[0];
			fRec68[2] = fRec68[1];
			fRec68[1] = fRec68[0];
			fRec71[1] = fRec71[0];
			fRec70[2] = fRec70[1];
			fRec70[1] = fRec70[0];
			fRec67[2] = fRec67[1];
			fRec67[1] = fRec67[0];
			fVec6[1] = fVec6[0];
			fRec66[1] = fRec66[0];
			fRec65[2] = fRec65[1];
			fRec65[1] = fRec65[0];
			fRec73[1] = fRec73[0];
			fRec72[2] = fRec72[1];
			fRec72[1] = fRec72[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fRec63[2] = fRec63[1];
			fRec63[1] = fRec63[0];
			fRec62[2] = fRec62[1];
			fRec62[1] = fRec62[0];
			fRec61[2] = fRec61[1];
			fRec61[1] = fRec61[0];
			fRec60[2] = fRec60[1];
			fRec60[1] = fRec60[0];
			fRec59[2] = fRec59[1];
			fRec59[1] = fRec59[0];
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
			fRec76[2] = fRec76[1];
			fRec76[1] = fRec76[0];
			fRec75[2] = fRec75[1];
			fRec75[1] = fRec75[0];
			fRec74[1] = fRec74[0];
			fRec50[2] = fRec50[1];
			fRec50[1] = fRec50[0];
			fRec49[2] = fRec49[1];
			fRec49[1] = fRec49[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
		}
	}

};

#endif