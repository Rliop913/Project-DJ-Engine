/* ------------------------------------------------------------
name: "EQ"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn EQFAUST -scn EQ_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __EQFAUST_H__
#define  __EQFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "eq.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS EQFAUST
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

static float EQFAUST_faustpower2_f(float value) {
	return value * value;
}

class EQFAUST final : public EQ_PDJE {
	
 private:
	
	float fYec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fRec1_perm[4];
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fRec0_perm[4];
	int iRec2_perm[4];
	float fRec4_perm[4];
	float fRec3_perm[4];
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fRec5_perm[4];
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fRec7_perm[4];
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fRec6_perm[4];
	float fRec9_perm[4];
	float fRec8_perm[4];
	float fYec1_perm[4];
	float fRec11_perm[4];
	float fRec10_perm[4];
	float fRec13_perm[4];
	float fRec12_perm[4];
	float fRec14_perm[4];
	float fRec16_perm[4];
	float fRec15_perm[4];
	float fRec18_perm[4];
	float fRec17_perm[4];
	
 public:
	EQFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn EQFAUST -scn EQ_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "EQ.dsp");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/highshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/lowshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowshelf:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("interpolators.lib/interpolate_cosine:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_cosine:licence", "MIT");
		m->declare("interpolators.lib/interpolate_cubic:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_cubic:licence", "MIT");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/interpolator_cosine:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolator_cosine:licence", "MIT");
		m->declare("interpolators.lib/interpolator_cubic:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolator_cubic:licence", "MIT");
		m->declare("interpolators.lib/interpolator_four_points:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolator_four_points:licence", "MIT");
		m->declare("interpolators.lib/interpolator_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolator_linear:licence", "MIT");
		m->declare("interpolators.lib/interpolator_two_points:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolator_two_points:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "1.3.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "EQ");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
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
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::tan(785.3982f / fConst0);
		fConst2 = 1.0f / fConst1;
		fConst3 = 1.0f - fConst2;
		fConst4 = 1.0f / (fConst2 + 1.0f);
		fConst5 = 1.0f / EQFAUST_faustpower2_f(fConst1);
		fConst6 = 2.0f * (1.0f - fConst5);
		fConst7 = (fConst2 + -1.0f) / fConst1 + 1.0f;
		fConst8 = 1.0f / ((fConst2 + 1.0f) / fConst1 + 1.0f);
		fConst9 = 5890.4863f / (fConst0 * std::sin(13351.769f / fConst0));
		fConst10 = std::tan(6675.8843f / fConst0);
		fConst11 = 1.0f / fConst10;
		fConst12 = 2.0f * (1.0f - 1.0f / EQFAUST_faustpower2_f(fConst10));
		fConst13 = std::tan(12566.371f / fConst0);
		fConst14 = 1.0f / fConst13;
		fConst15 = 1.0f - fConst14;
		fConst16 = 1.0f / (fConst14 + 1.0f);
		fConst17 = 1.0f / EQFAUST_faustpower2_f(fConst13);
		fConst18 = 2.0f * (1.0f - fConst17);
		fConst19 = (fConst14 + -1.0f) / fConst13 + 1.0f;
		fConst20 = 1.0f / ((fConst14 + 1.0f) / fConst13 + 1.0f);
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			fYec0_perm[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec1_perm[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec0_perm[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			iRec2_perm[l3] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec4_perm[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec3_perm[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec5_perm[l6] = 0.0f;
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
			fRec9_perm[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec8_perm[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fYec1_perm[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fRec11_perm[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec10_perm[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec13_perm[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec12_perm[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec14_perm[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec16_perm[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; l18 < 4; l18 = l18 + 1) {
			fRec15_perm[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; l19 < 4; l19 = l19 + 1) {
			fRec18_perm[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; l20 < 4; l20 = l20 + 1) {
			fRec17_perm[l20] = 0.0f;
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
	
	EQFAUST* clone() {
		return new EQFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("EQ");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fYec0_tmp[68];
		float* fYec0 = &fYec0_tmp[4];
		float fZec0[64];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fRec0_tmp[68];
		float* fRec0 = &fRec0_tmp[4];
		int iSlow0 = timerActive > 0;
		int iSlow1 = frames;
		int iRec2_tmp[68];
		int* iRec2 = &iRec2_tmp[4];
		float fZec1[64];
		float fRec4_tmp[68];
		float* fRec4 = &fRec4_tmp[4];
		float fRec3_tmp[68];
		float* fRec3 = &fRec3_tmp[4];
		int iSlow2 = selectInterpolator;
		int iSlow3 = iSlow2 >= 2;
		int iSlow4 = iSlow2 >= 1;
		float fSlow5 = 8.0f / float(iSlow1);
		float fZec2[64];
		int iZec3[64];
		int iSlow6 = v1;
		int iSlow7 = v2;
		int iSlow8 = v3;
		int iSlow9 = v4;
		int iSlow10 = v5;
		int iSlow11 = v6;
		int iSlow12 = v7;
		int iSlow13 = v8;
		int iZec4[64];
		int iZec5[64];
		int iZec6[64];
		float fZec7[64];
		float fZec8[64];
		float fZec9[64];
		float fZec10[64];
		int iSlow14 = iSlow2 >= 3;
		int iZec11[64];
		float fZec12[64];
		int iZec13[64];
		int iZec14[64];
		float fZec15[64];
		float fSlow15 = float(vZero);
		float fZec16[64];
		int iZec17[64];
		float fZec18[64];
		float fZec19[64];
		float fZec20[64];
		float fZec21[64];
		float fZec22[64];
		float fRec5_tmp[68];
		float* fRec5 = &fRec5_tmp[4];
		float fRec7_tmp[68];
		float* fRec7 = &fRec7_tmp[4];
		float fRec6_tmp[68];
		float* fRec6 = &fRec6_tmp[4];
		float fRec9_tmp[68];
		float* fRec9 = &fRec9_tmp[4];
		float fRec8_tmp[68];
		float* fRec8 = &fRec8_tmp[4];
		int iSlow16 = EQSelect;
		int iSlow17 = iSlow16 == 0;
		int iSlow18 = iSlow16 == 1;
		float fZec23[64];
		float fZec24[64];
		float fZec25[64];
		float fZec26[64];
		float fYec1_tmp[68];
		float* fYec1 = &fYec1_tmp[4];
		float fZec27[64];
		float fRec11_tmp[68];
		float* fRec11 = &fRec11_tmp[4];
		float fRec10_tmp[68];
		float* fRec10 = &fRec10_tmp[4];
		float fZec28[64];
		float fRec13_tmp[68];
		float* fRec13 = &fRec13_tmp[4];
		float fRec12_tmp[68];
		float* fRec12 = &fRec12_tmp[4];
		float fZec29[64];
		float fRec14_tmp[68];
		float* fRec14 = &fRec14_tmp[4];
		float fRec16_tmp[68];
		float* fRec16 = &fRec16_tmp[4];
		float fRec15_tmp[68];
		float* fRec15 = &fRec15_tmp[4];
		float fRec18_tmp[68];
		float* fRec18 = &fRec18_tmp[4];
		float fRec17_tmp[68];
		float* fRec17 = &fRec17_tmp[4];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 64); vindex = vindex + 64) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 64;
			/* Recursive loop 0 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec2_tmp[j6] = iRec2_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec2[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec2[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec2_perm[j7] = iRec2_tmp[vsize + j7];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow5 * float(iRec2[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? iSlow13 : iSlow12) : ((iZec3[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? iSlow9 : iSlow8) : ((iZec3[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? iSlow13 : iSlow12) : ((iZec5[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? iSlow9 : iSlow8) : ((iZec5[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec14[i] = ((iZec13[i] >= 4) ? ((iZec13[i] >= 6) ? ((iZec13[i] >= 7) ? iSlow13 : iSlow12) : ((iZec13[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec13[i] >= 2) ? ((iZec13[i] >= 3) ? iSlow9 : iSlow8) : ((iZec13[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec6[i] - iZec4[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = float(((iZec11[i] >= 4) ? ((iZec11[i] >= 6) ? ((iZec11[i] >= 7) ? iSlow13 : iSlow12) : ((iZec11[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec11[i] >= 2) ? ((iZec11[i] >= 3) ? iSlow9 : iSlow8) : ((iZec11[i] >= 1) ? iSlow7 : iSlow6))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = float(iZec14[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = ((iSlow3) ? ((iSlow14) ? fSlow15 : fZec10[i] + 0.5f * fZec9[i] * (float(iZec6[i] - iZec14[i]) + fZec9[i] * (2.0f * fZec15[i] + 4.0f * float(iZec6[i]) + fZec9[i] * (fZec12[i] + 3.0f * float(iZec4[i] - iZec6[i]) - fZec15[i]) - (5.0f * fZec10[i] + fZec12[i])))) : ((iSlow4) ? fZec10[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec10[i] + fZec9[i] * fZec7[i]));
			}
			/* Vectorizable loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fYec0_tmp[j0] = fYec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = float(input0[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fYec0_perm[j1] = fYec0_tmp[vsize + j1];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = fZec16[i] > 0.0f;
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = fConst9 * std::pow(1e+01f, 0.05f * std::fabs(fZec16[i]));
			}
			/* Vectorizable loop 19 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fYec1_tmp[j22] = fYec1_perm[j22];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = float(input1[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fYec1_perm[j23] = fYec1_tmp[vsize + j23];
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = float(input0[i]) + fYec0[i - 1];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = float(input0[i]) - fYec0[i - 1];
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = ((iZec17[i]) ? fConst9 : fZec18[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = float(input1[i]) + fYec1[i - 1];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = float(input1[i]) - fYec1[i - 1];
			}
			/* Recursive loop 25 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = -(fConst4 * (fConst3 * fRec1[i - 1] - fZec0[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Recursive loop 26 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec4_tmp[j8] = fRec4_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = -(fConst4 * (fConst3 * fRec4[i - 1] - fConst2 * fZec1[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec4_perm[j9] = fRec4_tmp[vsize + j9];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = fConst11 * (fConst11 + fZec19[i]) + 1.0f;
			}
			/* Vectorizable loop 28 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = fConst11 * (fConst11 - fZec19[i]) + 1.0f;
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = -(fConst16 * (fConst15 * fRec7[i - 1] - fZec0[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec7_perm[j15] = fRec7_tmp[vsize + j15];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec9_tmp[j18] = fRec9_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = -(fConst16 * (fConst15 * fRec9[i - 1] - fConst14 * fZec1[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec9_perm[j19] = fRec9_tmp[vsize + j19];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = ((iZec17[i]) ? fZec18[i] : fConst9);
			}
			/* Recursive loop 32 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec11_tmp[j24] = fRec11_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = -(fConst4 * (fConst3 * fRec11[i - 1] - fZec27[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec11_perm[j25] = fRec11_tmp[vsize + j25];
			}
			/* Recursive loop 33 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = -(fConst4 * (fConst3 * fRec13[i - 1] - fConst2 * fZec28[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			/* Recursive loop 34 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec16_tmp[j34] = fRec16_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = -(fConst16 * (fConst15 * fRec16[i - 1] - fZec27[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec16_perm[j35] = fRec16_tmp[vsize + j35];
			}
			/* Recursive loop 35 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec18_tmp[j38] = fRec18_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = -(fConst16 * (fConst15 * fRec18[i - 1] - fConst14 * fZec28[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec18_perm[j39] = fRec18_tmp[vsize + j39];
			}
			/* Recursive loop 36 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = fRec1[i] - fConst8 * (fConst7 * fRec0[i - 2] + fConst6 * fRec0[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Recursive loop 37 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec3_tmp[j10] = fRec3_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = fRec4[i] - fConst8 * (fConst7 * fRec3[i - 2] + fConst6 * fRec3[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec3_perm[j11] = fRec3_tmp[vsize + j11];
			}
			/* Recursive loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fConst12 * fRec5[i - 1];
				fRec5[i] = float(input0[i]) - (fRec5[i - 2] * fZec22[i] + fZec21[i]) / fZec20[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			/* Recursive loop 39 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec6_tmp[j16] = fRec6_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec6[i] = fRec7[i] - fConst20 * (fConst19 * fRec6[i - 2] + fConst18 * fRec6[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec6_perm[j17] = fRec6_tmp[vsize + j17];
			}
			/* Recursive loop 40 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec8_tmp[j20] = fRec8_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fRec9[i] - fConst20 * (fConst19 * fRec8[i - 2] + fConst18 * fRec8[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec8_perm[j21] = fRec8_tmp[vsize + j21];
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = std::pow(1e+01f, 0.05f * fZec16[i]);
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = fConst11 * (fConst11 - fZec24[i]) + 1.0f;
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = fConst11 * (fConst11 + fZec24[i]) + 1.0f;
			}
			/* Recursive loop 44 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec10_tmp[j26] = fRec10_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = fRec11[i] - fConst8 * (fConst7 * fRec10[i - 2] + fConst6 * fRec10[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec10_perm[j27] = fRec10_tmp[vsize + j27];
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = fRec13[i] - fConst8 * (fConst7 * fRec12[i - 2] + fConst6 * fRec12[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			/* Recursive loop 46 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec14_tmp[j32] = fRec14_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = fConst12 * fRec14[i - 1];
				fRec14[i] = float(input1[i]) - (fZec22[i] * fRec14[i - 2] + fZec29[i]) / fZec20[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec14_perm[j33] = fRec14_tmp[vsize + j33];
			}
			/* Recursive loop 47 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec15_tmp[j36] = fRec15_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = fRec16[i] - fConst20 * (fConst19 * fRec15[i - 2] + fConst18 * fRec15[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec15_perm[j37] = fRec15_tmp[vsize + j37];
			}
			/* Recursive loop 48 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec17_tmp[j40] = fRec17_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fRec18[i] - fConst20 * (fConst19 * fRec17[i - 2] + fConst18 * fRec17[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec17_perm[j41] = fRec17_tmp[vsize + j41];
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow17) ? fConst20 * (fRec6[i - 2] + fRec6[i] + 2.0f * fRec6[i - 1] + fConst17 * fZec23[i] * (fRec8[i] + fRec8[i - 2] - 2.0f * fRec8[i - 1])) : ((iSlow18) ? (fZec21[i] + fRec5[i] * fZec26[i] + fRec5[i - 2] * fZec25[i]) / fZec20[i] : fConst8 * ((fRec0[i - 2] + fRec0[i] + 2.0f * fRec0[i - 1]) * fZec23[i] + fConst5 * (fRec3[i] + fRec3[i - 2] - 2.0f * fRec3[i - 1])))));
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow17) ? fConst20 * (fRec15[i - 2] + fRec15[i] + 2.0f * fRec15[i - 1] + fConst17 * fZec23[i] * (fRec17[i] + fRec17[i - 2] - 2.0f * fRec17[i - 1])) : ((iSlow18) ? (fZec29[i] + fRec14[i] * fZec26[i] + fZec25[i] * fRec14[i - 2]) / fZec20[i] : fConst8 * (fZec23[i] * (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]) + fConst5 * (fRec12[i] + fRec12[i - 2] - 2.0f * fRec12[i - 1])))));
			}
		}
		/* Remaining frames */
		if (vindex < count) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = count - vindex;
			/* Recursive loop 0 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec2_tmp[j6] = iRec2_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec2[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec2[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec2_perm[j7] = iRec2_tmp[vsize + j7];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow5 * float(iRec2[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? iSlow13 : iSlow12) : ((iZec3[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? iSlow9 : iSlow8) : ((iZec3[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? iSlow13 : iSlow12) : ((iZec5[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? iSlow9 : iSlow8) : ((iZec5[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec14[i] = ((iZec13[i] >= 4) ? ((iZec13[i] >= 6) ? ((iZec13[i] >= 7) ? iSlow13 : iSlow12) : ((iZec13[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec13[i] >= 2) ? ((iZec13[i] >= 3) ? iSlow9 : iSlow8) : ((iZec13[i] >= 1) ? iSlow7 : iSlow6)));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec6[i] - iZec4[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = float(((iZec11[i] >= 4) ? ((iZec11[i] >= 6) ? ((iZec11[i] >= 7) ? iSlow13 : iSlow12) : ((iZec11[i] >= 5) ? iSlow11 : iSlow10)) : ((iZec11[i] >= 2) ? ((iZec11[i] >= 3) ? iSlow9 : iSlow8) : ((iZec11[i] >= 1) ? iSlow7 : iSlow6))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = float(iZec14[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = ((iSlow3) ? ((iSlow14) ? fSlow15 : fZec10[i] + 0.5f * fZec9[i] * (float(iZec6[i] - iZec14[i]) + fZec9[i] * (2.0f * fZec15[i] + 4.0f * float(iZec6[i]) + fZec9[i] * (fZec12[i] + 3.0f * float(iZec4[i] - iZec6[i]) - fZec15[i]) - (5.0f * fZec10[i] + fZec12[i])))) : ((iSlow4) ? fZec10[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec10[i] + fZec9[i] * fZec7[i]));
			}
			/* Vectorizable loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fYec0_tmp[j0] = fYec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[i] = float(input0[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fYec0_perm[j1] = fYec0_tmp[vsize + j1];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = fZec16[i] > 0.0f;
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = fConst9 * std::pow(1e+01f, 0.05f * std::fabs(fZec16[i]));
			}
			/* Vectorizable loop 19 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fYec1_tmp[j22] = fYec1_perm[j22];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[i] = float(input1[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fYec1_perm[j23] = fYec1_tmp[vsize + j23];
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = float(input0[i]) + fYec0[i - 1];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = float(input0[i]) - fYec0[i - 1];
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = ((iZec17[i]) ? fConst9 : fZec18[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = float(input1[i]) + fYec1[i - 1];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = float(input1[i]) - fYec1[i - 1];
			}
			/* Recursive loop 25 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[i] = -(fConst4 * (fConst3 * fRec1[i - 1] - fZec0[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Recursive loop 26 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec4_tmp[j8] = fRec4_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[i] = -(fConst4 * (fConst3 * fRec4[i - 1] - fConst2 * fZec1[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec4_perm[j9] = fRec4_tmp[vsize + j9];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = fConst11 * (fConst11 + fZec19[i]) + 1.0f;
			}
			/* Vectorizable loop 28 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = fConst11 * (fConst11 - fZec19[i]) + 1.0f;
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = -(fConst16 * (fConst15 * fRec7[i - 1] - fZec0[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec7_perm[j15] = fRec7_tmp[vsize + j15];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec9_tmp[j18] = fRec9_perm[j18];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[i] = -(fConst16 * (fConst15 * fRec9[i - 1] - fConst14 * fZec1[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec9_perm[j19] = fRec9_tmp[vsize + j19];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec24[i] = ((iZec17[i]) ? fZec18[i] : fConst9);
			}
			/* Recursive loop 32 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec11_tmp[j24] = fRec11_perm[j24];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec11[i] = -(fConst4 * (fConst3 * fRec11[i - 1] - fZec27[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec11_perm[j25] = fRec11_tmp[vsize + j25];
			}
			/* Recursive loop 33 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec13[i] = -(fConst4 * (fConst3 * fRec13[i - 1] - fConst2 * fZec28[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			/* Recursive loop 34 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec16_tmp[j34] = fRec16_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = -(fConst16 * (fConst15 * fRec16[i - 1] - fZec27[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec16_perm[j35] = fRec16_tmp[vsize + j35];
			}
			/* Recursive loop 35 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; j38 < 4; j38 = j38 + 1) {
				fRec18_tmp[j38] = fRec18_perm[j38];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[i] = -(fConst16 * (fConst15 * fRec18[i - 1] - fConst14 * fZec28[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; j39 < 4; j39 = j39 + 1) {
				fRec18_perm[j39] = fRec18_tmp[vsize + j39];
			}
			/* Recursive loop 36 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[i] = fRec1[i] - fConst8 * (fConst7 * fRec0[i - 2] + fConst6 * fRec0[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Recursive loop 37 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec3_tmp[j10] = fRec3_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[i] = fRec4[i] - fConst8 * (fConst7 * fRec3[i - 2] + fConst6 * fRec3[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec3_perm[j11] = fRec3_tmp[vsize + j11];
			}
			/* Recursive loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = fConst12 * fRec5[i - 1];
				fRec5[i] = float(input0[i]) - (fRec5[i - 2] * fZec22[i] + fZec21[i]) / fZec20[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			/* Recursive loop 39 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec6_tmp[j16] = fRec6_perm[j16];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec6[i] = fRec7[i] - fConst20 * (fConst19 * fRec6[i - 2] + fConst18 * fRec6[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec6_perm[j17] = fRec6_tmp[vsize + j17];
			}
			/* Recursive loop 40 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec8_tmp[j20] = fRec8_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fRec9[i] - fConst20 * (fConst19 * fRec8[i - 2] + fConst18 * fRec8[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec8_perm[j21] = fRec8_tmp[vsize + j21];
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = std::pow(1e+01f, 0.05f * fZec16[i]);
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = fConst11 * (fConst11 - fZec24[i]) + 1.0f;
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = fConst11 * (fConst11 + fZec24[i]) + 1.0f;
			}
			/* Recursive loop 44 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec10_tmp[j26] = fRec10_perm[j26];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[i] = fRec11[i] - fConst8 * (fConst7 * fRec10[i - 2] + fConst6 * fRec10[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec10_perm[j27] = fRec10_tmp[vsize + j27];
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[i] = fRec13[i] - fConst8 * (fConst7 * fRec12[i - 2] + fConst6 * fRec12[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			/* Recursive loop 46 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec14_tmp[j32] = fRec14_perm[j32];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = fConst12 * fRec14[i - 1];
				fRec14[i] = float(input1[i]) - (fZec22[i] * fRec14[i - 2] + fZec29[i]) / fZec20[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec14_perm[j33] = fRec14_tmp[vsize + j33];
			}
			/* Recursive loop 47 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; j36 < 4; j36 = j36 + 1) {
				fRec15_tmp[j36] = fRec15_perm[j36];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec15[i] = fRec16[i] - fConst20 * (fConst19 * fRec15[i - 2] + fConst18 * fRec15[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; j37 < 4; j37 = j37 + 1) {
				fRec15_perm[j37] = fRec15_tmp[vsize + j37];
			}
			/* Recursive loop 48 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; j40 < 4; j40 = j40 + 1) {
				fRec17_tmp[j40] = fRec17_perm[j40];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[i] = fRec18[i] - fConst20 * (fConst19 * fRec17[i - 2] + fConst18 * fRec17[i - 1]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; j41 < 4; j41 = j41 + 1) {
				fRec17_perm[j41] = fRec17_tmp[vsize + j41];
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iSlow17) ? fConst20 * (fRec6[i - 2] + fRec6[i] + 2.0f * fRec6[i - 1] + fConst17 * fZec23[i] * (fRec8[i] + fRec8[i - 2] - 2.0f * fRec8[i - 1])) : ((iSlow18) ? (fZec21[i] + fRec5[i] * fZec26[i] + fRec5[i - 2] * fZec25[i]) / fZec20[i] : fConst8 * ((fRec0[i - 2] + fRec0[i] + 2.0f * fRec0[i - 1]) * fZec23[i] + fConst5 * (fRec3[i] + fRec3[i - 2] - 2.0f * fRec3[i - 1])))));
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(((iSlow17) ? fConst20 * (fRec15[i - 2] + fRec15[i] + 2.0f * fRec15[i - 1] + fConst17 * fZec23[i] * (fRec17[i] + fRec17[i - 2] - 2.0f * fRec17[i - 1])) : ((iSlow18) ? (fZec29[i] + fRec14[i] * fZec26[i] + fZec25[i] * fRec14[i - 2]) / fZec20[i] : fConst8 * (fZec23[i] * (fRec10[i - 2] + fRec10[i] + 2.0f * fRec10[i - 1]) + fConst5 * (fRec12[i] + fRec12[i - 2] - 2.0f * fRec12[i - 1])))));
			}
		}
	}

};

#endif
