/* ------------------------------------------------------------
name: "OCSFILTER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn OcsFilterFAUSTMan -scn OcsFilterMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __OcsFilterFAUSTMan_H__
#define  __OcsFilterFAUSTMan_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "ocsFilterMan.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS OcsFilterFAUSTMan
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

static float OcsFilterFAUSTMan_faustpower2_f(float value) {
	return value * value;
}

class OcsFilterFAUSTMan final : public OcsFilterMan {
	
 private:
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec6_perm[4];
	float ftbl0OcsFilterFAUSTManSIG0[65536];
	float fConst2;
	float fRec5_perm[4];
	float fRec4_perm[4];
	float fRec3_perm[4];
	float fRec2_perm[4];
	float fRec1_perm[4];
	float fRec0_perm[4];
	float fRec12_perm[4];
	float fRec11_perm[4];
	float fRec10_perm[4];
	float fRec9_perm[4];
	float fRec8_perm[4];
	float fRec18_perm[4];
	float fRec17_perm[4];
	float fRec16_perm[4];
	float fRec15_perm[4];
	float fRec14_perm[4];
	float fRec13_perm[4];
	float fRec23_perm[4];
	float fRec22_perm[4];
	float fRec21_perm[4];
	float fRec20_perm[4];
	float fRec19_perm[4];
	int iVec1[2];
	int iRec7[2];
	
 public:
	OcsFilterFAUSTMan() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn OcsFilterFAUSTMan -scn OcsFilterMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("filename", "OCSFILTER.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "OCSFILTER");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
	}

	static constexpr int getStaticNumInputs() {
		return 2;
	}
	static constexpr int getStaticNumOutputs() {
		return 2;
	}
	int getNumInputs() {
		return 2;
	}
	int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {}
	
	void staticInit(int sample_rate) {
		for (int l2_re0 = 0; l2_re0 < 2; l2_re0 = l2_re0 + 1) {
			iVec1[l2_re0] = 0;
		}
		for (int l3_re0 = 0; l3_re0 < 2; l3_re0 = l3_re0 + 1) {
			iRec7[l3_re0] = 0;
		}
		for (int i1_re0 = 0; i1_re0 < 65536; i1_re0 = i1_re0 + 1) {
			iVec1[0] = 1;
			iRec7[0] = (iVec1[1] + iRec7[1]) % 65536;
			ftbl0OcsFilterFAUSTManSIG0[i1_re0] = std::cos(9.58738e-05f * float(iRec7[0]));
			iVec1[1] = iVec1[0];
			iRec7[1] = iRec7[0];
		}
	}
	
	void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 3.1415927f / fConst0;
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iVec0_perm[l0] = 0;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec6_perm[l1] = 0.0f;
		}
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec5_perm[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec4_perm[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec3_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec2_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec1_perm[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec0_perm[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec12_perm[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec11_perm[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fRec10_perm[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec9_perm[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec8_perm[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec18_perm[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec17_perm[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec16_perm[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec15_perm[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec14_perm[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec13_perm[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 4; l21 = l21 + 1) {
			fRec23_perm[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 4; l22 = l22 + 1) {
			fRec22_perm[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 4; l23 = l23 + 1) {
			fRec21_perm[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
			fRec20_perm[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 4; l25 = l25 + 1) {
			fRec19_perm[l25] = 0.0f;
		}
	}
	
	void init(int sample_rate) {
		instanceInit(sample_rate);
	}
	
	void instanceInit(int sample_rate) {
		staticInit(sample_rate);
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	OcsFilterFAUSTMan* clone() {
		return new OcsFilterFAUSTMan();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("OCSFILTER");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		float fSlow0 = fConst1 * bps;
		float fZec0[32];
		float fZec1[32];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fSlow1 = float(rangeFreqHalf);
		float fSlow2 = float(middleFreq);
		float fZec2[32];
		float fZec3[32];
		float fZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fZec7[32];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fZec8[32];
		float fZec9[32];
		float fZec10[32];
		float fZec11[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fZec12[32];
		float fZec13[32];
		float fZec14[32];
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fZec15[32];
		float fZec16[32];
		float fZec17[32];
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fZec18[32];
		float fZec19[32];
		float fZec20[32];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fZec21[32];
		float fZec22[32];
		float fZec23[32];
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		int iSlow3 = ocsFilterHighLowSW;
		float fZec24[32];
		float fSlow4 = std::min<float>(1.0f, std::max<float>(0.0f, OCSFilterDryWet));
		float fSlow5 = 1.0f - fSlow4;
		float fRec18_tmp[36];
		float* fRec18 = &fRec18_tmp[4];
		float fZec25[32];
		float fRec17_tmp[36];
		float* fRec17 = &fRec17_tmp[4];
		float fRec16_tmp[36];
		float* fRec16 = &fRec16_tmp[4];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		float fRec13_tmp[36];
		float* fRec13 = &fRec13_tmp[4];
		float fRec23_tmp[36];
		float* fRec23 = &fRec23_tmp[4];
		float fRec22_tmp[36];
		float* fRec22 = &fRec22_tmp[4];
		float fRec21_tmp[36];
		float* fRec21 = &fRec21_tmp[4];
		float fRec20_tmp[36];
		float* fRec20 = &fRec20_tmp[4];
		float fRec19_tmp[36];
		float* fRec19 = &fRec19_tmp[4];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 32); vindex = vindex + 32) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
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
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec6_tmp[j2] = fRec6_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec6[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec6[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec6_perm[j3] = fRec6_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = std::tan(fConst2 * std::min<float>(2.4e+04f, std::max<float>(1.0f, fSlow2 + fSlow1 * (ftbl0OcsFilterFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec6[i]), 65535))] + 1.0f))));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1.0f / fZec2[i];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = OcsFilterFAUSTMan_faustpower2_f(fZec2[i]);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = (fZec3[i] + 1.9828898f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = 1.0f - 1.0f / fZec5[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = (fZec3[i] + -1.9828898f) / fZec2[i] + 1.0f;
			}
			/* Recursive loop 8 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec5_tmp[j4] = fRec5_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec5[i] = float(input0[i]) - (fRec5[i - 2] * fZec7[i] + 2.0f * fRec5[i - 1] * fZec6[i]) / fZec4[i];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec5_perm[j5] = fRec5_tmp[vsize + j5];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec18_tmp[j26] = fRec18_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = float(input1[i]) - (fZec7[i] * fRec18[i - 2] + 2.0f * fZec6[i] * fRec18[i - 1]) / fZec4[i];
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec18_perm[j27] = fRec18_tmp[vsize + j27];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = (fZec3[i] + 1.847759f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = (fZec3[i] + -1.847759f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = fZec5[i] * fZec4[i];
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = 2.0f * fRec5[i - 1];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 2.0f * fRec18[i - 1];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = (fRec5[i - 2] + (fRec5[i] - fZec11[i])) / fZec10[i] - (fRec4[i - 2] * fZec9[i] + 2.0f * fZec6[i] * fRec4[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec4_perm[j7] = fRec4_tmp[vsize + j7];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = (fZec3[i] + 1.5867066f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = (fZec3[i] + -1.5867066f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec5[i] * fZec8[i];
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec12_tmp[j16] = fRec12_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = (fRec5[i - 2] + fRec5[i] + fZec11[i]) / fZec4[i] - (fZec9[i] * fRec12[i - 2] + 2.0f * fZec6[i] * fRec12[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec12_perm[j17] = fRec12_tmp[vsize + j17];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec17_tmp[j28] = fRec17_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = (fRec18[i - 2] + (fRec18[i] - fZec25[i])) / fZec10[i] - (fZec9[i] * fRec17[i - 2] + 2.0f * fZec6[i] * fRec17[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec17_perm[j29] = fRec17_tmp[vsize + j29];
			}
			/* Recursive loop 21 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec23_tmp[j38] = fRec23_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = (fRec18[i - 2] + fRec18[i] + fZec25[i]) / fZec4[i] - (fZec9[i] * fRec23[i - 2] + 2.0f * fZec6[i] * fRec23[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec23_perm[j39] = fRec23_tmp[vsize + j39];
			}
			/* Recursive loop 22 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec3_tmp[j8] = fRec3_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = (fRec4[i - 2] + (fRec4[i] - 2.0f * fRec4[i - 1])) / fZec14[i] - (fRec3[i - 2] * fZec13[i] + 2.0f * fZec6[i] * fRec3[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec3_perm[j9] = fRec3_tmp[vsize + j9];
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = (fZec3[i] + 1.2175229f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = (fZec3[i] + -1.2175229f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = fZec5[i] * fZec12[i];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec11_tmp[j18] = fRec11_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = (fRec12[i - 2] + fRec12[i] + 2.0f * fRec12[i - 1]) / fZec8[i] - (fZec13[i] * fRec11[i - 2] + 2.0f * fZec6[i] * fRec11[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec11_perm[j19] = fRec11_tmp[vsize + j19];
			}
			/* Recursive loop 27 */
			/* Pre code */
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec16_tmp[j30] = fRec16_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + (fRec17[i] - 2.0f * fRec17[i - 1])) / fZec14[i] - (fZec13[i] * fRec16[i - 2] + 2.0f * fZec6[i] * fRec16[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec16_perm[j31] = fRec16_tmp[vsize + j31];
			}
			/* Recursive loop 28 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec22_tmp[j40] = fRec22_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = (fRec23[i - 2] + fRec23[i] + 2.0f * fRec23[i - 1]) / fZec8[i] - (fZec13[i] * fRec22[i - 2] + 2.0f * fZec6[i] * fRec22[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec22_perm[j41] = fRec22_tmp[vsize + j41];
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec2_tmp[j10] = fRec2_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = (fRec3[i - 2] + (fRec3[i] - 2.0f * fRec3[i - 1])) / fZec17[i] - (fRec2[i - 2] * fZec16[i] + 2.0f * fZec6[i] * fRec2[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec2_perm[j11] = fRec2_tmp[vsize + j11];
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = (fZec3[i] + 0.76536685f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = (fZec3[i] + -0.76536685f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = fZec5[i] * fZec15[i];
			}
			/* Recursive loop 33 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec10_tmp[j20] = fRec10_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fRec11[i - 2] + fRec11[i] + 2.0f * fRec11[i - 1]) / fZec12[i] - (fZec16[i] * fRec10[i - 2] + 2.0f * fZec6[i] * fRec10[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec10_perm[j21] = fRec10_tmp[vsize + j21];
			}
			/* Recursive loop 34 */
			/* Pre code */
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec15_tmp[j32] = fRec15_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = (fRec16[i - 2] + (fRec16[i] - 2.0f * fRec16[i - 1])) / fZec17[i] - (fZec16[i] * fRec15[i - 2] + 2.0f * fZec6[i] * fRec15[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec15_perm[j33] = fRec15_tmp[vsize + j33];
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec21_tmp[j42] = fRec21_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = (fRec22[i - 2] + fRec22[i] + 2.0f * fRec22[i - 1]) / fZec12[i] - (fZec16[i] * fRec21[i - 2] + 2.0f * fZec6[i] * fRec21[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec21_perm[j43] = fRec21_tmp[vsize + j43];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec1_tmp[j12] = fRec1_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = (fRec2[i - 2] + (fRec2[i] - 2.0f * fRec2[i - 1])) / fZec20[i] - (fRec1[i - 2] * fZec19[i] + 2.0f * fZec6[i] * fRec1[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec1_perm[j13] = fRec1_tmp[vsize + j13];
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = (fZec3[i] + 0.26105237f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 38 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = (fZec3[i] + -0.26105237f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = fZec5[i] * fZec18[i];
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec9_tmp[j22] = fRec9_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]) / fZec15[i] - (fZec19[i] * fRec9[i - 2] + 2.0f * fZec6[i] * fRec9[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec9_perm[j23] = fRec9_tmp[vsize + j23];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec14_tmp[j34] = fRec14_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = (fRec15[i - 2] + (fRec15[i] - 2.0f * fRec15[i - 1])) / fZec20[i] - (fZec19[i] * fRec14[i - 2] + 2.0f * fZec6[i] * fRec14[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec14_perm[j35] = fRec14_tmp[vsize + j35];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec20_tmp[j44] = fRec20_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) / fZec15[i] - (fZec19[i] * fRec20[i - 2] + 2.0f * fZec6[i] * fRec20[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec20_perm[j45] = fRec20_tmp[vsize + j45];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec0_tmp[j14] = fRec0_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = (fRec1[i - 2] + (fRec1[i] - 2.0f * fRec1[i - 1])) / fZec23[i] - (fRec0[i - 2] * fZec22[i] + 2.0f * fZec6[i] * fRec0[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec0_perm[j15] = fRec0_tmp[vsize + j15];
			}
			/* Recursive loop 44 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec8_tmp[j24] = fRec8_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = (fRec9[i - 2] + fRec9[i] + 2.0f * fRec9[i - 1]) / fZec18[i] - (fZec22[i] * fRec8[i - 2] + 2.0f * fZec6[i] * fRec8[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec8_perm[j25] = fRec8_tmp[vsize + j25];
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = fZec5[i] * fZec21[i];
			}
			/* Recursive loop 46 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec13_tmp[j36] = fRec13_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = (fRec14[i - 2] + (fRec14[i] - 2.0f * fRec14[i - 1])) / fZec23[i] - (fZec22[i] * fRec13[i - 2] + 2.0f * fZec6[i] * fRec13[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec13_perm[j37] = fRec13_tmp[vsize + j37];
			}
			/* Recursive loop 47 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec19_tmp[j46] = fRec19_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) / fZec18[i] - (fZec22[i] * fRec19[i - 2] + 2.0f * fZec6[i] * fRec19[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec19_perm[j47] = fRec19_tmp[vsize + j47];
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow5 * float(input0[i]) + fSlow4 * ((iSlow3) ? (fRec8[i - 2] + fRec8[i] + 2.0f * fRec8[i - 1]) / fZec21[i] : (fRec0[i] + fRec0[i - 2] - 2.0f * fRec0[i - 1]) / fZec24[i]));
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow5 * float(input1[i]) + fSlow4 * ((iSlow3) ? (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]) / fZec21[i] : (fRec13[i] + fRec13[i - 2] - 2.0f * fRec13[i - 1]) / fZec24[i]));
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
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec6_tmp[j2] = fRec6_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec6[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec6[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec6_perm[j3] = fRec6_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = std::tan(fConst2 * std::min<float>(2.4e+04f, std::max<float>(1.0f, fSlow2 + fSlow1 * (ftbl0OcsFilterFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec6[i]), 65535))] + 1.0f))));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1.0f / fZec2[i];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = OcsFilterFAUSTMan_faustpower2_f(fZec2[i]);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = (fZec3[i] + 1.9828898f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = 1.0f - 1.0f / fZec5[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = (fZec3[i] + -1.9828898f) / fZec2[i] + 1.0f;
			}
			/* Recursive loop 8 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec5_tmp[j4] = fRec5_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec5[i] = float(input0[i]) - (fRec5[i - 2] * fZec7[i] + 2.0f * fRec5[i - 1] * fZec6[i]) / fZec4[i];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec5_perm[j5] = fRec5_tmp[vsize + j5];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec18_tmp[j26] = fRec18_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = float(input1[i]) - (fZec7[i] * fRec18[i - 2] + 2.0f * fZec6[i] * fRec18[i - 1]) / fZec4[i];
			}
			/* Post code */
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec18_perm[j27] = fRec18_tmp[vsize + j27];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = (fZec3[i] + 1.847759f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = (fZec3[i] + -1.847759f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = fZec5[i] * fZec4[i];
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = 2.0f * fRec5[i - 1];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 2.0f * fRec18[i - 1];
			}
			/* Recursive loop 15 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = (fRec5[i - 2] + (fRec5[i] - fZec11[i])) / fZec10[i] - (fRec4[i - 2] * fZec9[i] + 2.0f * fZec6[i] * fRec4[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec4_perm[j7] = fRec4_tmp[vsize + j7];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = (fZec3[i] + 1.5867066f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = (fZec3[i] + -1.5867066f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec5[i] * fZec8[i];
			}
			/* Recursive loop 19 */
			/* Pre code */
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec12_tmp[j16] = fRec12_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = (fRec5[i - 2] + fRec5[i] + fZec11[i]) / fZec4[i] - (fZec9[i] * fRec12[i - 2] + 2.0f * fZec6[i] * fRec12[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec12_perm[j17] = fRec12_tmp[vsize + j17];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec17_tmp[j28] = fRec17_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = (fRec18[i - 2] + (fRec18[i] - fZec25[i])) / fZec10[i] - (fZec9[i] * fRec17[i - 2] + 2.0f * fZec6[i] * fRec17[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec17_perm[j29] = fRec17_tmp[vsize + j29];
			}
			/* Recursive loop 21 */
			/* Pre code */
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec23_tmp[j38] = fRec23_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = (fRec18[i - 2] + fRec18[i] + fZec25[i]) / fZec4[i] - (fZec9[i] * fRec23[i - 2] + 2.0f * fZec6[i] * fRec23[i - 1]) / fZec8[i];
			}
			/* Post code */
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec23_perm[j39] = fRec23_tmp[vsize + j39];
			}
			/* Recursive loop 22 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec3_tmp[j8] = fRec3_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = (fRec4[i - 2] + (fRec4[i] - 2.0f * fRec4[i - 1])) / fZec14[i] - (fRec3[i - 2] * fZec13[i] + 2.0f * fZec6[i] * fRec3[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec3_perm[j9] = fRec3_tmp[vsize + j9];
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = (fZec3[i] + 1.2175229f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = (fZec3[i] + -1.2175229f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = fZec5[i] * fZec12[i];
			}
			/* Recursive loop 26 */
			/* Pre code */
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec11_tmp[j18] = fRec11_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = (fRec12[i - 2] + fRec12[i] + 2.0f * fRec12[i - 1]) / fZec8[i] - (fZec13[i] * fRec11[i - 2] + 2.0f * fZec6[i] * fRec11[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec11_perm[j19] = fRec11_tmp[vsize + j19];
			}
			/* Recursive loop 27 */
			/* Pre code */
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec16_tmp[j30] = fRec16_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + (fRec17[i] - 2.0f * fRec17[i - 1])) / fZec14[i] - (fZec13[i] * fRec16[i - 2] + 2.0f * fZec6[i] * fRec16[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec16_perm[j31] = fRec16_tmp[vsize + j31];
			}
			/* Recursive loop 28 */
			/* Pre code */
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec22_tmp[j40] = fRec22_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = (fRec23[i - 2] + fRec23[i] + 2.0f * fRec23[i - 1]) / fZec8[i] - (fZec13[i] * fRec22[i - 2] + 2.0f * fZec6[i] * fRec22[i - 1]) / fZec12[i];
			}
			/* Post code */
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec22_perm[j41] = fRec22_tmp[vsize + j41];
			}
			/* Recursive loop 29 */
			/* Pre code */
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec2_tmp[j10] = fRec2_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = (fRec3[i - 2] + (fRec3[i] - 2.0f * fRec3[i - 1])) / fZec17[i] - (fRec2[i - 2] * fZec16[i] + 2.0f * fZec6[i] * fRec2[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec2_perm[j11] = fRec2_tmp[vsize + j11];
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = (fZec3[i] + 0.76536685f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = (fZec3[i] + -0.76536685f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = fZec5[i] * fZec15[i];
			}
			/* Recursive loop 33 */
			/* Pre code */
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec10_tmp[j20] = fRec10_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fRec11[i - 2] + fRec11[i] + 2.0f * fRec11[i - 1]) / fZec12[i] - (fZec16[i] * fRec10[i - 2] + 2.0f * fZec6[i] * fRec10[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec10_perm[j21] = fRec10_tmp[vsize + j21];
			}
			/* Recursive loop 34 */
			/* Pre code */
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec15_tmp[j32] = fRec15_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = (fRec16[i - 2] + (fRec16[i] - 2.0f * fRec16[i - 1])) / fZec17[i] - (fZec16[i] * fRec15[i - 2] + 2.0f * fZec6[i] * fRec15[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec15_perm[j33] = fRec15_tmp[vsize + j33];
			}
			/* Recursive loop 35 */
			/* Pre code */
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec21_tmp[j42] = fRec21_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = (fRec22[i - 2] + fRec22[i] + 2.0f * fRec22[i - 1]) / fZec12[i] - (fZec16[i] * fRec21[i - 2] + 2.0f * fZec6[i] * fRec21[i - 1]) / fZec15[i];
			}
			/* Post code */
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec21_perm[j43] = fRec21_tmp[vsize + j43];
			}
			/* Recursive loop 36 */
			/* Pre code */
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec1_tmp[j12] = fRec1_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = (fRec2[i - 2] + (fRec2[i] - 2.0f * fRec2[i - 1])) / fZec20[i] - (fRec1[i - 2] * fZec19[i] + 2.0f * fZec6[i] * fRec1[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec1_perm[j13] = fRec1_tmp[vsize + j13];
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = (fZec3[i] + 0.26105237f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 38 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = (fZec3[i] + -0.26105237f) / fZec2[i] + 1.0f;
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = fZec5[i] * fZec18[i];
			}
			/* Recursive loop 40 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec9_tmp[j22] = fRec9_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]) / fZec15[i] - (fZec19[i] * fRec9[i - 2] + 2.0f * fZec6[i] * fRec9[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec9_perm[j23] = fRec9_tmp[vsize + j23];
			}
			/* Recursive loop 41 */
			/* Pre code */
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec14_tmp[j34] = fRec14_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[i] = (fRec15[i - 2] + (fRec15[i] - 2.0f * fRec15[i - 1])) / fZec20[i] - (fZec19[i] * fRec14[i - 2] + 2.0f * fZec6[i] * fRec14[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec14_perm[j35] = fRec14_tmp[vsize + j35];
			}
			/* Recursive loop 42 */
			/* Pre code */
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec20_tmp[j44] = fRec20_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) / fZec15[i] - (fZec19[i] * fRec20[i - 2] + 2.0f * fZec6[i] * fRec20[i - 1]) / fZec18[i];
			}
			/* Post code */
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec20_perm[j45] = fRec20_tmp[vsize + j45];
			}
			/* Recursive loop 43 */
			/* Pre code */
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec0_tmp[j14] = fRec0_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = (fRec1[i - 2] + (fRec1[i] - 2.0f * fRec1[i - 1])) / fZec23[i] - (fRec0[i - 2] * fZec22[i] + 2.0f * fZec6[i] * fRec0[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec0_perm[j15] = fRec0_tmp[vsize + j15];
			}
			/* Recursive loop 44 */
			/* Pre code */
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec8_tmp[j24] = fRec8_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = (fRec9[i - 2] + fRec9[i] + 2.0f * fRec9[i - 1]) / fZec18[i] - (fZec22[i] * fRec8[i - 2] + 2.0f * fZec6[i] * fRec8[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec8_perm[j25] = fRec8_tmp[vsize + j25];
			}
			/* Vectorizable loop 45 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = fZec5[i] * fZec21[i];
			}
			/* Recursive loop 46 */
			/* Pre code */
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec13_tmp[j36] = fRec13_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = (fRec14[i - 2] + (fRec14[i] - 2.0f * fRec14[i - 1])) / fZec23[i] - (fZec22[i] * fRec13[i - 2] + 2.0f * fZec6[i] * fRec13[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec13_perm[j37] = fRec13_tmp[vsize + j37];
			}
			/* Recursive loop 47 */
			/* Pre code */
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec19_tmp[j46] = fRec19_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) / fZec18[i] - (fZec22[i] * fRec19[i - 2] + 2.0f * fZec6[i] * fRec19[i - 1]) / fZec21[i];
			}
			/* Post code */
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec19_perm[j47] = fRec19_tmp[vsize + j47];
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow5 * float(input0[i]) + fSlow4 * ((iSlow3) ? (fRec8[i - 2] + fRec8[i] + 2.0f * fRec8[i - 1]) / fZec21[i] : (fRec0[i] + fRec0[i - 2] - 2.0f * fRec0[i - 1]) / fZec24[i]));
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow5 * float(input1[i]) + fSlow4 * ((iSlow3) ? (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]) / fZec21[i] : (fRec13[i] + fRec13[i - 2] - 2.0f * fRec13[i - 1]) / fZec24[i]));
			}
		}
	}

};

#endif
