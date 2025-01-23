/* ------------------------------------------------------------
name: "PHASER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __FilterFAUST_H__
#define  __FilterFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

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
	
	int iVec0_perm[4];
	int iRec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec8_perm[4];
	float fRec9_perm[4];
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fRec7_perm[4];
	float fRec6_perm[4];
	float fRec5_perm[4];
	float fRec4_perm[4];
	float fRec3_perm[4];
	float fRec2_perm[4];
	float fRec1_perm[4];
	float fRec16_perm[4];
	float fRec15_perm[4];
	float fRec14_perm[4];
	float fRec13_perm[4];
	float fRec12_perm[4];
	float fRec11_perm[4];
	float fRec10_perm[4];
	
 public:
	FilterFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "PHASER.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
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
		m->declare("name", "PHASER");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("phaflangers.lib/name", "Faust Phaser and Flanger Library");
		m->declare("phaflangers.lib/version", "1.1.0");
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
		fConst1 = 6.2831855f / fConst0;
		fConst2 = std::exp(-(14137.167f / fConst0));
		fConst3 = FilterFAUST_faustpower2_f(fConst2);
		fConst4 = 1.0f / fConst0;
		fConst5 = 2.0f * fConst2;
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iVec0_perm[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			iRec0_perm[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec8_perm[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec9_perm[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec7_perm[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec6_perm[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec5_perm[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec4_perm[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec3_perm[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec2_perm[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec1_perm[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec16_perm[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fRec15_perm[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec14_perm[l13] = 0.0f;
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
			fRec11_perm[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; l17 < 4; l17 = l17 + 1) {
			fRec10_perm[l17] = 0.0f;
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
		ui_interface->openVerticalBox("PHASER");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iSlow0 = timerActive > 0;
		int iSlow1 = frames;
		int iVec0_tmp[68];
		int* iVec0 = &iVec0_tmp[4];
		int iRec0_tmp[68];
		int* iRec0 = &iRec0_tmp[4];
		float fSlow2 = fConst1 * bps;
		float fSlow3 = std::cos(fSlow2);
		float fSlow4 = std::sin(fSlow2);
		float fRec8_tmp[68];
		float* fRec8 = &fRec8_tmp[4];
		float fRec9_tmp[68];
		float* fRec9 = &fRec9_tmp[4];
		float fZec0[64];
		float fRec7_tmp[68];
		float* fRec7 = &fRec7_tmp[4];
		float fRec6_tmp[68];
		float* fRec6 = &fRec6_tmp[4];
		float fRec5_tmp[68];
		float* fRec5 = &fRec5_tmp[4];
		float fRec4_tmp[68];
		float* fRec4 = &fRec4_tmp[4];
		float fRec3_tmp[68];
		float* fRec3 = &fRec3_tmp[4];
		float fRec2_tmp[68];
		float* fRec2 = &fRec2_tmp[4];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		int iSlow5 = selectInterpolator;
		int iSlow6 = iSlow5 >= 2;
		int iSlow7 = iSlow5 >= 1;
		float fSlow8 = 8.0f / float(iSlow1);
		float fZec1[64];
		int iZec2[64];
		int iSlow9 = v1;
		int iSlow10 = v2;
		int iSlow11 = v3;
		int iSlow12 = v4;
		int iSlow13 = v5;
		int iSlow14 = v6;
		int iSlow15 = v7;
		int iSlow16 = v8;
		int iZec3[64];
		int iZec4[64];
		int iZec5[64];
		float fZec6[64];
		float fZec7[64];
		float fZec8[64];
		float fZec9[64];
		int iSlow17 = iSlow5 >= 3;
		int iZec10[64];
		float fZec11[64];
		int iZec12[64];
		int iZec13[64];
		float fZec14[64];
		float fSlow18 = float(vZero);
		float fZec15[64];
		float fZec16[64];
		float fZec17[64];
		float fRec16_tmp[68];
		float* fRec16 = &fRec16_tmp[4];
		float fRec15_tmp[68];
		float* fRec15 = &fRec15_tmp[4];
		float fRec14_tmp[68];
		float* fRec14 = &fRec14_tmp[4];
		float fRec13_tmp[68];
		float* fRec13 = &fRec13_tmp[4];
		float fRec12_tmp[68];
		float* fRec12 = &fRec12_tmp[4];
		float fRec11_tmp[68];
		float* fRec11 = &fRec11_tmp[4];
		float fRec10_tmp[68];
		float* fRec10 = &fRec10_tmp[4];
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
				iVec0_tmp[j0] = iVec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iVec0_perm[j1] = iVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec0[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow8 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? iSlow16 : iSlow15) : ((iZec2[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? iSlow12 : iSlow11) : ((iZec2[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? iSlow16 : iSlow15) : ((iZec4[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? iSlow12 : iSlow11) : ((iZec4[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec2[i] + 2;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? iSlow16 : iSlow15) : ((iZec12[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? iSlow12 : iSlow11) : ((iZec12[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Recursive loop 11 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec9_tmp[j6] = fRec9_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fSlow4 * fRec9[i - 1] + fSlow3 * fRec8[i - 1];
				fRec9[i] = float(1 - iVec0[i - 1]) + fSlow3 * fRec9[i - 1] - fSlow4 * fRec8[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec8_perm[j5] = fRec8_tmp[vsize + j5];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec9_perm[j7] = fRec9_tmp[vsize + j7];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec5[i] - iZec3[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow16 : iSlow15) : ((iZec10[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow12 : iSlow11) : ((iZec10[i] >= 1) ? iSlow10 : iSlow9))));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = float(iZec13[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec8[i]) + 3141.5928f));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow6) ? ((iSlow17) ? fSlow18 : fZec9[i] + 0.5f * fZec8[i] * (float(iZec5[i] - iZec13[i]) + fZec8[i] * (2.0f * fZec14[i] + 4.0f * float(iZec5[i]) + fZec8[i] * (fZec11[i] + 3.0f * float(iZec3[i] - iZec5[i]) - fZec14[i]) - (5.0f * fZec9[i] + fZec11[i])))) : ((iSlow7) ? fZec9[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec9[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Recursive loop 20 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec7_tmp[j8] = fRec7_perm[j8];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec6_tmp[j10] = fRec6_perm[j10];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec4_tmp[j14] = fRec4_perm[j14];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec3_tmp[j16] = fRec3_perm[j16];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec2_tmp[j18] = fRec2_perm[j18];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec1_tmp[j20] = fRec1_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = float(input0[i]) + 0.3f * fRec1[i - 1] + fConst5 * fRec7[i - 1] * fZec0[i] - fConst3 * fRec7[i - 2];
				fRec6[i] = fRec7[i - 2] + fConst3 * (fRec7[i] - fRec6[i - 2]) - fConst5 * fZec0[i] * (fRec7[i - 1] - fRec6[i - 1]);
				fRec5[i] = fRec6[i - 2] + fConst3 * (fRec6[i] - fRec5[i - 2]) - fConst5 * fZec0[i] * (fRec6[i - 1] - fRec5[i - 1]);
				fRec4[i] = fRec5[i - 2] + fConst3 * (fRec5[i] - fRec4[i - 2]) - fConst5 * fZec0[i] * (fRec5[i - 1] - fRec4[i - 1]);
				fRec3[i] = fRec4[i - 2] + fConst3 * (fRec4[i] - fRec3[i - 2]) - fConst5 * fZec0[i] * (fRec4[i - 1] - fRec3[i - 1]);
				fRec2[i] = fRec3[i - 2] + fConst3 * (fRec3[i] - fRec2[i - 2]) - fConst5 * fZec0[i] * (fRec3[i - 1] - fRec2[i - 1]);
				fRec1[i] = fRec2[i - 2] + fConst3 * fRec2[i] - fConst5 * fZec0[i] * fRec2[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec7_perm[j9] = fRec7_tmp[vsize + j9];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec6_perm[j11] = fRec6_tmp[vsize + j11];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec4_perm[j15] = fRec4_tmp[vsize + j15];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec3_perm[j17] = fRec3_tmp[vsize + j17];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec2_perm[j19] = fRec2_tmp[vsize + j19];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec1_perm[j21] = fRec1_tmp[vsize + j21];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - 0.5f * fZec15[i];
			}
			/* Recursive loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec15_tmp[j24] = fRec15_perm[j24];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec14_tmp[j26] = fRec14_perm[j26];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec11_tmp[j32] = fRec11_perm[j32];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec10_tmp[j34] = fRec10_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = float(input1[i]) + 0.3f * fRec10[i - 1] + fConst5 * fRec16[i - 1] * fZec17[i] - fConst3 * fRec16[i - 2];
				fRec15[i] = fRec16[i - 2] + fConst3 * (fRec16[i] - fRec15[i - 2]) - fConst5 * fZec17[i] * (fRec16[i - 1] - fRec15[i - 1]);
				fRec14[i] = fRec15[i - 2] + fConst3 * (fRec15[i] - fRec14[i - 2]) - fConst5 * fZec17[i] * (fRec15[i - 1] - fRec14[i - 1]);
				fRec13[i] = fRec14[i - 2] + fConst3 * (fRec14[i] - fRec13[i - 2]) - fConst5 * fZec17[i] * (fRec14[i - 1] - fRec13[i - 1]);
				fRec12[i] = fRec13[i - 2] + fConst3 * (fRec13[i] - fRec12[i - 2]) - fConst5 * fZec17[i] * (fRec13[i - 1] - fRec12[i - 1]);
				fRec11[i] = fRec12[i - 2] + fConst3 * (fRec12[i] - fRec11[i - 2]) - fConst5 * fZec17[i] * (fRec12[i - 1] - fRec11[i - 1]);
				fRec10[i] = fRec11[i - 2] + fConst3 * fRec11[i] - fConst5 * fZec17[i] * fRec11[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec16_perm[j23] = fRec16_tmp[vsize + j23];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec15_perm[j25] = fRec15_tmp[vsize + j25];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec14_perm[j27] = fRec14_tmp[vsize + j27];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec11_perm[j33] = fRec11_tmp[vsize + j33];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec10_perm[j35] = fRec10_tmp[vsize + j35];
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec16[i] + 0.5f * fRec1[i] * fZec15[i]);
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec16[i] + 0.5f * fRec10[i] * fZec15[i]);
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
				iVec0_tmp[j0] = iVec0_perm[j0];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[i] = 1;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iVec0_perm[j1] = iVec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec0[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow8 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? iSlow16 : iSlow15) : ((iZec2[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? iSlow12 : iSlow11) : ((iZec2[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? iSlow16 : iSlow15) : ((iZec4[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? iSlow12 : iSlow11) : ((iZec4[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec2[i] + 2;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? iSlow16 : iSlow15) : ((iZec12[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? iSlow12 : iSlow11) : ((iZec12[i] >= 1) ? iSlow10 : iSlow9)));
			}
			/* Recursive loop 11 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec9_tmp[j6] = fRec9_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fSlow4 * fRec9[i - 1] + fSlow3 * fRec8[i - 1];
				fRec9[i] = float(1 - iVec0[i - 1]) + fSlow3 * fRec9[i - 1] - fSlow4 * fRec8[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec8_perm[j5] = fRec8_tmp[vsize + j5];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec9_perm[j7] = fRec9_tmp[vsize + j7];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec5[i] - iZec3[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow16 : iSlow15) : ((iZec10[i] >= 5) ? iSlow14 : iSlow13)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow12 : iSlow11) : ((iZec10[i] >= 1) ? iSlow10 : iSlow9))));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = float(iZec13[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec8[i]) + 3141.5928f));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow6) ? ((iSlow17) ? fSlow18 : fZec9[i] + 0.5f * fZec8[i] * (float(iZec5[i] - iZec13[i]) + fZec8[i] * (2.0f * fZec14[i] + 4.0f * float(iZec5[i]) + fZec8[i] * (fZec11[i] + 3.0f * float(iZec3[i] - iZec5[i]) - fZec14[i]) - (5.0f * fZec9[i] + fZec11[i])))) : ((iSlow7) ? fZec9[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec9[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Recursive loop 20 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec7_tmp[j8] = fRec7_perm[j8];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec6_tmp[j10] = fRec6_perm[j10];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec4_tmp[j14] = fRec4_perm[j14];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec3_tmp[j16] = fRec3_perm[j16];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec2_tmp[j18] = fRec2_perm[j18];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; j20 < 4; j20 = j20 + 1) {
				fRec1_tmp[j20] = fRec1_perm[j20];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[i] = float(input0[i]) + 0.3f * fRec1[i - 1] + fConst5 * fRec7[i - 1] * fZec0[i] - fConst3 * fRec7[i - 2];
				fRec6[i] = fRec7[i - 2] + fConst3 * (fRec7[i] - fRec6[i - 2]) - fConst5 * fZec0[i] * (fRec7[i - 1] - fRec6[i - 1]);
				fRec5[i] = fRec6[i - 2] + fConst3 * (fRec6[i] - fRec5[i - 2]) - fConst5 * fZec0[i] * (fRec6[i - 1] - fRec5[i - 1]);
				fRec4[i] = fRec5[i - 2] + fConst3 * (fRec5[i] - fRec4[i - 2]) - fConst5 * fZec0[i] * (fRec5[i - 1] - fRec4[i - 1]);
				fRec3[i] = fRec4[i - 2] + fConst3 * (fRec4[i] - fRec3[i - 2]) - fConst5 * fZec0[i] * (fRec4[i - 1] - fRec3[i - 1]);
				fRec2[i] = fRec3[i - 2] + fConst3 * (fRec3[i] - fRec2[i - 2]) - fConst5 * fZec0[i] * (fRec3[i - 1] - fRec2[i - 1]);
				fRec1[i] = fRec2[i - 2] + fConst3 * fRec2[i] - fConst5 * fZec0[i] * fRec2[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec7_perm[j9] = fRec7_tmp[vsize + j9];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec6_perm[j11] = fRec6_tmp[vsize + j11];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec4_perm[j15] = fRec4_tmp[vsize + j15];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec3_perm[j17] = fRec3_tmp[vsize + j17];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec2_perm[j19] = fRec2_tmp[vsize + j19];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec1_perm[j21] = fRec1_tmp[vsize + j21];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - 0.5f * fZec15[i];
			}
			/* Recursive loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec15_tmp[j24] = fRec15_perm[j24];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec14_tmp[j26] = fRec14_perm[j26];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec11_tmp[j32] = fRec11_perm[j32];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec10_tmp[j34] = fRec10_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = float(input1[i]) + 0.3f * fRec10[i - 1] + fConst5 * fRec16[i - 1] * fZec17[i] - fConst3 * fRec16[i - 2];
				fRec15[i] = fRec16[i - 2] + fConst3 * (fRec16[i] - fRec15[i - 2]) - fConst5 * fZec17[i] * (fRec16[i - 1] - fRec15[i - 1]);
				fRec14[i] = fRec15[i - 2] + fConst3 * (fRec15[i] - fRec14[i - 2]) - fConst5 * fZec17[i] * (fRec15[i - 1] - fRec14[i - 1]);
				fRec13[i] = fRec14[i - 2] + fConst3 * (fRec14[i] - fRec13[i - 2]) - fConst5 * fZec17[i] * (fRec14[i - 1] - fRec13[i - 1]);
				fRec12[i] = fRec13[i - 2] + fConst3 * (fRec13[i] - fRec12[i - 2]) - fConst5 * fZec17[i] * (fRec13[i - 1] - fRec12[i - 1]);
				fRec11[i] = fRec12[i - 2] + fConst3 * (fRec12[i] - fRec11[i - 2]) - fConst5 * fZec17[i] * (fRec12[i - 1] - fRec11[i - 1]);
				fRec10[i] = fRec11[i - 2] + fConst3 * fRec11[i] - fConst5 * fZec17[i] * fRec11[i - 1];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec16_perm[j23] = fRec16_tmp[vsize + j23];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec15_perm[j25] = fRec15_tmp[vsize + j25];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec14_perm[j27] = fRec14_tmp[vsize + j27];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec11_perm[j33] = fRec11_tmp[vsize + j33];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec10_perm[j35] = fRec10_tmp[vsize + j35];
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec16[i] + 0.5f * fRec1[i] * fZec15[i]);
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec16[i] + 0.5f * fRec10[i] * fZec15[i]);
			}
		}
	}

};

#endif
