/* ------------------------------------------------------------
name: "TRANCE"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn TranceFAUST -scn Trance_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __TranceFAUST_H__
#define  __TranceFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "trance.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS TranceFAUST
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


class TranceFAUST final : public Trance_PDJE {
	
 private:
	
	int iVec0_perm[4];
	int iRec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec1_perm[4];
	
 public:
	TranceFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn TranceFAUST -scn Trance_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "TRANCE.dsp");
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
		m->declare("name", "TRANCE");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
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
		fConst0 = 1.0f / std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
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
			fRec1_perm[l2] = 0.0f;
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
	
	TranceFAUST* clone() {
		return new TranceFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("TRANCE");
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
		float fSlow2 = fConst0 * bps;
		float fZec0[64];
		float fZec1[64];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow3 = gain;
		float fZec2[64];
		float fZec3[64];
		int iSlow4 = selectInterpolator;
		int iSlow5 = iSlow4 >= 2;
		int iSlow6 = iSlow4 >= 1;
		float fSlow7 = 8.0f / float(iSlow1);
		float fZec4[64];
		int iZec5[64];
		float fSlow8 = v1;
		float fSlow9 = v2;
		float fSlow10 = v3;
		float fSlow11 = v4;
		float fSlow12 = v5;
		float fSlow13 = v6;
		float fSlow14 = v7;
		float fSlow15 = v8;
		float fZec6[64];
		int iZec7[64];
		float fZec8[64];
		float fZec9[64];
		float fZec10[64];
		float fZec11[64];
		int iSlow16 = iSlow4 >= 3;
		int iZec12[64];
		float fZec13[64];
		int iZec14[64];
		float fZec15[64];
		float fSlow17 = vZero;
		float fZec16[64];
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
				fZec4[i] = fSlow7 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = int(fZec4[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec1_tmp[j4] = fRec1_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow2 + fRec1[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec1[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec1_perm[j5] = fRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec7[i] = iZec5[i] + 1;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow3 - fRec1[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow15 : fSlow14) : ((iZec5[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow11 : fSlow10) : ((iZec5[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = ((iZec7[i] >= 4) ? ((iZec7[i] >= 6) ? ((iZec7[i] >= 7) ? fSlow15 : fSlow14) : ((iZec7[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec7[i] >= 2) ? ((iZec7[i] >= 3) ? fSlow11 : fSlow10) : ((iZec7[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = float(iZec5[i]);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec5[i] + -1;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec14[i] = iZec5[i] + 2;
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = float(int(fZec2[i]));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec8[i] - fZec6[i];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = fZec4[i] - ((fZec4[i] == fZec10[i]) ? fZec4[i] : ((fZec4[i] >= 0.0f) ? fZec10[i] : fZec10[i] + -1.0f));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow15 : fSlow14) : ((iZec12[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow11 : fSlow10) : ((iZec12[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = ((iZec14[i] >= 4) ? ((iZec14[i] >= 6) ? ((iZec14[i] >= 7) ? fSlow15 : fSlow14) : ((iZec14[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec14[i] >= 2) ? ((iZec14[i] >= 3) ? fSlow11 : fSlow10) : ((iZec14[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec6[i] + 0.5f * fZec11[i] * (fZec8[i] + fZec11[i] * (2.0f * fZec13[i] + 4.0f * fZec8[i] + fZec11[i] * (fZec15[i] + 3.0f * (fZec6[i] - fZec8[i]) - fZec13[i]) - (5.0f * fZec6[i] + fZec15[i])) - fZec13[i])) : ((iSlow6) ? fZec6[i] + 0.5f * fZec9[i] * (1.0f - std::cos(3.1415927f * fZec11[i])) : fZec6[i] + fZec11[i] * fZec9[i])))) * (1.0f - ((fZec2[i] == fZec3[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec3[i] + 1.0f : fZec3[i])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec16[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec16[i]);
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
				fZec4[i] = fSlow7 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = int(fZec4[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec1_tmp[j4] = fRec1_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow2 + fRec1[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec1[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec1_perm[j5] = fRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec7[i] = iZec5[i] + 1;
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow3 - fRec1[i];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow15 : fSlow14) : ((iZec5[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow11 : fSlow10) : ((iZec5[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = ((iZec7[i] >= 4) ? ((iZec7[i] >= 6) ? ((iZec7[i] >= 7) ? fSlow15 : fSlow14) : ((iZec7[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec7[i] >= 2) ? ((iZec7[i] >= 3) ? fSlow11 : fSlow10) : ((iZec7[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = float(iZec5[i]);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec5[i] + -1;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec14[i] = iZec5[i] + 2;
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = float(int(fZec2[i]));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec8[i] - fZec6[i];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = fZec4[i] - ((fZec4[i] == fZec10[i]) ? fZec4[i] : ((fZec4[i] >= 0.0f) ? fZec10[i] : fZec10[i] + -1.0f));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow15 : fSlow14) : ((iZec12[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow11 : fSlow10) : ((iZec12[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = ((iZec14[i] >= 4) ? ((iZec14[i] >= 6) ? ((iZec14[i] >= 7) ? fSlow15 : fSlow14) : ((iZec14[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec14[i] >= 2) ? ((iZec14[i] >= 3) ? fSlow11 : fSlow10) : ((iZec14[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec6[i] + 0.5f * fZec11[i] * (fZec8[i] + fZec11[i] * (2.0f * fZec13[i] + 4.0f * fZec8[i] + fZec11[i] * (fZec15[i] + 3.0f * (fZec6[i] - fZec8[i]) - fZec13[i]) - (5.0f * fZec6[i] + fZec15[i])) - fZec13[i])) : ((iSlow6) ? fZec6[i] + 0.5f * fZec9[i] * (1.0f - std::cos(3.1415927f * fZec11[i])) : fZec6[i] + fZec11[i] * fZec9[i])))) * (1.0f - ((fZec2[i] == fZec3[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec3[i] + 1.0f : fZec3[i])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec16[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec16[i]);
			}
		}
	}

};

#endif
