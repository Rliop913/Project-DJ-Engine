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
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	int iRec0_perm[4];
	float fConst2;
	float fRec1_perm[4];
	int iRec2_perm[4];
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
			iRec0_perm[l0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec1_perm[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			iRec2_perm[l2] = 0;
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
		int iSlow0 = float(RollSwitch) < 0.5f;
		float fSlow1 = RollBpm;
		float fSlow2 = fConst1 / fSlow1;
		float fSlow3 = fSlow2 + -1.0f;
		int iRec0_tmp[68];
		int* iRec0 = &iRec0_tmp[4];
		float fSlow4 = float(iSlow0);
		float fZec0[64];
		float fSlow5 = fConst2 * fSlow1;
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		int iSlow6 = timerActive > 0;
		int iSlow7 = frames;
		int iRec2_tmp[68];
		int* iRec2 = &iRec2_tmp[4];
		int iSlow8 = selectInterpolator;
		int iSlow9 = iSlow8 >= 2;
		int iSlow10 = iSlow8 >= 1;
		float fSlow11 = 8.0f / float(iSlow7);
		float fZec1[64];
		int iZec2[64];
		int iSlow12 = v1;
		int iSlow13 = v2;
		int iSlow14 = v3;
		int iSlow15 = v4;
		int iSlow16 = v5;
		int iSlow17 = v6;
		int iSlow18 = v7;
		int iSlow19 = v8;
		int iZec3[64];
		int iZec4[64];
		int iZec5[64];
		float fZec6[64];
		float fZec7[64];
		float fZec8[64];
		float fZec9[64];
		int iSlow20 = iSlow8 >= 3;
		int iZec10[64];
		float fZec11[64];
		int iZec12[64];
		int iZec13[64];
		float fZec14[64];
		float fSlow21 = float(vZero);
		float fZec15[64];
		float fZec16[64];
		int iZec17[64];
		int iZec18[64];
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
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				iRec2_tmp[j4] = iRec2_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec2[i] = ((iSlow6) ? 0 : std::min<int>(iSlow7, iRec2[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				iRec2_perm[j5] = iRec2_tmp[vsize + j5];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow11 * float(iRec2[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? iSlow19 : iSlow18) : ((iZec2[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? iSlow15 : iSlow14) : ((iZec2[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? iSlow19 : iSlow18) : ((iZec4[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? iSlow15 : iSlow14) : ((iZec4[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec2[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? iSlow19 : iSlow18) : ((iZec12[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? iSlow15 : iSlow14) : ((iZec12[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec5[i] - iZec3[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow19 : iSlow18) : ((iZec10[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow15 : iSlow14) : ((iZec10[i] >= 1) ? iSlow13 : iSlow12))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = float(iZec13[i]);
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec0_tmp[j0] = iRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iSlow0 * iRec0[i - 1] + 1), fSlow3));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec0_perm[j1] = iRec0_tmp[vsize + j1];
			}
			/* Recursive loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow4 * fRec1[i - 1];
				fRec1[i] = fZec0[i] + (1.0f - fSlow2 * float(int(fSlow5 * (fZec0[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow9) ? ((iSlow20) ? fSlow21 : fZec9[i] + 0.5f * fZec8[i] * (float(iZec5[i] - iZec13[i]) + fZec8[i] * (2.0f * fZec14[i] + 4.0f * float(iZec5[i]) + fZec8[i] * (fZec11[i] + 3.0f * float(iZec3[i] - iZec5[i]) - fZec14[i]) - (5.0f * fZec9[i] + fZec11[i])))) : ((iSlow10) ? fZec9[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec9[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - fZec15[i];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = std::max<int>(0, std::min<int>(int(fRec1[i]), 95999));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec17[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(ftbl0[iZec18[i]] * fZec15[i] + float(input0[i]) * fZec16[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec17[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(ftbl1[iZec18[i]] * fZec15[i] + float(input1[i]) * fZec16[i]);
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
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				iRec2_tmp[j4] = iRec2_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec2[i] = ((iSlow6) ? 0 : std::min<int>(iSlow7, iRec2[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				iRec2_perm[j5] = iRec2_tmp[vsize + j5];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow11 * float(iRec2[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? iSlow19 : iSlow18) : ((iZec2[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? iSlow15 : iSlow14) : ((iZec2[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? iSlow19 : iSlow18) : ((iZec4[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? iSlow15 : iSlow14) : ((iZec4[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec2[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? iSlow19 : iSlow18) : ((iZec12[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? iSlow15 : iSlow14) : ((iZec12[i] >= 1) ? iSlow13 : iSlow12)));
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec5[i] - iZec3[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow19 : iSlow18) : ((iZec10[i] >= 5) ? iSlow17 : iSlow16)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow15 : iSlow14) : ((iZec10[i] >= 1) ? iSlow13 : iSlow12))));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = float(iZec13[i]);
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec0_tmp[j0] = iRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iSlow0 * iRec0[i - 1] + 1), fSlow3));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec0_perm[j1] = iRec0_tmp[vsize + j1];
			}
			/* Recursive loop 16 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow4 * fRec1[i - 1];
				fRec1[i] = fZec0[i] + (1.0f - fSlow2 * float(int(fSlow5 * (fZec0[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow9) ? ((iSlow20) ? fSlow21 : fZec9[i] + 0.5f * fZec8[i] * (float(iZec5[i] - iZec13[i]) + fZec8[i] * (2.0f * fZec14[i] + 4.0f * float(iZec5[i]) + fZec8[i] * (fZec11[i] + 3.0f * float(iZec3[i] - iZec5[i]) - fZec14[i]) - (5.0f * fZec9[i] + fZec11[i])))) : ((iSlow10) ? fZec9[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec9[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 1.0f - fZec15[i];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = std::max<int>(0, std::min<int>(int(fRec1[i]), 95999));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec17[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(ftbl0[iZec18[i]] * fZec15[i] + float(input0[i]) * fZec16[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec17[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(ftbl1[iZec18[i]] * fZec15[i] + float(input1[i]) * fZec16[i]);
			}
		}
	}

};

#endif
