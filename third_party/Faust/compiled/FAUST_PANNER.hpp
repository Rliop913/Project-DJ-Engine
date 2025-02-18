/* ------------------------------------------------------------
name: "PANNER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn PannerFAUST -scn Panner_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __PannerFAUST_H__
#define  __PannerFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "panner.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS PannerFAUST
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


class PannerFAUST final : public Panner_PDJE {
	
 private:
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec0_perm[4];
	int iRec1_perm[4];
	float ftbl0PannerFAUSTSIG0[65536];
	int iVec1[2];
	int iRec2[2];
	
 public:
	PannerFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn PannerFAUST -scn Panner_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "PANNER.dsp");
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
		m->declare("name", "PANNER");
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
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3_re0 = 0; l3_re0 < 2; l3_re0 = l3_re0 + 1) {
			iVec1[l3_re0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4_re0 = 0; l4_re0 < 2; l4_re0 = l4_re0 + 1) {
			iRec2[l4_re0] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i1_re0 = 0; i1_re0 < 65536; i1_re0 = i1_re0 + 1) {
			iVec1[0] = 1;
			iRec2[0] = (iVec1[1] + iRec2[1]) % 65536;
			ftbl0PannerFAUSTSIG0[i1_re0] = std::sin(9.58738e-05f * float(iRec2[0]));
			iVec1[1] = iVec1[0];
			iRec2[1] = iRec2[0];
		}
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
			fRec0_perm[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			iRec1_perm[l2] = 0;
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
	
	PannerFAUST* clone() {
		return new PannerFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("PANNER");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iVec0_tmp[68];
		int* iVec0 = &iVec0_tmp[4];
		float fSlow0 = fConst0 * bps;
		float fZec0[64];
		float fZec1[64];
		float fRec0_tmp[68];
		float* fRec0 = &fRec0_tmp[4];
		int iSlow1 = timerActive > 0;
		int iSlow2 = frames;
		int iRec1_tmp[68];
		int* iRec1 = &iRec1_tmp[4];
		int iSlow3 = selectInterpolator;
		int iSlow4 = iSlow3 >= 2;
		int iSlow5 = iSlow3 >= 1;
		float fSlow6 = 8.0f / float(iSlow2);
		float fZec2[64];
		int iZec3[64];
		float fSlow7 = v1;
		float fSlow8 = v2;
		float fSlow9 = v3;
		float fSlow10 = v4;
		float fSlow11 = v5;
		float fSlow12 = v6;
		float fSlow13 = v7;
		float fSlow14 = v8;
		float fZec4[64];
		int iZec5[64];
		float fZec6[64];
		float fZec7[64];
		float fZec8[64];
		float fZec9[64];
		int iSlow15 = iSlow3 >= 3;
		int iZec10[64];
		float fZec11[64];
		int iZec12[64];
		float fZec13[64];
		float fSlow16 = vZero;
		float fZec14[64];
		float fZec15[64];
		float fSlow17 = PGain;
		float fZec16[64];
		float fZec17[64];
		int iZec18[64];
		float fZec19[64];
		int iZec20[64];
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
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				iRec1_tmp[j4] = iRec1_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow1) ? 0 : std::min<int>(iSlow2, iRec1[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				iRec1_perm[j5] = iRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow6 * float(iRec1[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Recursive loop 5 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec0[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec0[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow14 : fSlow13) : ((iZec3[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow10 : fSlow9) : ((iZec3[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow14 : fSlow13) : ((iZec5[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow10 : fSlow9) : ((iZec5[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec6[i] - fZec4[i];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow14 : fSlow13) : ((iZec10[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow10 : fSlow9) : ((iZec10[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow14 : fSlow13) : ((iZec12[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow10 : fSlow9) : ((iZec12[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = fSlow17 * ftbl0PannerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow4) ? ((iSlow15) ? fSlow16 : fZec4[i] + 0.5f * fZec9[i] * (fZec6[i] + fZec9[i] * (2.0f * fZec11[i] + 4.0f * fZec6[i] + fZec9[i] * (fZec13[i] + 3.0f * (fZec4[i] - fZec6[i]) - fZec11[i]) - (5.0f * fZec4[i] + fZec13[i])) - fZec11[i])) : ((iSlow5) ? fZec4[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec4[i] + fZec9[i] * fZec7[i]))));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = float(int(-(fZec16[i])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = float(int(fZec16[i]));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec14[i];
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = int(((-(fZec16[i]) == fZec17[i]) ? -(fZec16[i]) : ((-(fZec16[i]) >= 0.0f) ? fZec17[i] + 1.0f : fZec17[i])));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec20[i] = int(((fZec16[i] == fZec19[i]) ? fZec16[i] : ((fZec16[i] >= 0.0f) ? fZec19[i] + 1.0f : fZec19[i])));
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iZec18[i]) ? float(input0[i]) : ((iZec18[i]) ? 0.0f : float(input0[i])) * (1.0f - fZec16[i])) * fZec14[i] + float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fZec14[i] * ((iZec20[i]) ? float(input1[i]) : ((iZec20[i]) ? 0.0f : float(input1[i])) * (fZec16[i] + 1.0f)) + float(input1[i]) * fZec15[i]);
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
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				iRec1_tmp[j4] = iRec1_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow1) ? 0 : std::min<int>(iSlow2, iRec1[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				iRec1_perm[j5] = iRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow6 * float(iRec1[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Recursive loop 5 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec0[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec0[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow14 : fSlow13) : ((iZec3[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow10 : fSlow9) : ((iZec3[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow14 : fSlow13) : ((iZec5[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow10 : fSlow9) : ((iZec5[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec6[i] - fZec4[i];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow14 : fSlow13) : ((iZec10[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow10 : fSlow9) : ((iZec10[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow14 : fSlow13) : ((iZec12[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow10 : fSlow9) : ((iZec12[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = fSlow17 * ftbl0PannerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow4) ? ((iSlow15) ? fSlow16 : fZec4[i] + 0.5f * fZec9[i] * (fZec6[i] + fZec9[i] * (2.0f * fZec11[i] + 4.0f * fZec6[i] + fZec9[i] * (fZec13[i] + 3.0f * (fZec4[i] - fZec6[i]) - fZec11[i]) - (5.0f * fZec4[i] + fZec13[i])) - fZec11[i])) : ((iSlow5) ? fZec4[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec4[i] + fZec9[i] * fZec7[i]))));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = float(int(-(fZec16[i])));
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = float(int(fZec16[i]));
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec14[i];
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = int(((-(fZec16[i]) == fZec17[i]) ? -(fZec16[i]) : ((-(fZec16[i]) >= 0.0f) ? fZec17[i] + 1.0f : fZec17[i])));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec20[i] = int(((fZec16[i] == fZec19[i]) ? fZec16[i] : ((fZec16[i] >= 0.0f) ? fZec19[i] + 1.0f : fZec19[i])));
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(((iZec18[i]) ? float(input0[i]) : ((iZec18[i]) ? 0.0f : float(input0[i])) * (1.0f - fZec16[i])) * fZec14[i] + float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fZec14[i] * ((iZec20[i]) ? float(input1[i]) : ((iZec20[i]) ? 0.0f : float(input1[i])) * (fZec16[i] + 1.0f)) + float(input1[i]) * fZec15[i]);
			}
		}
	}

};

#endif
