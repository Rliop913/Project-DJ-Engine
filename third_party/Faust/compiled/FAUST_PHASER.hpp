/* ------------------------------------------------------------
name: "PHASER"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn PhaserFAUST -scn Phaser_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __PhaserFAUST_H__
#define  __PhaserFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "phaser.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS PhaserFAUST
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

static float PhaserFAUST_faustpower2_f(float value) {
	return value * value;
}

class PhaserFAUST final : public Phaser_PDJE {
	
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
	PhaserFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn PhaserFAUST -scn Phaser_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
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
		fConst3 = PhaserFAUST_faustpower2_f(fConst2);
		fConst4 = 1.0f / fConst0;
		fConst5 = 2.0f * fConst2;
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 4; l0 = l0 + 1) {
			iVec0_perm[l0] = 0;
		}
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			iRec0_perm[l1] = 0;
		}
		for (int l2 = 0; l2 < 4; l2 = l2 + 1) {
			fRec8_perm[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 4; l3 = l3 + 1) {
			fRec9_perm[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 4; l4 = l4 + 1) {
			fRec7_perm[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4; l5 = l5 + 1) {
			fRec6_perm[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4; l6 = l6 + 1) {
			fRec5_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
			fRec4_perm[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec3_perm[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 4; l9 = l9 + 1) {
			fRec2_perm[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4; l10 = l10 + 1) {
			fRec1_perm[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4; l11 = l11 + 1) {
			fRec16_perm[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 4; l12 = l12 + 1) {
			fRec15_perm[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4; l13 = l13 + 1) {
			fRec14_perm[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4; l14 = l14 + 1) {
			fRec13_perm[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4; l15 = l15 + 1) {
			fRec12_perm[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 4; l16 = l16 + 1) {
			fRec11_perm[l16] = 0.0f;
		}
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
	
	PhaserFAUST* clone() {
		return new PhaserFAUST();
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
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		int iRec0_tmp[36];
		int* iRec0 = &iRec0_tmp[4];
		float fSlow2 = fConst1 * bps;
		float fSlow3 = std::cos(fSlow2);
		float fSlow4 = std::sin(fSlow2);
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fZec0[32];
		float fRec7_tmp[36];
		float* fRec7 = &fRec7_tmp[4];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		int iSlow5 = selectInterpolator;
		int iSlow6 = iSlow5 >= 2;
		int iSlow7 = iSlow5 >= 1;
		float fSlow8 = 8.0f / float(iSlow1);
		float fZec1[32];
		int iZec2[32];
		float fSlow9 = v1;
		float fSlow10 = v2;
		float fSlow11 = v3;
		float fSlow12 = v4;
		float fSlow13 = v5;
		float fSlow14 = v6;
		float fSlow15 = v7;
		float fSlow16 = v8;
		float fZec3[32];
		int iZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fZec7[32];
		float fZec8[32];
		int iSlow17 = iSlow5 >= 3;
		int iZec9[32];
		float fZec10[32];
		int iZec11[32];
		float fZec12[32];
		float fSlow18 = vZero;
		float fZec13[32];
		float fZec14[32];
		float fZec15[32];
		float fRec16_tmp[36];
		float* fRec16 = &fRec16_tmp[4];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		float fRec13_tmp[36];
		float* fRec13 = &fRec13_tmp[4];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
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
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec0[i - 1] + 1));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow8 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? fSlow16 : fSlow15) : ((iZec2[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? fSlow12 : fSlow11) : ((iZec2[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? fSlow16 : fSlow15) : ((iZec4[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? fSlow12 : fSlow11) : ((iZec4[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec9[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = iZec2[i] + 2;
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec9_tmp[j6] = fRec9_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fSlow4 * fRec9[i - 1] + fSlow3 * fRec8[i - 1];
				fRec9[i] = float(1 - iVec0[i - 1]) + fSlow3 * fRec9[i - 1] - fSlow4 * fRec8[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec8_perm[j5] = fRec8_tmp[vsize + j5];
			}
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec9_perm[j7] = fRec9_tmp[vsize + j7];
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = fZec5[i] - fZec3[i];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = ((iZec9[i] >= 4) ? ((iZec9[i] >= 6) ? ((iZec9[i] >= 7) ? fSlow16 : fSlow15) : ((iZec9[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec9[i] >= 2) ? ((iZec9[i] >= 3) ? fSlow12 : fSlow11) : ((iZec9[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((iZec11[i] >= 4) ? ((iZec11[i] >= 6) ? ((iZec11[i] >= 7) ? fSlow16 : fSlow15) : ((iZec11[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec11[i] >= 2) ? ((iZec11[i] >= 3) ? fSlow12 : fSlow11) : ((iZec11[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec8[i]) + 3141.5928f));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow6) ? ((iSlow17) ? fSlow18 : fZec3[i] + 0.5f * fZec8[i] * (fZec5[i] + fZec8[i] * (2.0f * fZec10[i] + 4.0f * fZec5[i] + fZec8[i] * (fZec12[i] + 3.0f * (fZec3[i] - fZec5[i]) - fZec10[i]) - (5.0f * fZec3[i] + fZec12[i])) - fZec10[i])) : ((iSlow7) ? fZec3[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec3[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Recursive loop 18 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec7_tmp[j8] = fRec7_perm[j8];
			}
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec6_tmp[j10] = fRec6_perm[j10];
			}
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec4_tmp[j14] = fRec4_perm[j14];
			}
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec3_tmp[j16] = fRec3_perm[j16];
			}
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec2_tmp[j18] = fRec2_perm[j18];
			}
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
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec7_perm[j9] = fRec7_tmp[vsize + j9];
			}
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec6_perm[j11] = fRec6_tmp[vsize + j11];
			}
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec4_perm[j15] = fRec4_tmp[vsize + j15];
			}
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec3_perm[j17] = fRec3_tmp[vsize + j17];
			}
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec2_perm[j19] = fRec2_tmp[vsize + j19];
			}
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec1_perm[j21] = fRec1_tmp[vsize + j21];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = 1.0f - 0.5f * fZec13[i];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec15_tmp[j24] = fRec15_perm[j24];
			}
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec14_tmp[j26] = fRec14_perm[j26];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec11_tmp[j32] = fRec11_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec10_tmp[j34] = fRec10_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = float(input1[i]) + 0.3f * fRec10[i - 1] + fConst5 * fRec16[i - 1] * fZec15[i] - fConst3 * fRec16[i - 2];
				fRec15[i] = fRec16[i - 2] + fConst3 * (fRec16[i] - fRec15[i - 2]) - fConst5 * fZec15[i] * (fRec16[i - 1] - fRec15[i - 1]);
				fRec14[i] = fRec15[i - 2] + fConst3 * (fRec15[i] - fRec14[i - 2]) - fConst5 * fZec15[i] * (fRec15[i - 1] - fRec14[i - 1]);
				fRec13[i] = fRec14[i - 2] + fConst3 * (fRec14[i] - fRec13[i - 2]) - fConst5 * fZec15[i] * (fRec14[i - 1] - fRec13[i - 1]);
				fRec12[i] = fConst3 * (fRec13[i] - fRec12[i - 2]) + fRec13[i - 2] + fConst5 * fZec15[i] * (fRec12[i - 1] - fRec13[i - 1]);
				fRec11[i] = fRec12[i - 2] + fConst3 * (fRec12[i] - fRec11[i - 2]) - fConst5 * fZec15[i] * (fRec12[i - 1] - fRec11[i - 1]);
				fRec10[i] = fRec11[i - 2] + fConst3 * fRec11[i] - fConst5 * fZec15[i] * fRec11[i - 1];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec16_perm[j23] = fRec16_tmp[vsize + j23];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec15_perm[j25] = fRec15_tmp[vsize + j25];
			}
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec14_perm[j27] = fRec14_tmp[vsize + j27];
			}
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec11_perm[j33] = fRec11_tmp[vsize + j33];
			}
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec10_perm[j35] = fRec10_tmp[vsize + j35];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec14[i] + 0.5f * fRec1[i] * fZec13[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec14[i] + 0.5f * fRec10[i] * fZec13[i]);
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
				iRec0_tmp[j2] = iRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = ((iSlow0) ? 0 : std::min<int>(iSlow1, iRec0[i - 1] + 1));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				iRec0_perm[j3] = iRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = fSlow8 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = int(fZec1[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = iZec2[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = ((iZec2[i] >= 4) ? ((iZec2[i] >= 6) ? ((iZec2[i] >= 7) ? fSlow16 : fSlow15) : ((iZec2[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec2[i] >= 2) ? ((iZec2[i] >= 3) ? fSlow12 : fSlow11) : ((iZec2[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = ((iZec4[i] >= 4) ? ((iZec4[i] >= 6) ? ((iZec4[i] >= 7) ? fSlow16 : fSlow15) : ((iZec4[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec4[i] >= 2) ? ((iZec4[i] >= 3) ? fSlow12 : fSlow11) : ((iZec4[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = float(iZec2[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec9[i] = iZec2[i] + -1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec11[i] = iZec2[i] + 2;
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j4 = 0; j4 < 4; j4 = j4 + 1) {
				fRec8_tmp[j4] = fRec8_perm[j4];
			}
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec9_tmp[j6] = fRec9_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[i] = fSlow4 * fRec9[i - 1] + fSlow3 * fRec8[i - 1];
				fRec9[i] = float(1 - iVec0[i - 1]) + fSlow3 * fRec9[i - 1] - fSlow4 * fRec8[i - 1];
			}
			/* Post code */
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec8_perm[j5] = fRec8_tmp[vsize + j5];
			}
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec9_perm[j7] = fRec9_tmp[vsize + j7];
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = fZec5[i] - fZec3[i];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = fZec1[i] - ((fZec1[i] == fZec7[i]) ? fZec1[i] : ((fZec1[i] >= 0.0f) ? fZec7[i] : fZec7[i] + -1.0f));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec10[i] = ((iZec9[i] >= 4) ? ((iZec9[i] >= 6) ? ((iZec9[i] >= 7) ? fSlow16 : fSlow15) : ((iZec9[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec9[i] >= 2) ? ((iZec9[i] >= 3) ? fSlow12 : fSlow11) : ((iZec9[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((iZec11[i] >= 4) ? ((iZec11[i] >= 6) ? ((iZec11[i] >= 7) ? fSlow16 : fSlow15) : ((iZec11[i] >= 5) ? fSlow14 : fSlow13)) : ((iZec11[i] >= 2) ? ((iZec11[i] >= 3) ? fSlow12 : fSlow11) : ((iZec11[i] >= 1) ? fSlow10 : fSlow9)));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec8[i]) + 3141.5928f));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow6) ? ((iSlow17) ? fSlow18 : fZec3[i] + 0.5f * fZec8[i] * (fZec5[i] + fZec8[i] * (2.0f * fZec10[i] + 4.0f * fZec5[i] + fZec8[i] * (fZec12[i] + 3.0f * (fZec3[i] - fZec5[i]) - fZec10[i]) - (5.0f * fZec3[i] + fZec12[i])) - fZec10[i])) : ((iSlow7) ? fZec3[i] + 0.5f * fZec6[i] * (1.0f - std::cos(3.1415927f * fZec8[i])) : fZec3[i] + fZec8[i] * fZec6[i]))));
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = std::cos(fConst4 * (14137.167f * (1.0f - fRec9[i]) + 3141.5928f));
			}
			/* Recursive loop 18 */
			/* Pre code */
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec7_tmp[j8] = fRec7_perm[j8];
			}
			for (int j10 = 0; j10 < 4; j10 = j10 + 1) {
				fRec6_tmp[j10] = fRec6_perm[j10];
			}
			for (int j12 = 0; j12 < 4; j12 = j12 + 1) {
				fRec5_tmp[j12] = fRec5_perm[j12];
			}
			for (int j14 = 0; j14 < 4; j14 = j14 + 1) {
				fRec4_tmp[j14] = fRec4_perm[j14];
			}
			for (int j16 = 0; j16 < 4; j16 = j16 + 1) {
				fRec3_tmp[j16] = fRec3_perm[j16];
			}
			for (int j18 = 0; j18 < 4; j18 = j18 + 1) {
				fRec2_tmp[j18] = fRec2_perm[j18];
			}
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
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec7_perm[j9] = fRec7_tmp[vsize + j9];
			}
			for (int j11 = 0; j11 < 4; j11 = j11 + 1) {
				fRec6_perm[j11] = fRec6_tmp[vsize + j11];
			}
			for (int j13 = 0; j13 < 4; j13 = j13 + 1) {
				fRec5_perm[j13] = fRec5_tmp[vsize + j13];
			}
			for (int j15 = 0; j15 < 4; j15 = j15 + 1) {
				fRec4_perm[j15] = fRec4_tmp[vsize + j15];
			}
			for (int j17 = 0; j17 < 4; j17 = j17 + 1) {
				fRec3_perm[j17] = fRec3_tmp[vsize + j17];
			}
			for (int j19 = 0; j19 < 4; j19 = j19 + 1) {
				fRec2_perm[j19] = fRec2_tmp[vsize + j19];
			}
			for (int j21 = 0; j21 < 4; j21 = j21 + 1) {
				fRec1_perm[j21] = fRec1_tmp[vsize + j21];
			}
			/* Vectorizable loop 19 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = 1.0f - 0.5f * fZec13[i];
			}
			/* Recursive loop 20 */
			/* Pre code */
			for (int j22 = 0; j22 < 4; j22 = j22 + 1) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			for (int j24 = 0; j24 < 4; j24 = j24 + 1) {
				fRec15_tmp[j24] = fRec15_perm[j24];
			}
			for (int j26 = 0; j26 < 4; j26 = j26 + 1) {
				fRec14_tmp[j26] = fRec14_perm[j26];
			}
			for (int j28 = 0; j28 < 4; j28 = j28 + 1) {
				fRec13_tmp[j28] = fRec13_perm[j28];
			}
			for (int j30 = 0; j30 < 4; j30 = j30 + 1) {
				fRec12_tmp[j30] = fRec12_perm[j30];
			}
			for (int j32 = 0; j32 < 4; j32 = j32 + 1) {
				fRec11_tmp[j32] = fRec11_perm[j32];
			}
			for (int j34 = 0; j34 < 4; j34 = j34 + 1) {
				fRec10_tmp[j34] = fRec10_perm[j34];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec16[i] = float(input1[i]) + 0.3f * fRec10[i - 1] + fConst5 * fRec16[i - 1] * fZec15[i] - fConst3 * fRec16[i - 2];
				fRec15[i] = fRec16[i - 2] + fConst3 * (fRec16[i] - fRec15[i - 2]) - fConst5 * fZec15[i] * (fRec16[i - 1] - fRec15[i - 1]);
				fRec14[i] = fRec15[i - 2] + fConst3 * (fRec15[i] - fRec14[i - 2]) - fConst5 * fZec15[i] * (fRec15[i - 1] - fRec14[i - 1]);
				fRec13[i] = fRec14[i - 2] + fConst3 * (fRec14[i] - fRec13[i - 2]) - fConst5 * fZec15[i] * (fRec14[i - 1] - fRec13[i - 1]);
				fRec12[i] = fConst3 * (fRec13[i] - fRec12[i - 2]) + fRec13[i - 2] + fConst5 * fZec15[i] * (fRec12[i - 1] - fRec13[i - 1]);
				fRec11[i] = fRec12[i - 2] + fConst3 * (fRec12[i] - fRec11[i - 2]) - fConst5 * fZec15[i] * (fRec12[i - 1] - fRec11[i - 1]);
				fRec10[i] = fRec11[i - 2] + fConst3 * fRec11[i] - fConst5 * fZec15[i] * fRec11[i - 1];
			}
			/* Post code */
			for (int j23 = 0; j23 < 4; j23 = j23 + 1) {
				fRec16_perm[j23] = fRec16_tmp[vsize + j23];
			}
			for (int j25 = 0; j25 < 4; j25 = j25 + 1) {
				fRec15_perm[j25] = fRec15_tmp[vsize + j25];
			}
			for (int j27 = 0; j27 < 4; j27 = j27 + 1) {
				fRec14_perm[j27] = fRec14_tmp[vsize + j27];
			}
			for (int j29 = 0; j29 < 4; j29 = j29 + 1) {
				fRec13_perm[j29] = fRec13_tmp[vsize + j29];
			}
			for (int j31 = 0; j31 < 4; j31 = j31 + 1) {
				fRec12_perm[j31] = fRec12_tmp[vsize + j31];
			}
			for (int j33 = 0; j33 < 4; j33 = j33 + 1) {
				fRec11_perm[j33] = fRec11_tmp[vsize + j33];
			}
			for (int j35 = 0; j35 < 4; j35 = j35 + 1) {
				fRec10_perm[j35] = fRec10_tmp[vsize + j35];
			}
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec14[i] + 0.5f * fRec1[i] * fZec13[i]);
			}
			/* Vectorizable loop 22 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec14[i] + 0.5f * fRec10[i] * fZec13[i]);
			}
		}
	}

};

#endif
