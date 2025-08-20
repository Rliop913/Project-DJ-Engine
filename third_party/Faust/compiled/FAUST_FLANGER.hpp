/* ------------------------------------------------------------
name: "FLANGER"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUST -scn Flanger_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
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
	float fYec0[64];
	int fYec0_idx;
	int fYec0_idx_save;
	float ftbl0FlangerFAUSTSIG0[65536];
	float fRec0_perm[4];
	int iRec3_perm[4];
	float fRec5_perm[4];
	float fYec1[64];
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
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUST -scn Flanger_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
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
		m->declare("interpolators.lib/version", "1.4.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "FLANGER");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.6.0");
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
		for (int l3_re0 = 0; l3_re0 < 2; l3_re0 = l3_re0 + 1) {
			iVec1[l3_re0] = 0;
		}
		for (int l4_re0 = 0; l4_re0 < 2; l4_re0 = l4_re0 + 1) {
			iRec2[l4_re0] = 0;
		}
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
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iVec0_perm[l0] = 0;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			fRec1_perm[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 64; l2 = l2 + 1) {
			fYec0[l2] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec0_perm[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			iRec3_perm[l6] = 0;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec5_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 64; l8 = l8 + 1) {
			fYec1[l8] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
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
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		int iZec0[32];
		float fSlow0 = bps;
		float fSlow1 = fConst0 * (fSlow0 + 0.1f);
		float fZec1[32];
		float fZec2[32];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fZec3[32];
		int iZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		int iSlow2 = timerActive > 0;
		int iSlow3 = frames;
		int iRec3_tmp[36];
		int* iRec3 = &iRec3_tmp[4];
		int iSlow4 = selectInterpolator;
		int iSlow5 = iSlow4 >= 2;
		int iSlow6 = iSlow4 >= 1;
		float fSlow7 = 8.0f / float(iSlow3);
		float fZec7[32];
		int iZec8[32];
		float fSlow8 = v1;
		float fSlow9 = v2;
		float fSlow10 = v3;
		float fSlow11 = v4;
		float fSlow12 = v5;
		float fSlow13 = v6;
		float fSlow14 = v7;
		float fSlow15 = v8;
		float fZec9[32];
		int iZec10[32];
		float fZec11[32];
		float fZec12[32];
		float fZec13[32];
		float fZec14[32];
		int iSlow16 = iSlow4 >= 3;
		int iZec15[32];
		float fZec16[32];
		int iZec17[32];
		float fZec18[32];
		float fSlow17 = vZero;
		float fZec19[32];
		float fZec20[32];
		float fSlow18 = fConst0 * fSlow0;
		float fZec21[32];
		float fZec22[32];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fZec23[32];
		int iZec24[32];
		float fZec25[32];
		float fZec26[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
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
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec3_tmp[j6] = iRec3_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec3[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec3[i - 1] + 1));
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec3_perm[j7] = iRec3_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fSlow7 * float(iRec3[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
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
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(fZec7[i]);
			}
			/* Recursive loop 6 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = ((iZec0[i]) ? 0.0f : fSlow18 + fRec5[i - 1]);
				fZec22[i] = float(int(fZec21[i]));
				fRec5[i] = fZec21[i] - ((fZec21[i] == fZec22[i]) ? fZec21[i] : ((fZec21[i] >= 0.0f) ? fZec22[i] : fZec22[i] + -1.0f));
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec5_perm[j9] = fRec5_tmp[vsize + j9];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec8[i] + 1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? fSlow15 : fSlow14) : ((iZec8[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? fSlow11 : fSlow10) : ((iZec8[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow15 : fSlow14) : ((iZec10[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow11 : fSlow10) : ((iZec10[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = float(iZec8[i]);
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec15[i] = iZec8[i] + -1;
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = iZec8[i] + 2;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec24[i] = int(fZec23[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = fZec11[i] - fZec9[i];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec7[i] - ((fZec7[i] == fZec13[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec13[i] : fZec13[i] + -1.0f));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = ((iZec15[i] >= 4) ? ((iZec15[i] >= 6) ? ((iZec15[i] >= 7) ? fSlow15 : fSlow14) : ((iZec15[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec15[i] >= 2) ? ((iZec15[i] >= 3) ? fSlow11 : fSlow10) : ((iZec15[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = ((iZec17[i] >= 4) ? ((iZec17[i] >= 6) ? ((iZec17[i] >= 7) ? fSlow15 : fSlow14) : ((iZec17[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec17[i] >= 2) ? ((iZec17[i] >= 3) ? fSlow11 : fSlow10) : ((iZec17[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = float(iZec24[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec9[i] + 0.5f * fZec14[i] * (fZec11[i] + fZec14[i] * (2.0f * fZec16[i] + 4.0f * fZec11[i] + fZec14[i] * (fZec18[i] + 3.0f * (fZec9[i] - fZec11[i]) - fZec16[i]) - (5.0f * fZec9[i] + fZec18[i])) - fZec16[i])) : ((iSlow6) ? fZec9[i] + 0.5f * fZec12[i] * (1.0f - std::cos(3.1415927f * fZec14[i])) : fZec9[i] + fZec14[i] * fZec12[i]))));
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = ((fZec23[i] == fZec25[i]) ? fZec23[i] : ((fZec23[i] >= 0.0f) ? fZec25[i] : fZec25[i] + -1.0f));
			}
			/* Recursive loop 26 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 63;
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 63] = 0.8f * fRec0[i - 1] - float(input0[i]);
				fRec0[i] = fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i]))) & 63] * (fZec6[i] + (1.0f - fZec3[i])) + (fZec3[i] - fZec6[i]) * fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i] + 1))) & 63];
			}
			/* Post code */
			fYec0_idx_save = vsize;
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = 1.0f - fZec19[i];
			}
			/* Recursive loop 28 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 63;
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec4_tmp[j10] = fRec4_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 63] = 0.8f * fRec4[i - 1] - float(input1[i]);
				fRec4[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec24[i]))) & 63] * (fZec26[i] + (1.0f - fZec23[i])) + (fZec23[i] - fZec26[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec24[i] + 1))) & 63];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec4_perm[j11] = fRec4_tmp[vsize + j11];
			}
			/* Vectorizable loop 29 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(0.5f * (float(input0[i]) + fRec0[i]) * fZec19[i] + float(input0[i]) * fZec20[i]);
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(0.5f * fZec19[i] * (float(input1[i]) + fRec4[i]) + float(input1[i]) * fZec20[i]);
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
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				iRec3_tmp[j6] = iRec3_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec3[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec3[i - 1] + 1));
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				iRec3_perm[j7] = iRec3_tmp[vsize + j7];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fSlow7 * float(iRec3[i]);
			}
			/* Recursive loop 4 */
			/* Pre code */
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
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = int(fZec7[i]);
			}
			/* Recursive loop 6 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = ((iZec0[i]) ? 0.0f : fSlow18 + fRec5[i - 1]);
				fZec22[i] = float(int(fZec21[i]));
				fRec5[i] = fZec21[i] - ((fZec21[i] == fZec22[i]) ? fZec21[i] : ((fZec21[i] >= 0.0f) ? fZec22[i] : fZec22[i] + -1.0f));
			}
			/* Post code */
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec5_perm[j9] = fRec5_tmp[vsize + j9];
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec8[i] + 1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec23[i] = 1e+01f * (ftbl0FlangerFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = ((iZec8[i] >= 4) ? ((iZec8[i] >= 6) ? ((iZec8[i] >= 7) ? fSlow15 : fSlow14) : ((iZec8[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec8[i] >= 2) ? ((iZec8[i] >= 3) ? fSlow11 : fSlow10) : ((iZec8[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow15 : fSlow14) : ((iZec10[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow11 : fSlow10) : ((iZec10[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = float(iZec8[i]);
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec15[i] = iZec8[i] + -1;
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = iZec8[i] + 2;
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec24[i] = int(fZec23[i]);
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = fZec11[i] - fZec9[i];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = fZec7[i] - ((fZec7[i] == fZec13[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec13[i] : fZec13[i] + -1.0f));
			}
			/* Vectorizable loop 20 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = ((iZec15[i] >= 4) ? ((iZec15[i] >= 6) ? ((iZec15[i] >= 7) ? fSlow15 : fSlow14) : ((iZec15[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec15[i] >= 2) ? ((iZec15[i] >= 3) ? fSlow11 : fSlow10) : ((iZec15[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = ((iZec17[i] >= 4) ? ((iZec17[i] >= 6) ? ((iZec17[i] >= 7) ? fSlow15 : fSlow14) : ((iZec17[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec17[i] >= 2) ? ((iZec17[i] >= 3) ? fSlow11 : fSlow10) : ((iZec17[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = float(iZec24[i]);
			}
			/* Vectorizable loop 23 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec9[i] + 0.5f * fZec14[i] * (fZec11[i] + fZec14[i] * (2.0f * fZec16[i] + 4.0f * fZec11[i] + fZec14[i] * (fZec18[i] + 3.0f * (fZec9[i] - fZec11[i]) - fZec16[i]) - (5.0f * fZec9[i] + fZec18[i])) - fZec16[i])) : ((iSlow6) ? fZec9[i] + 0.5f * fZec12[i] * (1.0f - std::cos(3.1415927f * fZec14[i])) : fZec9[i] + fZec14[i] * fZec12[i]))));
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = ((fZec23[i] == fZec25[i]) ? fZec23[i] : ((fZec23[i] >= 0.0f) ? fZec25[i] : fZec25[i] + -1.0f));
			}
			/* Recursive loop 26 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 63;
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec0_tmp[j4] = fRec0_perm[j4];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 63] = 0.8f * fRec0[i - 1] - float(input0[i]);
				fRec0[i] = fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i]))) & 63] * (fZec6[i] + (1.0f - fZec3[i])) + (fZec3[i] - fZec6[i]) * fYec0[(i + fYec0_idx - std::min<int>(21, std::max<int>(0, iZec4[i] + 1))) & 63];
			}
			/* Post code */
			fYec0_idx_save = vsize;
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec0_perm[j5] = fRec0_tmp[vsize + j5];
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = 1.0f - fZec19[i];
			}
			/* Recursive loop 28 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 63;
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec4_tmp[j10] = fRec4_perm[j10];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 63] = 0.8f * fRec4[i - 1] - float(input1[i]);
				fRec4[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec24[i]))) & 63] * (fZec26[i] + (1.0f - fZec23[i])) + (fZec23[i] - fZec26[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec24[i] + 1))) & 63];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec4_perm[j11] = fRec4_tmp[vsize + j11];
			}
			/* Vectorizable loop 29 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(0.5f * (float(input0[i]) + fRec0[i]) * fZec19[i] + float(input0[i]) * fZec20[i]);
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(0.5f * fZec19[i] * (float(input1[i]) + fRec4[i]) + float(input1[i]) * fZec20[i]);
			}
		}
	}

};

#endif
