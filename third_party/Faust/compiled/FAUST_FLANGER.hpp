/* ------------------------------------------------------------
name: "FLANGER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUST -scn Flanger_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __FlangerFAUST_H__
#define  __FlangerFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "flanger.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS FlangerFAUST
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


class FlangerFAUST final : public Flanger_PDJE {
	
 private:
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec1_perm[4];
	float fYec0[128];
	int fYec0_idx;
	int fYec0_idx_save;
	float ftbl0FlangerFAUSTSIG0[65536];
	float fRec0_perm[4];
	int iRec3_perm[4];
	float fRec5_perm[4];
	float fYec1[128];
	int fYec1_idx;
	int fYec1_idx_save;
	float fRec4_perm[4];
	int iVec1[2];
	int iRec2[2];
	
 public:
	FlangerFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUST -scn Flanger_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "FLANGER.dsp");
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
		m->declare("name", "FLANGER");
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
			ftbl0FlangerFAUSTSIG0[i1_re0] = std::sin(9.58738e-05f * float(iRec2[0]));
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
			fRec1_perm[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; l2 < 128; l2 = l2 + 1) {
			fYec0[l2] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec0_perm[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			iRec3_perm[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec5_perm[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; l8 < 128; l8 = l8 + 1) {
			fYec1[l8] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec4_perm[l9] = 0.0f;
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
	
	FlangerFAUST* clone() {
		return new FlangerFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("FLANGER");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iVec0_tmp[68];
		int* iVec0 = &iVec0_tmp[4];
		int iZec0[64];
		float fSlow0 = bps;
		float fSlow1 = fConst0 * (fSlow0 + 0.1f);
		float fZec1[64];
		float fZec2[64];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fZec3[64];
		int iZec4[64];
		float fZec5[64];
		float fZec6[64];
		float fRec0_tmp[68];
		float* fRec0 = &fRec0_tmp[4];
		int iSlow2 = timerActive > 0;
		int iSlow3 = frames;
		int iRec3_tmp[68];
		int* iRec3 = &iRec3_tmp[4];
		int iSlow4 = selectInterpolator;
		int iSlow5 = iSlow4 >= 2;
		int iSlow6 = iSlow4 >= 1;
		float fSlow7 = 8.0f / float(iSlow3);
		float fZec7[64];
		int iZec8[64];
		int iSlow8 = v1;
		int iSlow9 = v2;
		int iSlow10 = v3;
		int iSlow11 = v4;
		int iSlow12 = v5;
		int iSlow13 = v6;
		int iSlow14 = v7;
		int iSlow15 = v8;
		int iZec9[64];
		int iZec10[64];
		int iZec11[64];
		float fZec12[64];
		float fZec13[64];
		float fZec14[64];
		float fZec15[64];
		int iSlow16 = iSlow4 >= 3;
		int iZec16[64];
		float fZec17[64];
		int iZec18[64];
		int iZec19[64];
		float fZec20[64];
		float fSlow17 = float(vZero);
		float fZec21[64];
		float fZec22[64];
		float fSlow18 = fConst0 * fSlow0;
		float fZec23[64];
		float fZec24[64];
		float fRec5_tmp[68];
		float* fRec5 = &fRec5_tmp[4];
		float fZec25[64];
		int iZec26[64];
		float fZec27[64];
		float fZec28[64];
		float fRec4_tmp[68];
		float* fRec4 = &fRec4_tmp[4];
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
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec3_tmp[j6] = iRec3_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec3[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec3[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec3_perm[j7] = iRec3_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fSlow7 * float(iRec3[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((iZec0[i]) ? 0.0f : fSlow1 + fRec1[i - 1]);
				fZec2[i] = float(int(fZec1[i]));
				fRec1[i] = fZec1[i] - ((fZec1[i] == fZec2[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec2[i] : fZec2[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(fZec7[i]);
			}
			/* Recursive loop 6 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = ((iZec0[i]) ? 0.0f : fSlow18 + fRec5[i - 1]);
				fZec24[i] = float(int(fZec23[i]));
				fRec5[i] = fZec23[i] - ((fZec23[i] == fZec24[i]) ? fZec23[i] : ((fZec23[i] >= 0.0f) ? fZec24[i] : fZec24[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec5_perm[j9] = fRec5_tmp[vsize + j9];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec8[i] + 1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = iZec8[i] + -1;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? iSlow15 : iSlow14) : ((iZec8[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? iSlow11 : iSlow10) : ((iZec8[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow15 : iSlow14) : ((iZec10[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow11 : iSlow10) : ((iZec10[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = float(iZec8[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec16[i] = iZec8[i] + 2;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec19[i] = ((iZec18[i] >= 4) ? ((iZec18[i] >= 6) ? ((iZec18[i] >= 7) ? iSlow15 : iSlow14) : ((iZec18[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec18[i] >= 2) ? ((iZec18[i] >= 3) ? iSlow11 : iSlow10) : ((iZec18[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec26[i] = int(fZec25[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = float(iZec11[i] - iZec9[i]);
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec7[i] - ((fZec7[i] == fZec13[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec13[i] : fZec13[i] + -1.0f));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = float(iZec9[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = float(((iZec16[i] >= 4) ? ((iZec16[i] >= 6) ? ((iZec16[i] >= 7) ? iSlow15 : iSlow14) : ((iZec16[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec16[i] >= 2) ? ((iZec16[i] >= 3) ? iSlow11 : iSlow10) : ((iZec16[i] >= 1) ? iSlow9 : iSlow8))));
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = float(iZec19[i]);
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = float(iZec26[i]);
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 26 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = std::max<float>(0.0f, std::min<float>(1.0f, 0.01f * ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec15[i] + 0.5f * fZec14[i] * (float(iZec11[i] - iZec19[i]) + fZec14[i] * (2.0f * fZec20[i] + 4.0f * float(iZec11[i]) + fZec14[i] * (fZec17[i] + 3.0f * float(iZec9[i] - iZec11[i]) - fZec20[i]) - (5.0f * fZec15[i] + fZec17[i])))) : ((iSlow6) ? fZec15[i] + 0.5f * fZec12[i] * (1.0f - std::cos(3.1415927f * fZec14[i])) : fZec15[i] + fZec14[i] * fZec12[i]))));
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = ((fZec25[i] == fZec27[i]) ? fZec25[i] : ((fZec25[i] >= 0.0f) ? fZec27[i] : fZec27[i] + -1.0f));
			}
			/* Recursive loop 28 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 127;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 127] = 0.8f * fRec0[i - 1] - float(input0[i]);
				fRec0[i] = fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i]))) & 127] * (fZec6[i] + (1.0f - fZec3[i])) + (fZec3[i] - fZec6[i]) * fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i] + 1))) & 127];
			}
			/* Post code */
			fYec0_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Vectorizable loop 29 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = 1.0f - fZec21[i];
			}
			/* Recursive loop 30 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 127;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec4_tmp[j10] = fRec4_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 127] = 0.8f * fRec4[i - 1] - float(input1[i]);
				fRec4[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec26[i]))) & 127] * (fZec28[i] + (1.0f - fZec25[i])) + (fZec25[i] - fZec28[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec26[i] + 1))) & 127];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec4_perm[j11] = fRec4_tmp[vsize + j11];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(0.5f * (float(input0[i]) + fRec0[i]) * fZec21[i] + float(input0[i]) * fZec22[i]);
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(0.5f * fZec21[i] * (float(input1[i]) + fRec4[i]) + float(input1[i]) * fZec22[i]);
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
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec3_tmp[j6] = iRec3_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec3[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec3[i - 1] + 1));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec3_perm[j7] = iRec3_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fSlow7 * float(iRec3[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((iZec0[i]) ? 0.0f : fSlow1 + fRec1[i - 1]);
				fZec2[i] = float(int(fZec1[i]));
				fRec1[i] = fZec1[i] - ((fZec1[i] == fZec2[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec2[i] : fZec2[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(fZec7[i]);
			}
			/* Recursive loop 6 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = ((iZec0[i]) ? 0.0f : fSlow18 + fRec5[i - 1]);
				fZec24[i] = float(int(fZec23[i]));
				fRec5[i] = fZec23[i] - ((fZec23[i] == fZec24[i]) ? fZec23[i] : ((fZec23[i] >= 0.0f) ? fZec24[i] : fZec24[i] + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec5_perm[j9] = fRec5_tmp[vsize + j9];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec8[i] + 1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec18[i] = iZec8[i] + -1;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? iSlow15 : iSlow14) : ((iZec8[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? iSlow11 : iSlow10) : ((iZec8[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? iSlow15 : iSlow14) : ((iZec10[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? iSlow11 : iSlow10) : ((iZec10[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = float(iZec8[i]);
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec16[i] = iZec8[i] + 2;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec19[i] = ((iZec18[i] >= 4) ? ((iZec18[i] >= 6) ? ((iZec18[i] >= 7) ? iSlow15 : iSlow14) : ((iZec18[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec18[i] >= 2) ? ((iZec18[i] >= 3) ? iSlow11 : iSlow10) : ((iZec18[i] >= 1) ? iSlow9 : iSlow8)));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec26[i] = int(fZec25[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = float(iZec11[i] - iZec9[i]);
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec7[i] - ((fZec7[i] == fZec13[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec13[i] : fZec13[i] + -1.0f));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = float(iZec9[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec17[i] = float(((iZec16[i] >= 4) ? ((iZec16[i] >= 6) ? ((iZec16[i] >= 7) ? iSlow15 : iSlow14) : ((iZec16[i] >= 5) ? iSlow13 : iSlow12)) : ((iZec16[i] >= 2) ? ((iZec16[i] >= 3) ? iSlow11 : iSlow10) : ((iZec16[i] >= 1) ? iSlow9 : iSlow8))));
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = float(iZec19[i]);
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = float(iZec26[i]);
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 26 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = std::max<float>(0.0f, std::min<float>(1.0f, 0.01f * ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec15[i] + 0.5f * fZec14[i] * (float(iZec11[i] - iZec19[i]) + fZec14[i] * (2.0f * fZec20[i] + 4.0f * float(iZec11[i]) + fZec14[i] * (fZec17[i] + 3.0f * float(iZec9[i] - iZec11[i]) - fZec20[i]) - (5.0f * fZec15[i] + fZec17[i])))) : ((iSlow6) ? fZec15[i] + 0.5f * fZec12[i] * (1.0f - std::cos(3.1415927f * fZec14[i])) : fZec15[i] + fZec14[i] * fZec12[i]))));
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = ((fZec25[i] == fZec27[i]) ? fZec25[i] : ((fZec25[i] >= 0.0f) ? fZec27[i] : fZec27[i] + -1.0f));
			}
			/* Recursive loop 28 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 127;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 127] = 0.8f * fRec0[i - 1] - float(input0[i]);
				fRec0[i] = fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i]))) & 127] * (fZec6[i] + (1.0f - fZec3[i])) + (fZec3[i] - fZec6[i]) * fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i] + 1))) & 127];
			}
			/* Post code */
			fYec0_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Vectorizable loop 29 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = 1.0f - fZec21[i];
			}
			/* Recursive loop 30 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 127;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec4_tmp[j10] = fRec4_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 127] = 0.8f * fRec4[i - 1] - float(input1[i]);
				fRec4[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec26[i]))) & 127] * (fZec28[i] + (1.0f - fZec25[i])) + (fZec25[i] - fZec28[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec26[i] + 1))) & 127];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec4_perm[j11] = fRec4_tmp[vsize + j11];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(0.5f * (float(input0[i]) + fRec0[i]) * fZec21[i] + float(input0[i]) * fZec22[i]);
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(0.5f * fZec21[i] * (float(input1[i]) + fRec4[i]) + float(input1[i]) * fZec22[i]);
			}
		}
	}

};

#endif
