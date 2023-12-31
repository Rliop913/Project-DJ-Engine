/* ------------------------------------------------------------
name: "untitled"
Code generated with Faust 2.58.13 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
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

static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class Faust_engine {
public:
	//---------fader&trim---------//
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
	int cru_sw = 1; void crush_sw(bool turn_on) { turn_on ? cru_sw = 0 : cru_sw = 1; }
	//--------values------------//
	int low_freq_value = 500; void set_filter_low_freq_value(int value) { low_freq_value = value; }
	int high_freq_value = 500; void set_filter_high_freq_value(int value) { high_freq_value = value; }
	float eq_mid_v = 0; void set_eq_mid_value(int value) { eq_mid_v = value; }
	float eq_low_v = 0; void set_eq_low_value(int value) { eq_low_v = value; }
	float eq_hi_v = 0; void set_eq_high_value(int value) { eq_hi_v = value; }
	int cru_bits = 3; void set_crush_bits_value(int value) { cru_bits = value; }
 private:
	
	int fSampleRate;
	float fConst1;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst11;
	float fConst12;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fVec0[2];
	float fConst20;
	float fConst22;
	float fRec8[2];
	float fRec7[3];
	float fConst23;
	float fRec10[2];
	float fRec9[3];
	float fConst24;
	float fRec6[3];
	float fVec1[2];
	float fConst25;
	float fConst27;
	float fConst28;
	float fRec5[2];
	float fRec4[3];
	float fConst29;
	float fRec12[2];
	float fRec11[3];
	float fVec2[2];
	float fRec3[2];
	float fRec2[3];
	float fVec3[2];
	float fRec1[2];
	float fRec0[3];
	float fVec4[2];
	float fRec21[2];
	float fRec20[3];
	float fRec23[2];
	float fRec22[3];
	float fRec19[3];
	float fVec5[2];
	float fRec18[2];
	float fRec17[3];
	float fRec25[2];
	float fRec24[3];
	float fVec6[2];
	float fRec16[2];
	float fRec15[3];
	float fVec7[2];
	float fRec14[2];
	float fRec13[3];
	
 public:
	 Faust_engine() {}


	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 3.1415927f / fConst0;
		float fConst2 = std::tan(15707.963f / fConst0);
		float fConst3 = mydsp_faustpower2_f(fConst2);
		fConst4 = 1.0f / fConst3;
		fConst5 = 2.0f * (1.0f - fConst4);
		fConst6 = 1.0f / fConst2;
		fConst7 = (fConst6 + -1.0f) / fConst2 + 1.0f;
		fConst8 = 1.0f / ((fConst6 + 1.0f) / fConst2 + 1.0f);
		fConst9 = 7068.5835f / (fConst0 * std::sin(17278.76f / fConst0));
		float fConst10 = std::tan(8639.38f / fConst0);
		fConst11 = 1.0f / fConst10;
		fConst12 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fConst10));
		float fConst13 = std::tan(1570.7964f / fConst0);
		float fConst14 = mydsp_faustpower2_f(fConst13);
		fConst15 = 1.0f / fConst14;
		fConst16 = 2.0f * (1.0f - fConst15);
		fConst17 = 1.0f / fConst13;
		fConst18 = (fConst17 + -1.0f) / fConst13 + 1.0f;
		fConst19 = 1.0f / ((fConst17 + 1.0f) / fConst13 + 1.0f);
		fConst20 = 1.0f - fConst17;
		float fConst21 = fConst17 + 1.0f;
		fConst22 = 1.0f / fConst21;
		fConst23 = 0.0f - 1.0f / (fConst13 * fConst21);
		fConst24 = 0.0f - 2.0f / fConst14;
		fConst25 = 1.0f - fConst6;
		float fConst26 = fConst6 + 1.0f;
		fConst27 = 1.0f / fConst26;
		fConst28 = 0.0f - 1.0f / (fConst2 * fConst26);
		fConst29 = 0.0f - 2.0f / fConst3;
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec8[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec7[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec10[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec9[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = l5 + 1) {
			fRec6[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec12[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec11[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec2[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec3[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = l13 + 1) {
			fRec2[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec3[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec1[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec0[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			fVec4[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec21[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = l19 + 1) {
			fRec20[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec23[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = l21 + 1) {
			fRec22[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec19[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fVec5[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec18[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = l25 + 1) {
			fRec17[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec25[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = l27 + 1) {
			fRec24[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fVec6[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec16[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec15[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec7[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec14[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = l33 + 1) {
			fRec13[l33] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual Faust_engine* clone() {
		return new Faust_engine();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		int iSlow0 = cru_sw;
		int iSlow1 = lo_fi_sw;
		float fSlow2 = std::tan(fConst1 * float(low_freq_value));
		float fSlow3 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(fSlow2));
		float fSlow4 = 1.0f / fSlow2;
		float fSlow5 = (fSlow4 + -1.0f) / fSlow2 + 1.0f;
		float fSlow6 = 1.0f / ((fSlow4 + 1.0f) / fSlow2 + 1.0f);
		int iSlow7 = hi_fi_sw;
		float fSlow8 = std::tan(fConst1 * float(high_freq_value));
		float fSlow9 = mydsp_faustpower2_f(fSlow8);
		float fSlow10 = 1.0f / fSlow9;
		float fSlow11 = 2.0f * (1.0f - fSlow10);
		float fSlow12 = 1.0f / fSlow8;
		float fSlow13 = (fSlow12 + -1.0f) / fSlow8 + 1.0f;
		float fSlow14 = 1.0f / ((fSlow12 + 1.0f) / fSlow8 + 1.0f);
		int iSlow15 = eq_high_sw;
		int iSlow16 = eq_mid_sw;
		int iSlow17 = eq_mid_v;
		int iSlow18 = iSlow17 > 0;
		float fSlow19 = fConst9 * std::pow(1e+01f, 0.05f * float(std::abs(iSlow17)));
		float fSlow20 = ((iSlow18) ? fConst9 : fSlow19);
		float fSlow21 = fConst11 * (fConst11 + fSlow20) + 1.0f;
		float fSlow22 = ((iSlow18) ? fSlow19 : fConst9);
		float fSlow23 = fConst11 * (fConst11 - fSlow22) + 1.0f;
		float fSlow24 = fConst11 * (fConst11 - fSlow20) + 1.0f;
		int iSlow25 = eq_low_sw;
		float fSlow26 = std::pow(1e+01f, 0.05f * float(eq_low_v));
		float fSlow27 = fConst11 * (fConst11 + fSlow22) + 1.0f;
		float fSlow28 = std::pow(1e+01f, 0.05f * float(eq_hi_v));
		float fSlow29 = 1.0f - fSlow12;
		float fSlow30 = fSlow12 + 1.0f;
		float fSlow31 = 1.0f / fSlow30;
		float fSlow32 = 0.0f - 1.0f / (fSlow8 * fSlow30);
		float fSlow33 = 0.0f - 2.0f / fSlow9;
		float fSlow34 = 1.0f - fSlow4;
		float fSlow35 = 1.0f / (fSlow4 + 1.0f);
		float fSlow36 = float(int(std::pow(float(2), float(cru_bits))) + -1);
		float fSlow37 = 1.0f / fSlow36;
		float fSlow38 = trim * fader;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = fConst12 * fRec6[1];
			float fTemp1 = float(input0[i0]);
			float fTemp2 = ((iSlow25) ? 0.0f : fTemp1);
			fVec0[0] = fTemp2;
			fRec8[0] = 0.0f - fConst22 * (fConst20 * fRec8[1] - (fTemp2 + fVec0[1]));
			fRec7[0] = fRec8[0] - fConst19 * (fConst18 * fRec7[2] + fConst16 * fRec7[1]);
			fRec10[0] = fConst23 * fVec0[1] - fConst22 * (fConst20 * fRec10[1] - fConst17 * fTemp2);
			fRec9[0] = fRec10[0] - fConst19 * (fConst18 * fRec9[2] + fConst16 * fRec9[1]);
			float fTemp3 = ((iSlow25) ? fTemp1 : fConst19 * (fConst15 * fRec9[0] + fConst24 * fRec9[1] + fConst15 * fRec9[2] + fSlow26 * (fRec7[2] + fRec7[0] + 2.0f * fRec7[1])));
			fRec6[0] = ((iSlow16) ? 0.0f : fTemp3) - (fRec6[2] * fSlow24 + fTemp0) / fSlow21;
			float fTemp4 = ((iSlow16) ? fTemp3 : (fTemp0 + fRec6[0] * fSlow27 + fRec6[2] * fSlow23) / fSlow21);
			float fTemp5 = ((iSlow15) ? 0.0f : fTemp4);
			fVec1[0] = fTemp5;
			fRec5[0] = fConst28 * fVec1[1] - fConst27 * (fConst25 * fRec5[1] - fConst6 * fTemp5);
			fRec4[0] = fRec5[0] - fConst8 * (fConst7 * fRec4[2] + fConst5 * fRec4[1]);
			fRec12[0] = 0.0f - fConst27 * (fConst25 * fRec12[1] - (fTemp5 + fVec1[1]));
			fRec11[0] = fRec12[0] - fConst8 * (fConst7 * fRec11[2] + fConst5 * fRec11[1]);
			float fTemp6 = ((iSlow15) ? fTemp4 : fConst8 * (fRec11[2] + fRec11[0] + 2.0f * fRec11[1] + fSlow28 * (fConst4 * fRec4[0] + fConst29 * fRec4[1] + fConst4 * fRec4[2])));
			float fTemp7 = ((iSlow7) ? 0.0f : fTemp6);
			fVec2[0] = fTemp7;
			fRec3[0] = fSlow32 * fVec2[1] - fSlow31 * (fSlow29 * fRec3[1] - fSlow12 * fTemp7);
			fRec2[0] = fRec3[0] - fSlow14 * (fSlow13 * fRec2[2] + fSlow11 * fRec2[1]);
			float fTemp8 = ((iSlow7) ? fTemp6 : fSlow14 * (fSlow10 * fRec2[0] + fSlow33 * fRec2[1] + fSlow10 * fRec2[2]));
			float fTemp9 = ((iSlow1) ? 0.0f : fTemp8);
			fVec3[0] = fTemp9;
			fRec1[0] = 0.0f - fSlow35 * (fSlow34 * fRec1[1] - (fTemp9 + fVec3[1]));
			fRec0[0] = fRec1[0] - fSlow6 * (fSlow5 * fRec0[2] + fSlow3 * fRec0[1]);
			float fTemp10 = ((iSlow1) ? fTemp8 : fSlow6 * (fRec0[2] + fRec0[0] + 2.0f * fRec0[1]));
			output0[i0] = FAUSTFLOAT(fSlow38 * ((iSlow0) ? fTemp10 : fSlow37 * std::floor(fSlow36 * ((iSlow0) ? 0.0f : fTemp10) + 0.5f)));
			float fTemp11 = fConst12 * fRec19[1];
			float fTemp12 = float(input1[i0]);
			float fTemp13 = ((iSlow25) ? 0.0f : fTemp12);
			fVec4[0] = fTemp13;
			fRec21[0] = 0.0f - fConst22 * (fConst20 * fRec21[1] - (fTemp13 + fVec4[1]));
			fRec20[0] = fRec21[0] - fConst19 * (fConst18 * fRec20[2] + fConst16 * fRec20[1]);
			fRec23[0] = fConst23 * fVec4[1] - fConst22 * (fConst20 * fRec23[1] - fConst17 * fTemp13);
			fRec22[0] = fRec23[0] - fConst19 * (fConst18 * fRec22[2] + fConst16 * fRec22[1]);
			float fTemp14 = ((iSlow25) ? fTemp12 : fConst19 * (fConst15 * fRec22[0] + fConst24 * fRec22[1] + fConst15 * fRec22[2] + fSlow26 * (fRec20[2] + fRec20[0] + 2.0f * fRec20[1])));
			fRec19[0] = ((iSlow16) ? 0.0f : fTemp14) - (fSlow24 * fRec19[2] + fTemp11) / fSlow21;
			float fTemp15 = ((iSlow16) ? fTemp14 : (fTemp11 + fRec19[0] * fSlow27 + fSlow23 * fRec19[2]) / fSlow21);
			float fTemp16 = ((iSlow15) ? 0.0f : fTemp15);
			fVec5[0] = fTemp16;
			fRec18[0] = fConst28 * fVec5[1] - fConst27 * (fConst25 * fRec18[1] - fConst6 * fTemp16);
			fRec17[0] = fRec18[0] - fConst8 * (fConst7 * fRec17[2] + fConst5 * fRec17[1]);
			fRec25[0] = 0.0f - fConst27 * (fConst25 * fRec25[1] - (fTemp16 + fVec5[1]));
			fRec24[0] = fRec25[0] - fConst8 * (fConst7 * fRec24[2] + fConst5 * fRec24[1]);
			float fTemp17 = ((iSlow15) ? fTemp15 : fConst8 * (fRec24[2] + fRec24[0] + 2.0f * fRec24[1] + fSlow28 * (fConst4 * fRec17[0] + fConst29 * fRec17[1] + fConst4 * fRec17[2])));
			float fTemp18 = ((iSlow7) ? 0.0f : fTemp17);
			fVec6[0] = fTemp18;
			fRec16[0] = fSlow32 * fVec6[1] - fSlow31 * (fSlow29 * fRec16[1] - fSlow12 * fTemp18);
			fRec15[0] = fRec16[0] - fSlow14 * (fSlow13 * fRec15[2] + fSlow11 * fRec15[1]);
			float fTemp19 = ((iSlow7) ? fTemp17 : fSlow14 * (fSlow10 * fRec15[0] + fSlow33 * fRec15[1] + fSlow10 * fRec15[2]));
			float fTemp20 = ((iSlow1) ? 0.0f : fTemp19);
			fVec7[0] = fTemp20;
			fRec14[0] = 0.0f - fSlow35 * (fSlow34 * fRec14[1] - (fTemp20 + fVec7[1]));
			fRec13[0] = fRec14[0] - fSlow6 * (fSlow5 * fRec13[2] + fSlow3 * fRec13[1]);
			float fTemp21 = ((iSlow1) ? fTemp19 : fSlow6 * (fRec13[2] + fRec13[0] + 2.0f * fRec13[1]));
			output1[i0] = FAUSTFLOAT(fSlow38 * ((iSlow0) ? fTemp21 : fSlow37 * std::floor(fSlow36 * ((iSlow0) ? 0.0f : fTemp21) + 0.5f)));
			fVec0[1] = fVec0[0];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fVec1[1] = fVec1[0];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec12[1] = fRec12[0];
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fVec2[1] = fVec2[0];
			fRec3[1] = fRec3[0];
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fVec3[1] = fVec3[0];
			fRec1[1] = fRec1[0];
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fVec4[1] = fVec4[0];
			fRec21[1] = fRec21[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec23[1] = fRec23[0];
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fVec5[1] = fVec5[0];
			fRec18[1] = fRec18[0];
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fRec25[1] = fRec25[0];
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fVec6[1] = fVec6[0];
			fRec16[1] = fRec16[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec7[1] = fVec7[0];
			fRec14[1] = fRec14[0];
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
		}
	}

};

#endif
