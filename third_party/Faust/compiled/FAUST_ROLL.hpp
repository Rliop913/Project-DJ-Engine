/* ------------------------------------------------------------
name: "ROLL"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn RollFAUST -scn Roll_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __RollFAUST_H__
#define  __RollFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "roll.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS RollFAUST
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


class RollFAUST final : public Roll_PDJE {
	
 private:
	
	int iRec1_perm[4];
	int fSampleRate;
	float fConst0;
	float fConst1;
	int iRec0_perm[4];
	float fConst2;
	float fRec2_perm[4];
	float ftbl0[96000];
	float ftbl1[96000];
	
 public:
	RollFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn RollFAUST -scn Roll_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "ROLL.dsp");
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
		m->declare("name", "ROLL");
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
		fConst1 = 6e+01f * fConst0;
		fConst2 = 0.016666668f / fConst0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i1_re0 = 0; i1_re0 < 96000; i1_re0 = i1_re0 + 1) {
			ftbl0[i1_re0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i1_re1 = 0; i1_re1 < 96000; i1_re1 = i1_re1 + 1) {
			ftbl1[i1_re1] = 0.0f;
		}
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iRec1_perm[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			iRec0_perm[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec2_perm[l2] = 0.0f;
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
	
	RollFAUST* clone() {
		return new RollFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ROLL");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iSlow0 = timerActive > 0;
		int iSlow1 = frames;
		int iRec1_tmp[68];
		int* iRec1 = &iRec1_tmp[4];
		int iSlow2 = selectInterpolator;
		int iSlow3 = iSlow2 >= 2;
		int iSlow4 = iSlow2 >= 1;
		float fSlow5 = 8.0f / float(iSlow1);
		float fZec0[64];
		int iZec1[64];
		float fSlow6 = v1;
		float fSlow7 = v2;
		float fSlow8 = v3;
		float fSlow9 = v4;
		float fSlow10 = v5;
		float fSlow11 = v6;
		float fSlow12 = v7;
		float fSlow13 = v8;
		float fZec2[64];
		int iZec3[64];
		float fZec4[64];
		float fZec5[64];
		float fZec6[64];
		float fZec7[64];
		int iSlow14 = iSlow2 >= 3;
		int iZec8[64];
		float fZec9[64];
		int iZec10[64];
		float fZec11[64];
		float fSlow15 = vZero;
		float fZec12[64];
		int iZec13[64];
		float fSlow16 = RollBpm;
		float fSlow17 = fConst1 / fSlow16;
		float fSlow18 = fSlow17 + -1.0f;
		int iRec0_tmp[68];
		int* iRec0 = &iRec0_tmp[4];
		float fZec14[64];
		float fSlow19 = fConst2 * fSlow16;
		float fRec2_tmp[68];
		float* fRec2 = &fRec2_tmp[4];
		float fZec15[64];
		int iZec16[64];
		int iZec17[64];
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
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec1_tmp[j0] = iRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec1[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec1_perm[j1] = iRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow5 * float(iRec1[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = int(fZec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = iZec1[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec1[i] >= 4) ? ((iZec1[i] >= 6) ? ((iZec1[i] >= 7) ? fSlow13 : fSlow12) : ((iZec1[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec1[i] >= 2) ? ((iZec1[i] >= 3) ? fSlow9 : fSlow8) : ((iZec1[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow13 : fSlow12) : ((iZec3[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow9 : fSlow8) : ((iZec3[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec1[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = iZec1[i] + -1;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec1[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fZec4[i] - fZec2[i];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec0[i] - ((fZec0[i] == fZec6[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec6[i] : fZec6[i] + -1.0f));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? fSlow13 : fSlow12) : ((iZec8[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? fSlow9 : fSlow8) : ((iZec8[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow13 : fSlow12) : ((iZec10[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow9 : fSlow8) : ((iZec10[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow3) ? ((iSlow14) ? fSlow15 : fZec2[i] + 0.5f * fZec7[i] * (fZec4[i] + fZec7[i] * (2.0f * fZec9[i] + 4.0f * fZec4[i] + fZec7[i] * (fZec11[i] + 3.0f * (fZec2[i] - fZec4[i]) - fZec9[i]) - (5.0f * fZec2[i] + fZec11[i])) - fZec9[i])) : ((iSlow4) ? fZec2[i] + 0.5f * fZec5[i] * (1.0f - std::cos(3.1415927f * fZec7[i])) : fZec2[i] + fZec7[i] * fZec5[i]))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = fZec12[i] > 0.01f;
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iRec0[i - 1] * iZec13[i] + 1), fSlow18));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Recursive loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec2_tmp[j4] = fRec2_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fRec2[i - 1] * float(iZec13[i]);
				fRec2[i] = fZec14[i] + (1.0f - fSlow17 * float(int(fSlow19 * (fZec14[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec2_perm[j5] = fRec2_tmp[vsize + j5];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec12[i];
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec16[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = std::max<int>(0, std::min<int>(int(fRec2[i]), 95999));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec16[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(ftbl0[iZec17[i]] * fZec12[i] + float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec16[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(ftbl1[iZec17[i]] * fZec12[i] + float(input1[i]) * fZec15[i]);
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
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec1_tmp[j0] = iRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec1[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec1_perm[j1] = iRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow5 * float(iRec1[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = int(fZec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = iZec1[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec1[i] >= 4) ? ((iZec1[i] >= 6) ? ((iZec1[i] >= 7) ? fSlow13 : fSlow12) : ((iZec1[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec1[i] >= 2) ? ((iZec1[i] >= 3) ? fSlow9 : fSlow8) : ((iZec1[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow13 : fSlow12) : ((iZec3[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow9 : fSlow8) : ((iZec3[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec1[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = iZec1[i] + -1;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec1[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fZec4[i] - fZec2[i];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec0[i] - ((fZec0[i] == fZec6[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec6[i] : fZec6[i] + -1.0f));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? fSlow13 : fSlow12) : ((iZec8[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? fSlow9 : fSlow8) : ((iZec8[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow13 : fSlow12) : ((iZec10[i] >= 5) ? fSlow11 : fSlow10)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow9 : fSlow8) : ((iZec10[i] >= 1) ? fSlow7 : fSlow6)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow3) ? ((iSlow14) ? fSlow15 : fZec2[i] + 0.5f * fZec7[i] * (fZec4[i] + fZec7[i] * (2.0f * fZec9[i] + 4.0f * fZec4[i] + fZec7[i] * (fZec11[i] + 3.0f * (fZec2[i] - fZec4[i]) - fZec9[i]) - (5.0f * fZec2[i] + fZec11[i])) - fZec9[i])) : ((iSlow4) ? fZec2[i] + 0.5f * fZec5[i] * (1.0f - std::cos(3.1415927f * fZec7[i])) : fZec2[i] + fZec7[i] * fZec5[i]))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = fZec12[i] > 0.01f;
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iRec0[i - 1] * iZec13[i] + 1), fSlow18));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Recursive loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec2_tmp[j4] = fRec2_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fRec2[i - 1] * float(iZec13[i]);
				fRec2[i] = fZec14[i] + (1.0f - fSlow17 * float(int(fSlow19 * (fZec14[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec2_perm[j5] = fRec2_tmp[vsize + j5];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec12[i];
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec16[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = std::max<int>(0, std::min<int>(int(fRec2[i]), 95999));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec16[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(ftbl0[iZec17[i]] * fZec12[i] + float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec16[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(ftbl1[iZec17[i]] * fZec12[i] + float(input1[i]) * fZec15[i]);
			}
		}
	}

};

#endif
