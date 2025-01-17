/* ------------------------------------------------------------
name: "FILTER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __FilterFAUST_H__
#define  __FilterFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "EFFECTS.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS FilterFAUST
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

static float FilterFAUST_faustpower2_f(float value) {
	return value * value;
}

class FilterFAUST final : public FilterVal {
	
 private:
	
	float fVec0_perm[4];
	int iRec12_perm[4];
	float fRec13_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec11_perm[4];
	float fRec10_perm[4];
	float fRec9_perm[4];
	float fRec8_perm[4];
	float fRec7_perm[4];
	float fRec6_perm[4];
	float fVec1_perm[4];
	int iRec14_perm[4];
	float fRec15_perm[4];
	float fRec5_perm[4];
	float fRec4_perm[4];
	float fRec3_perm[4];
	float fRec2_perm[4];
	float fRec1_perm[4];
	float fRec0_perm[4];
	float fRec27_perm[4];
	float fRec26_perm[4];
	float fRec25_perm[4];
	float fRec24_perm[4];
	float fRec23_perm[4];
	float fRec22_perm[4];
	float fRec21_perm[4];
	float fRec20_perm[4];
	float fRec19_perm[4];
	float fRec18_perm[4];
	float fRec17_perm[4];
	float fRec16_perm[4];
	
 public:
	FilterFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "FILTER.dsp");
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
		m->declare("name", "FILTER");
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
	}
	
	void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = 3.1415927f / std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			fVec0_perm[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			iRec12_perm[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec13_perm[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec11_perm[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec10_perm[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec9_perm[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec8_perm[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec7_perm[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec6_perm[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fVec1_perm[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			iRec14_perm[l10] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec15_perm[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fRec5_perm[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec4_perm[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec3_perm[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec2_perm[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec1_perm[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec0_perm[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec27_perm[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec26_perm[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec25_perm[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; l21 < 4; l21 = l21 + 1) {
			fRec24_perm[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; l22 < 4; l22 = l22 + 1) {
			fRec23_perm[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; l23 < 4; l23 = l23 + 1) {
			fRec22_perm[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
			fRec21_perm[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; l25 < 4; l25 = l25 + 1) {
			fRec20_perm[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; l26 < 4; l26 = l26 + 1) {
			fRec19_perm[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; l27 < 4; l27 = l27 + 1) {
			fRec18_perm[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; l28 < 4; l28 = l28 + 1) {
			fRec17_perm[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; l29 < 4; l29 = l29 + 1) {
			fRec16_perm[l29] = 0.0f;
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
	
	FilterFAUST* clone() {
		return new FilterFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("FILTER");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fSlow0 = HighFreq;
		float fVec0_tmp[68];
		float* fVec0 = &fVec0_tmp[4];
		int iSlow1 = InterpolateSamples;
		int iZec0[64];
		int iRec12_tmp[68];
		int* iRec12 = &iRec12_tmp[4];
		float fRec13_tmp[68];
		float* fRec13 = &fRec13_tmp[4];
		int iSlow2 = InterpolateSW;
		float fZec1[64];
		float fZec2[64];
		float fZec3[64];
		float fZec4[64];
		float fZec5[64];
		float fZec6[64];
		int iSlow3 = HighFilterSW;
		float fRec11_tmp[68];
		float* fRec11 = &fRec11_tmp[4];
		float fZec7[64];
		float fZec8[64];
		float fZec9[64];
		float fRec10_tmp[68];
		float* fRec10 = &fRec10_tmp[4];
		float fZec10[64];
		float fZec11[64];
		float fZec12[64];
		float fRec9_tmp[68];
		float* fRec9 = &fRec9_tmp[4];
		float fZec13[64];
		float fZec14[64];
		float fZec15[64];
		float fRec8_tmp[68];
		float* fRec8 = &fRec8_tmp[4];
		float fZec16[64];
		float fZec17[64];
		float fZec18[64];
		float fRec7_tmp[68];
		float* fRec7 = &fRec7_tmp[4];
		float fZec19[64];
		float fZec20[64];
		float fZec21[64];
		float fRec6_tmp[68];
		float* fRec6 = &fRec6_tmp[4];
		float fSlow4 = LowFreq;
		float fVec1_tmp[68];
		float* fVec1 = &fVec1_tmp[4];
		int iZec22[64];
		int iRec14_tmp[68];
		int* iRec14 = &iRec14_tmp[4];
		float fRec15_tmp[68];
		float* fRec15 = &fRec15_tmp[4];
		float fZec23[64];
		float fZec24[64];
		float fZec25[64];
		float fZec26[64];
		float fZec27[64];
		int iSlow5 = LowFilterSW;
		float fZec28[64];
		float fZec29[64];
		float fRec5_tmp[68];
		float* fRec5 = &fRec5_tmp[4];
		float fZec30[64];
		float fZec31[64];
		float fRec4_tmp[68];
		float* fRec4 = &fRec4_tmp[4];
		float fZec32[64];
		float fZec33[64];
		float fRec3_tmp[68];
		float* fRec3 = &fRec3_tmp[4];
		float fZec34[64];
		float fZec35[64];
		float fRec2_tmp[68];
		float* fRec2 = &fRec2_tmp[4];
		float fZec36[64];
		float fZec37[64];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fZec38[64];
		float fZec39[64];
		float fRec0_tmp[68];
		float* fRec0 = &fRec0_tmp[4];
		float fRec27_tmp[68];
		float* fRec27 = &fRec27_tmp[4];
		float fRec26_tmp[68];
		float* fRec26 = &fRec26_tmp[4];
		float fRec25_tmp[68];
		float* fRec25 = &fRec25_tmp[4];
		float fRec24_tmp[68];
		float* fRec24 = &fRec24_tmp[4];
		float fRec23_tmp[68];
		float* fRec23 = &fRec23_tmp[4];
		float fRec22_tmp[68];
		float* fRec22 = &fRec22_tmp[4];
		float fZec40[64];
		float fRec21_tmp[68];
		float* fRec21 = &fRec21_tmp[4];
		float fRec20_tmp[68];
		float* fRec20 = &fRec20_tmp[4];
		float fRec19_tmp[68];
		float* fRec19 = &fRec19_tmp[4];
		float fRec18_tmp[68];
		float* fRec18 = &fRec18_tmp[4];
		float fRec17_tmp[68];
		float* fRec17 = &fRec17_tmp[4];
		float fRec16_tmp[68];
		float* fRec16 = &fRec16_tmp[4];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 64); vindex = vindex + 64) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 64;
			/* Vectorizable loop 0 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fVec0_tmp[j0] = fVec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fVec0[i] = fSlow0;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fVec0_perm[j1] = fVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec12_tmp[j2] = iRec12_perm[j2];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec13_tmp[j4] = fRec13_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = ((fSlow0 != fVec0[i - 1]) ? iSlow1 : iRec12[i - 1] + -1);
				iRec12[i] = iZec0[i];
				fRec13[i] = ((iZec0[i] > 0) ? fRec13[i - 1] + (fSlow0 - fRec13[i - 1]) / float(iZec0[i]) : fSlow0);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec12_perm[j3] = iRec12_tmp[vsize + j3];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec13_perm[j5] = fRec13_tmp[vsize + j5];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = std::tan(fConst0 * ((iSlow2) ? fSlow0 : fRec13[i]));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = 1.0f / fZec1[i];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = FilterFAUST_faustpower2_f(fZec1[i]);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = (fZec2[i] + 1.9828898f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = 1.0f - 1.0f / fZec4[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = (fZec2[i] + -1.9828898f) / fZec1[i] + 1.0f;
			}
			/* Recursive loop 8 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec11_tmp[j6] = fRec11_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = ((iSlow3) ? 0.0f : float(input0[i])) - (fRec11[i - 2] * fZec6[i] + 2.0f * fRec11[i - 1] * fZec5[i]) / fZec3[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec11_perm[j7] = fRec11_tmp[vsize + j7];
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = (fZec2[i] + 1.847759f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = (fZec2[i] + -1.847759f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec4[i] * fZec3[i];
			}
			/* Recursive loop 12 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec27_tmp[j36] = fRec27_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec27[i] = ((iSlow3) ? 0.0f : float(input1[i])) - (fZec6[i] * fRec27[i - 2] + 2.0f * fZec5[i] * fRec27[i - 1]) / fZec3[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec27_perm[j37] = fRec27_tmp[vsize + j37];
			}
			/* Recursive loop 13 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec10_tmp[j8] = fRec10_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fRec11[i - 2] + (fRec11[i] - 2.0f * fRec11[i - 1])) / fZec9[i] - (fRec10[i - 2] * fZec8[i] + 2.0f * fZec5[i] * fRec10[i - 1]) / fZec7[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec10_perm[j9] = fRec10_tmp[vsize + j9];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = (fZec2[i] + 1.5867066f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = (fZec2[i] + -1.5867066f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = fZec4[i] * fZec7[i];
			}
			/* Recursive loop 17 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec26_tmp[j38] = fRec26_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = (fRec27[i - 2] + (fRec27[i] - 2.0f * fRec27[i - 1])) / fZec9[i] - (fZec8[i] * fRec26[i - 2] + 2.0f * fZec5[i] * fRec26[i - 1]) / fZec7[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec26_perm[j39] = fRec26_tmp[vsize + j39];
			}
			/* Recursive loop 18 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec9_tmp[j10] = fRec9_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = (fRec10[i - 2] + (fRec10[i] - 2.0f * fRec10[i - 1])) / fZec12[i] - (fRec9[i - 2] * fZec11[i] + 2.0f * fZec5[i] * fRec9[i - 1]) / fZec10[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec9_perm[j11] = fRec9_tmp[vsize + j11];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = (fZec2[i] + 1.2175229f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = (fZec2[i] + -1.2175229f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = fZec4[i] * fZec10[i];
			}
			/* Vectorizable loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fVec1_tmp[j18] = fVec1_perm[j18];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fVec1[i] = fSlow4;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fVec1_perm[j19] = fVec1_tmp[vsize + j19];
			}
			/* Recursive loop 23 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec25_tmp[j40] = fRec25_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = (fRec26[i - 2] + (fRec26[i] - 2.0f * fRec26[i - 1])) / fZec12[i] - (fZec11[i] * fRec25[i - 2] + 2.0f * fZec5[i] * fRec25[i - 1]) / fZec10[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec25_perm[j41] = fRec25_tmp[vsize + j41];
			}
			/* Recursive loop 24 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec8_tmp[j12] = fRec8_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = (fRec9[i - 2] + (fRec9[i] - 2.0f * fRec9[i - 1])) / fZec15[i] - (fRec8[i - 2] * fZec14[i] + 2.0f * fZec5[i] * fRec8[i - 1]) / fZec13[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec8_perm[j13] = fRec8_tmp[vsize + j13];
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = (fZec2[i] + 0.76536685f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 26 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = (fZec2[i] + -0.76536685f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = fZec4[i] * fZec13[i];
			}
			/* Recursive loop 28 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				iRec14_tmp[j20] = iRec14_perm[j20];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec15_tmp[j22] = fRec15_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec22[i] = ((fSlow4 != fVec1[i - 1]) ? iSlow1 : iRec14[i - 1] + -1);
				iRec14[i] = iZec22[i];
				fRec15[i] = ((iZec22[i] > 0) ? fRec15[i - 1] + (fSlow4 - fRec15[i - 1]) / float(iZec22[i]) : fSlow4);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				iRec14_perm[j21] = iRec14_tmp[vsize + j21];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec15_perm[j23] = fRec15_tmp[vsize + j23];
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec24_tmp[j42] = fRec24_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = (fRec25[i - 2] + (fRec25[i] - 2.0f * fRec25[i - 1])) / fZec15[i] - (fZec14[i] * fRec24[i - 2] + 2.0f * fZec5[i] * fRec24[i - 1]) / fZec13[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec24_perm[j43] = fRec24_tmp[vsize + j43];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = (fRec8[i - 2] + (fRec8[i] - 2.0f * fRec8[i - 1])) / fZec18[i] - (fRec7[i - 2] * fZec17[i] + 2.0f * fZec5[i] * fRec7[i - 1]) / fZec16[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec7_perm[j15] = fRec7_tmp[vsize + j15];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = (fZec2[i] + 0.26105237f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = (fZec2[i] + -0.26105237f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fZec4[i] * fZec16[i];
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = std::tan(fConst0 * ((iSlow2) ? fSlow4 : fRec15[i]));
			}
			/* Recursive loop 35 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec23_tmp[j44] = fRec23_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = (fRec24[i - 2] + (fRec24[i] - 2.0f * fRec24[i - 1])) / fZec18[i] - (fZec17[i] * fRec23[i - 2] + 2.0f * fZec5[i] * fRec23[i - 1]) / fZec16[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec23_perm[j45] = fRec23_tmp[vsize + j45];
			}
			/* Recursive loop 36 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec6_tmp[j16] = fRec6_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec6[i] = (fRec7[i - 2] + (fRec7[i] - 2.0f * fRec7[i - 1])) / fZec21[i] - (fRec6[i - 2] * fZec20[i] + 2.0f * fZec5[i] * fRec6[i - 1]) / fZec19[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec6_perm[j17] = fRec6_tmp[vsize + j17];
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = 1.0f / fZec23[i];
			}
			/* Recursive loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec22_tmp[j46] = fRec22_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = (fRec23[i - 2] + (fRec23[i] - 2.0f * fRec23[i - 1])) / fZec21[i] - (fZec20[i] * fRec22[i - 2] + 2.0f * fZec5[i] * fRec22[i - 1]) / fZec19[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec22_perm[j47] = fRec22_tmp[vsize + j47];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = (fZec24[i] + 1.9828898f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = 1.0f - 1.0f / FilterFAUST_faustpower2_f(fZec23[i]);
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = (fZec24[i] + -1.9828898f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = fZec4[i] * fZec19[i];
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = 2.0f * fRec6[i - 1];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = 2.0f * fRec22[i - 1];
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec5_tmp[j24] = fRec5_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec5[i] = ((iSlow5) ? 0.0f : ((iSlow3) ? float(input0[i]) : (fRec6[i - 2] + (fRec6[i] - fZec29[i])) / fZec28[i])) - (fRec5[i - 2] * fZec27[i] + 2.0f * fRec5[i - 1] * fZec26[i]) / fZec25[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec5_perm[j25] = fRec5_tmp[vsize + j25];
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec24[i] + 1.847759f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = (fZec24[i] + -1.847759f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 48 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec21_tmp[j48] = fRec21_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = ((iSlow5) ? 0.0f : ((iSlow3) ? float(input1[i]) : (fRec22[i - 2] + (fRec22[i] - fZec40[i])) / fZec28[i])) - (fZec27[i] * fRec21[i - 2] + 2.0f * fZec26[i] * fRec21[i - 1]) / fZec25[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec21_perm[j49] = fRec21_tmp[vsize + j49];
			}
			/* Recursive loop 49 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec4_tmp[j26] = fRec4_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = (fRec5[i - 2] + fRec5[i] + 2.0f * fRec5[i - 1]) / fZec25[i] - (fRec4[i - 2] * fZec31[i] + 2.0f * fZec26[i] * fRec4[i - 1]) / fZec30[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec4_perm[j27] = fRec4_tmp[vsize + j27];
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = (fZec24[i] + 1.5867066f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = (fZec24[i] + -1.5867066f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 52 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec20_tmp[j50] = fRec20_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) / fZec25[i] - (fZec31[i] * fRec20[i - 2] + 2.0f * fZec26[i] * fRec20[i - 1]) / fZec30[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec20_perm[j51] = fRec20_tmp[vsize + j51];
			}
			/* Recursive loop 53 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec3_tmp[j28] = fRec3_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = (fRec4[i - 2] + fRec4[i] + 2.0f * fRec4[i - 1]) / fZec30[i] - (fRec3[i - 2] * fZec33[i] + 2.0f * fZec26[i] * fRec3[i - 1]) / fZec32[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec3_perm[j29] = fRec3_tmp[vsize + j29];
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = (fZec24[i] + 1.2175229f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = (fZec24[i] + -1.2175229f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 56 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec19_tmp[j52] = fRec19_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) / fZec30[i] - (fZec33[i] * fRec19[i - 2] + 2.0f * fZec26[i] * fRec19[i - 1]) / fZec32[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec19_perm[j53] = fRec19_tmp[vsize + j53];
			}
			/* Recursive loop 57 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec2_tmp[j30] = fRec2_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = (fRec3[i - 2] + fRec3[i] + 2.0f * fRec3[i - 1]) / fZec32[i] - (fRec2[i - 2] * fZec35[i] + 2.0f * fZec26[i] * fRec2[i - 1]) / fZec34[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec2_perm[j31] = fRec2_tmp[vsize + j31];
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = (fZec24[i] + 0.76536685f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = (fZec24[i] + -0.76536685f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 60 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec18_tmp[j54] = fRec18_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]) / fZec32[i] - (fZec35[i] * fRec18[i - 2] + 2.0f * fZec26[i] * fRec18[i - 1]) / fZec34[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec18_perm[j55] = fRec18_tmp[vsize + j55];
			}
			/* Recursive loop 61 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec1_tmp[j32] = fRec1_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = (fRec2[i - 2] + fRec2[i] + 2.0f * fRec2[i - 1]) / fZec34[i] - (fRec1[i - 2] * fZec37[i] + 2.0f * fZec26[i] * fRec1[i - 1]) / fZec36[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec1_perm[j33] = fRec1_tmp[vsize + j33];
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = (fZec24[i] + 0.26105237f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = (fZec24[i] + -0.26105237f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 64 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec17_tmp[j56] = fRec17_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = (fRec18[i - 2] + fRec18[i] + 2.0f * fRec18[i - 1]) / fZec34[i] - (fZec37[i] * fRec17[i - 2] + 2.0f * fZec26[i] * fRec17[i - 1]) / fZec36[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec17_perm[j57] = fRec17_tmp[vsize + j57];
			}
			/* Recursive loop 65 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec0_tmp[j34] = fRec0_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = (fRec1[i - 2] + fRec1[i] + 2.0f * fRec1[i - 1]) / fZec36[i] - (fRec0[i - 2] * fZec39[i] + 2.0f * fZec26[i] * fRec0[i - 1]) / fZec38[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec0_perm[j35] = fRec0_tmp[vsize + j35];
			}
			/* Recursive loop 66 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec16_tmp[j58] = fRec16_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + fRec17[i] + 2.0f * fRec17[i - 1]) / fZec36[i] - (fZec39[i] * fRec16[i - 2] + 2.0f * fZec26[i] * fRec16[i - 1]) / fZec38[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec16_perm[j59] = fRec16_tmp[vsize + j59];
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow5) ? ((iSlow3) ? float(input0[i]) : (fRec6[i] + fRec6[i - 2] - fZec29[i]) / fZec28[i]) : (fRec0[i - 2] + fRec0[i] + 2.0f * fRec0[i - 1]) / fZec38[i]));
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow5) ? ((iSlow3) ? float(input1[i]) : (fRec22[i] + fRec22[i - 2] - fZec40[i]) / fZec28[i]) : (fRec16[i - 2] + fRec16[i] + 2.0f * fRec16[i - 1]) / fZec38[i]));
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
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fVec0_tmp[j0] = fVec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fVec0[i] = fSlow0;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fVec0_perm[j1] = fVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec12_tmp[j2] = iRec12_perm[j2];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec13_tmp[j4] = fRec13_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = ((fSlow0 != fVec0[i - 1]) ? iSlow1 : iRec12[i - 1] + -1);
				iRec12[i] = iZec0[i];
				fRec13[i] = ((iZec0[i] > 0) ? fRec13[i - 1] + (fSlow0 - fRec13[i - 1]) / float(iZec0[i]) : fSlow0);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec12_perm[j3] = iRec12_tmp[vsize + j3];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec13_perm[j5] = fRec13_tmp[vsize + j5];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = std::tan(fConst0 * ((iSlow2) ? fSlow0 : fRec13[i]));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = 1.0f / fZec1[i];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = FilterFAUST_faustpower2_f(fZec1[i]);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = (fZec2[i] + 1.9828898f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = 1.0f - 1.0f / fZec4[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = (fZec2[i] + -1.9828898f) / fZec1[i] + 1.0f;
			}
			/* Recursive loop 8 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec11_tmp[j6] = fRec11_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = ((iSlow3) ? 0.0f : float(input0[i])) - (fRec11[i - 2] * fZec6[i] + 2.0f * fRec11[i - 1] * fZec5[i]) / fZec3[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec11_perm[j7] = fRec11_tmp[vsize + j7];
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = (fZec2[i] + 1.847759f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = (fZec2[i] + -1.847759f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec4[i] * fZec3[i];
			}
			/* Recursive loop 12 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec27_tmp[j36] = fRec27_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec27[i] = ((iSlow3) ? 0.0f : float(input1[i])) - (fZec6[i] * fRec27[i - 2] + 2.0f * fZec5[i] * fRec27[i - 1]) / fZec3[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec27_perm[j37] = fRec27_tmp[vsize + j37];
			}
			/* Recursive loop 13 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec10_tmp[j8] = fRec10_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = (fRec11[i - 2] + (fRec11[i] - 2.0f * fRec11[i - 1])) / fZec9[i] - (fRec10[i - 2] * fZec8[i] + 2.0f * fZec5[i] * fRec10[i - 1]) / fZec7[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec10_perm[j9] = fRec10_tmp[vsize + j9];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = (fZec2[i] + 1.5867066f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = (fZec2[i] + -1.5867066f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = fZec4[i] * fZec7[i];
			}
			/* Recursive loop 17 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec26_tmp[j38] = fRec26_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec26[i] = (fRec27[i - 2] + (fRec27[i] - 2.0f * fRec27[i - 1])) / fZec9[i] - (fZec8[i] * fRec26[i - 2] + 2.0f * fZec5[i] * fRec26[i - 1]) / fZec7[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec26_perm[j39] = fRec26_tmp[vsize + j39];
			}
			/* Recursive loop 18 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec9_tmp[j10] = fRec9_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = (fRec10[i - 2] + (fRec10[i] - 2.0f * fRec10[i - 1])) / fZec12[i] - (fRec9[i - 2] * fZec11[i] + 2.0f * fZec5[i] * fRec9[i - 1]) / fZec10[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec9_perm[j11] = fRec9_tmp[vsize + j11];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = (fZec2[i] + 1.2175229f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = (fZec2[i] + -1.2175229f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = fZec4[i] * fZec10[i];
			}
			/* Vectorizable loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fVec1_tmp[j18] = fVec1_perm[j18];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fVec1[i] = fSlow4;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fVec1_perm[j19] = fVec1_tmp[vsize + j19];
			}
			/* Recursive loop 23 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec25_tmp[j40] = fRec25_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec25[i] = (fRec26[i - 2] + (fRec26[i] - 2.0f * fRec26[i - 1])) / fZec12[i] - (fZec11[i] * fRec25[i - 2] + 2.0f * fZec5[i] * fRec25[i - 1]) / fZec10[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec25_perm[j41] = fRec25_tmp[vsize + j41];
			}
			/* Recursive loop 24 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec8_tmp[j12] = fRec8_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = (fRec9[i - 2] + (fRec9[i] - 2.0f * fRec9[i - 1])) / fZec15[i] - (fRec8[i - 2] * fZec14[i] + 2.0f * fZec5[i] * fRec8[i - 1]) / fZec13[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec8_perm[j13] = fRec8_tmp[vsize + j13];
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = (fZec2[i] + 0.76536685f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 26 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = (fZec2[i] + -0.76536685f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = fZec4[i] * fZec13[i];
			}
			/* Recursive loop 28 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				iRec14_tmp[j20] = iRec14_perm[j20];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec15_tmp[j22] = fRec15_perm[j22];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec22[i] = ((fSlow4 != fVec1[i - 1]) ? iSlow1 : iRec14[i - 1] + -1);
				iRec14[i] = iZec22[i];
				fRec15[i] = ((iZec22[i] > 0) ? fRec15[i - 1] + (fSlow4 - fRec15[i - 1]) / float(iZec22[i]) : fSlow4);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				iRec14_perm[j21] = iRec14_tmp[vsize + j21];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec15_perm[j23] = fRec15_tmp[vsize + j23];
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j42 = 0; j42 < 4; j42 = j42 + 1) {
				fRec24_tmp[j42] = fRec24_perm[j42];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec24[i] = (fRec25[i - 2] + (fRec25[i] - 2.0f * fRec25[i - 1])) / fZec15[i] - (fZec14[i] * fRec24[i - 2] + 2.0f * fZec5[i] * fRec24[i - 1]) / fZec13[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j43 = 0; j43 < 4; j43 = j43 + 1) {
				fRec24_perm[j43] = fRec24_tmp[vsize + j43];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = (fRec8[i - 2] + (fRec8[i] - 2.0f * fRec8[i - 1])) / fZec18[i] - (fRec7[i - 2] * fZec17[i] + 2.0f * fZec5[i] * fRec7[i - 1]) / fZec16[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec7_perm[j15] = fRec7_tmp[vsize + j15];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = (fZec2[i] + 0.26105237f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = (fZec2[i] + -0.26105237f) / fZec1[i] + 1.0f;
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fZec4[i] * fZec16[i];
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = std::tan(fConst0 * ((iSlow2) ? fSlow4 : fRec15[i]));
			}
			/* Recursive loop 35 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j44 = 0; j44 < 4; j44 = j44 + 1) {
				fRec23_tmp[j44] = fRec23_perm[j44];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec23[i] = (fRec24[i - 2] + (fRec24[i] - 2.0f * fRec24[i - 1])) / fZec18[i] - (fZec17[i] * fRec23[i - 2] + 2.0f * fZec5[i] * fRec23[i - 1]) / fZec16[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j45 = 0; j45 < 4; j45 = j45 + 1) {
				fRec23_perm[j45] = fRec23_tmp[vsize + j45];
			}
			/* Recursive loop 36 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec6_tmp[j16] = fRec6_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec6[i] = (fRec7[i - 2] + (fRec7[i] - 2.0f * fRec7[i - 1])) / fZec21[i] - (fRec6[i - 2] * fZec20[i] + 2.0f * fZec5[i] * fRec6[i - 1]) / fZec19[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec6_perm[j17] = fRec6_tmp[vsize + j17];
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = 1.0f / fZec23[i];
			}
			/* Recursive loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j46 = 0; j46 < 4; j46 = j46 + 1) {
				fRec22_tmp[j46] = fRec22_perm[j46];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec22[i] = (fRec23[i - 2] + (fRec23[i] - 2.0f * fRec23[i - 1])) / fZec21[i] - (fZec20[i] * fRec22[i - 2] + 2.0f * fZec5[i] * fRec22[i - 1]) / fZec19[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j47 = 0; j47 < 4; j47 = j47 + 1) {
				fRec22_perm[j47] = fRec22_tmp[vsize + j47];
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = (fZec24[i] + 1.9828898f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = 1.0f - 1.0f / FilterFAUST_faustpower2_f(fZec23[i]);
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = (fZec24[i] + -1.9828898f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = fZec4[i] * fZec19[i];
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = 2.0f * fRec6[i - 1];
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = 2.0f * fRec22[i - 1];
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec5_tmp[j24] = fRec5_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec5[i] = ((iSlow5) ? 0.0f : ((iSlow3) ? float(input0[i]) : (fRec6[i - 2] + (fRec6[i] - fZec29[i])) / fZec28[i])) - (fRec5[i - 2] * fZec27[i] + 2.0f * fRec5[i - 1] * fZec26[i]) / fZec25[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec5_perm[j25] = fRec5_tmp[vsize + j25];
			}
			/* Vectorizable loop 46 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = (fZec24[i] + 1.847759f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = (fZec24[i] + -1.847759f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 48 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j48 = 0; j48 < 4; j48 = j48 + 1) {
				fRec21_tmp[j48] = fRec21_perm[j48];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[i] = ((iSlow5) ? 0.0f : ((iSlow3) ? float(input1[i]) : (fRec22[i - 2] + (fRec22[i] - fZec40[i])) / fZec28[i])) - (fZec27[i] * fRec21[i - 2] + 2.0f * fZec26[i] * fRec21[i - 1]) / fZec25[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j49 = 0; j49 < 4; j49 = j49 + 1) {
				fRec21_perm[j49] = fRec21_tmp[vsize + j49];
			}
			/* Recursive loop 49 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec4_tmp[j26] = fRec4_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = (fRec5[i - 2] + fRec5[i] + 2.0f * fRec5[i - 1]) / fZec25[i] - (fRec4[i - 2] * fZec31[i] + 2.0f * fZec26[i] * fRec4[i - 1]) / fZec30[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec4_perm[j27] = fRec4_tmp[vsize + j27];
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = (fZec24[i] + 1.5867066f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 51 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = (fZec24[i] + -1.5867066f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 52 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j50 = 0; j50 < 4; j50 = j50 + 1) {
				fRec20_tmp[j50] = fRec20_perm[j50];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec20[i] = (fRec21[i - 2] + fRec21[i] + 2.0f * fRec21[i - 1]) / fZec25[i] - (fZec31[i] * fRec20[i - 2] + 2.0f * fZec26[i] * fRec20[i - 1]) / fZec30[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j51 = 0; j51 < 4; j51 = j51 + 1) {
				fRec20_perm[j51] = fRec20_tmp[vsize + j51];
			}
			/* Recursive loop 53 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec3_tmp[j28] = fRec3_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = (fRec4[i - 2] + fRec4[i] + 2.0f * fRec4[i - 1]) / fZec30[i] - (fRec3[i - 2] * fZec33[i] + 2.0f * fZec26[i] * fRec3[i - 1]) / fZec32[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec3_perm[j29] = fRec3_tmp[vsize + j29];
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = (fZec24[i] + 1.2175229f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = (fZec24[i] + -1.2175229f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 56 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j52 = 0; j52 < 4; j52 = j52 + 1) {
				fRec19_tmp[j52] = fRec19_perm[j52];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[i] = (fRec20[i - 2] + fRec20[i] + 2.0f * fRec20[i - 1]) / fZec30[i] - (fZec33[i] * fRec19[i - 2] + 2.0f * fZec26[i] * fRec19[i - 1]) / fZec32[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j53 = 0; j53 < 4; j53 = j53 + 1) {
				fRec19_perm[j53] = fRec19_tmp[vsize + j53];
			}
			/* Recursive loop 57 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec2_tmp[j30] = fRec2_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[i] = (fRec3[i - 2] + fRec3[i] + 2.0f * fRec3[i - 1]) / fZec32[i] - (fRec2[i - 2] * fZec35[i] + 2.0f * fZec26[i] * fRec2[i - 1]) / fZec34[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec2_perm[j31] = fRec2_tmp[vsize + j31];
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = (fZec24[i] + 0.76536685f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = (fZec24[i] + -0.76536685f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 60 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j54 = 0; j54 < 4; j54 = j54 + 1) {
				fRec18_tmp[j54] = fRec18_perm[j54];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = (fRec19[i - 2] + fRec19[i] + 2.0f * fRec19[i - 1]) / fZec32[i] - (fZec35[i] * fRec18[i - 2] + 2.0f * fZec26[i] * fRec18[i - 1]) / fZec34[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j55 = 0; j55 < 4; j55 = j55 + 1) {
				fRec18_perm[j55] = fRec18_tmp[vsize + j55];
			}
			/* Recursive loop 61 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec1_tmp[j32] = fRec1_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = (fRec2[i - 2] + fRec2[i] + 2.0f * fRec2[i - 1]) / fZec34[i] - (fRec1[i - 2] * fZec37[i] + 2.0f * fZec26[i] * fRec1[i - 1]) / fZec36[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec1_perm[j33] = fRec1_tmp[vsize + j33];
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = (fZec24[i] + 0.26105237f) / fZec23[i] + 1.0f;
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = (fZec24[i] + -0.26105237f) / fZec23[i] + 1.0f;
			}
			/* Recursive loop 64 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j56 = 0; j56 < 4; j56 = j56 + 1) {
				fRec17_tmp[j56] = fRec17_perm[j56];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = (fRec18[i - 2] + fRec18[i] + 2.0f * fRec18[i - 1]) / fZec34[i] - (fZec37[i] * fRec17[i - 2] + 2.0f * fZec26[i] * fRec17[i - 1]) / fZec36[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j57 = 0; j57 < 4; j57 = j57 + 1) {
				fRec17_perm[j57] = fRec17_tmp[vsize + j57];
			}
			/* Recursive loop 65 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec0_tmp[j34] = fRec0_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = (fRec1[i - 2] + fRec1[i] + 2.0f * fRec1[i - 1]) / fZec36[i] - (fRec0[i - 2] * fZec39[i] + 2.0f * fZec26[i] * fRec0[i - 1]) / fZec38[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec0_perm[j35] = fRec0_tmp[vsize + j35];
			}
			/* Recursive loop 66 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j58 = 0; j58 < 4; j58 = j58 + 1) {
				fRec16_tmp[j58] = fRec16_perm[j58];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = (fRec17[i - 2] + fRec17[i] + 2.0f * fRec17[i - 1]) / fZec36[i] - (fZec39[i] * fRec16[i - 2] + 2.0f * fZec26[i] * fRec16[i - 1]) / fZec38[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j59 = 0; j59 < 4; j59 = j59 + 1) {
				fRec16_perm[j59] = fRec16_tmp[vsize + j59];
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow5) ? ((iSlow3) ? float(input0[i]) : (fRec6[i] + fRec6[i - 2] - fZec29[i]) / fZec28[i]) : (fRec0[i - 2] + fRec0[i] + 2.0f * fRec0[i - 1]) / fZec38[i]));
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow5) ? ((iSlow3) ? float(input1[i]) : (fRec22[i] + fRec22[i - 2] - fZec40[i]) / fZec28[i]) : (fRec16[i - 2] + fRec16[i] + 2.0f * fRec16[i - 1]) / fZec38[i]));
			}
		}
	}

};

#endif
