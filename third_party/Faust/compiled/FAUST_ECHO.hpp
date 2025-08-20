/* ------------------------------------------------------------
name: "ECHO"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn EchoFAUST -scn Echo_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __EchoFAUST_H__
#define  __EchoFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "echo.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS EchoFAUST
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


class EchoFAUST final : public Echo_PDJE {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0[2097152];
	int fRec0_idx;
	int fRec0_idx_save;
	int iRec1_perm[4];
	float fRec2[2097152];
	int fRec2_idx;
	int fRec2_idx_save;
	
 public:
	EchoFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn EchoFAUST -scn Echo_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "ECHO.dsp");
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
		m->declare("misceffects.lib/echo:author", "Romain Michon");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.1");
		m->declare("name", "ECHO");
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
		fConst1 = 1e+01f * fConst0;
	}
	
	void instanceResetUserInterface() {
	}
	
	void instanceClear() {
		for (int l0 = 0; l0 < 2097152; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		fRec0_idx = 0;
		fRec0_idx_save = 0;
		for (int l1 = 0; l1 < 4; l1 = l1 + 1) {
			iRec1_perm[l1] = 0;
		}
		for (int l2 = 0; l2 < 2097152; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		fRec2_idx = 0;
		fRec2_idx_save = 0;
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
	
	EchoFAUST* clone() {
		return new EchoFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ECHO");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iSlow0 = int(std::min<float>(fConst1, std::max<float>(0.0f, fConst0 * EchoBps))) + 1;
		float fSlow1 = EchoFeedback;
		int iSlow2 = timerActive > 0;
		int iSlow3 = frames;
		int iRec1_tmp[36];
		int* iRec1 = &iRec1_tmp[4];
		int iSlow4 = selectInterpolator;
		int iSlow5 = iSlow4 >= 2;
		int iSlow6 = iSlow4 >= 1;
		float fSlow7 = 8.0f / float(iSlow3);
		float fZec0[32];
		int iZec1[32];
		float fSlow8 = v1;
		float fSlow9 = v2;
		float fSlow10 = v3;
		float fSlow11 = v4;
		float fSlow12 = v5;
		float fSlow13 = v6;
		float fSlow14 = v7;
		float fSlow15 = v8;
		float fZec2[32];
		int iZec3[32];
		float fZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fZec7[32];
		int iSlow16 = iSlow4 >= 3;
		int iZec8[32];
		float fZec9[32];
		int iZec10[32];
		float fZec11[32];
		float fSlow17 = vZero;
		float fZec12[32];
		float fZec13[32];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 32); vindex = vindex + 32) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
			/* Recursive loop 0 */
			/* Pre code */
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec1_tmp[j0] = iRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec1[i - 1] + 1));
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec1_perm[j1] = iRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow7 * float(iRec1[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = int(fZec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = iZec1[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec1[i] >= 4) ? ((iZec1[i] >= 6) ? ((iZec1[i] >= 7) ? fSlow15 : fSlow14) : ((iZec1[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec1[i] >= 2) ? ((iZec1[i] >= 3) ? fSlow11 : fSlow10) : ((iZec1[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow15 : fSlow14) : ((iZec3[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow11 : fSlow10) : ((iZec3[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec1[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = iZec1[i] + -1;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec1[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fZec4[i] - fZec2[i];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec0[i] - ((fZec0[i] == fZec6[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec6[i] : fZec6[i] + -1.0f));
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
				fZec12[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec2[i] + 0.5f * fZec7[i] * (fZec4[i] + fZec7[i] * (2.0f * fZec9[i] + 4.0f * fZec4[i] + fZec7[i] * (fZec11[i] + 3.0f * (fZec2[i] - fZec4[i]) - fZec9[i]) - (5.0f * fZec2[i] + fZec11[i])) - fZec9[i])) : ((iSlow6) ? fZec2[i] + 0.5f * fZec5[i] * (1.0f - std::cos(3.1415927f * fZec7[i])) : fZec2[i] + fZec7[i] * fZec5[i]))));
			}
			/* Recursive loop 14 */
			/* Pre code */
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[(i + fRec0_idx) & 2097151] = float(input0[i]) + fSlow1 * fRec0[(i + fRec0_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec0_idx_save = vsize;
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = 1.0f - fZec12[i];
			}
			/* Recursive loop 16 */
			/* Pre code */
			fRec2_idx = (fRec2_idx + fRec2_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[(i + fRec2_idx) & 2097151] = float(input1[i]) + fSlow1 * fRec2[(i + fRec2_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec2_idx_save = vsize;
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fRec0[(i + fRec0_idx) & 2097151] * fZec12[i] + float(input0[i]) * fZec13[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fRec2[(i + fRec2_idx) & 2097151] * fZec12[i] + float(input1[i]) * fZec13[i]);
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
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				iRec1_tmp[j0] = iRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec1[i] = ((iSlow2) ? 0 : std::min<int>(iSlow3, iRec1[i - 1] + 1));
			}
			/* Post code */
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec1_perm[j1] = iRec1_tmp[vsize + j1];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow7 * float(iRec1[i]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = int(fZec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = iZec1[i] + 1;
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec1[i] >= 4) ? ((iZec1[i] >= 6) ? ((iZec1[i] >= 7) ? fSlow15 : fSlow14) : ((iZec1[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec1[i] >= 2) ? ((iZec1[i] >= 3) ? fSlow11 : fSlow10) : ((iZec1[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow15 : fSlow14) : ((iZec3[i] >= 5) ? fSlow13 : fSlow12)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow11 : fSlow10) : ((iZec3[i] >= 1) ? fSlow9 : fSlow8)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = float(iZec1[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = iZec1[i] + -1;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec1[i] + 2;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = fZec4[i] - fZec2[i];
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec0[i] - ((fZec0[i] == fZec6[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec6[i] : fZec6[i] + -1.0f));
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
				fZec12[i] = std::max<float>(0.0f, std::min<float>(1.0f, ((iSlow5) ? ((iSlow16) ? fSlow17 : fZec2[i] + 0.5f * fZec7[i] * (fZec4[i] + fZec7[i] * (2.0f * fZec9[i] + 4.0f * fZec4[i] + fZec7[i] * (fZec11[i] + 3.0f * (fZec2[i] - fZec4[i]) - fZec9[i]) - (5.0f * fZec2[i] + fZec11[i])) - fZec9[i])) : ((iSlow6) ? fZec2[i] + 0.5f * fZec5[i] * (1.0f - std::cos(3.1415927f * fZec7[i])) : fZec2[i] + fZec7[i] * fZec5[i]))));
			}
			/* Recursive loop 14 */
			/* Pre code */
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[(i + fRec0_idx) & 2097151] = float(input0[i]) + fSlow1 * fRec0[(i + fRec0_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec0_idx_save = vsize;
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = 1.0f - fZec12[i];
			}
			/* Recursive loop 16 */
			/* Pre code */
			fRec2_idx = (fRec2_idx + fRec2_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec2[(i + fRec2_idx) & 2097151] = float(input1[i]) + fSlow1 * fRec2[(i + fRec2_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec2_idx_save = vsize;
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fRec0[(i + fRec0_idx) & 2097151] * fZec12[i] + float(input0[i]) * fZec13[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fRec2[(i + fRec2_idx) & 2097151] * fZec12[i] + float(input1[i]) * fZec13[i]);
			}
		}
	}

};

#endif
