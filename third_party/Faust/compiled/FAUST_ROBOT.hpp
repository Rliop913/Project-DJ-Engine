/* ------------------------------------------------------------
name: "ROBOT"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn RobotFAUST -scn Robot_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __RobotFAUST_H__
#define  __RobotFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "Faust_interpolate.hpp"
#include "robot.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS RobotFAUST
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


class RobotFAUST final : public Robot_PDJE {
	
 private:
	
	int iVec0_perm[4];
	int iRec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec1_perm[4];
	float ftbl0RobotFAUSTSIG0[65536];
	int iVec1[2];
	int iRec2[2];
	
 public:
	RobotFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn RobotFAUST -scn Robot_PDJE -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("filename", "ROBOT.dsp");
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
		m->declare("misceffects.lib/dryWetMixer:author", "David Braun, revised by Stéphane Letz");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.1");
		m->declare("name", "ROBOT");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
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
			ftbl0RobotFAUSTSIG0[i1_re0] = std::sin(9.58738e-05f * float(iRec2[0]));
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
			iRec0_perm[l1] = 0;
		}
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
	
	RobotFAUST* clone() {
		return new RobotFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ROBOT");
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
		float fSlow2 = fConst0 * float(robotFreq);
		float fZec0[32];
		float fZec1[32];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		int iSlow3 = selectInterpolator;
		int iSlow4 = iSlow3 >= 2;
		int iSlow5 = iSlow3 >= 1;
		float fSlow6 = 8.0f / float(iSlow1);
		float fZec2[32];
		int iZec3[32];
		float fSlow7 = v1;
		float fSlow8 = v2;
		float fSlow9 = v3;
		float fSlow10 = v4;
		float fSlow11 = v5;
		float fSlow12 = v6;
		float fSlow13 = v7;
		float fSlow14 = v8;
		float fZec4[32];
		int iZec5[32];
		float fZec6[32];
		float fZec7[32];
		float fZec8[32];
		float fZec9[32];
		int iSlow15 = iSlow3 >= 3;
		int iZec10[32];
		float fZec11[32];
		int iZec12[32];
		float fZec13[32];
		float fSlow16 = vZero;
		float fZec14[32];
		float fZec15[32];
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
				fZec2[i] = fSlow6 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow14 : fSlow13) : ((iZec3[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow10 : fSlow9) : ((iZec3[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow14 : fSlow13) : ((iZec5[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow10 : fSlow9) : ((iZec5[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec6[i] - fZec4[i];
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow14 : fSlow13) : ((iZec10[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow10 : fSlow9) : ((iZec10[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow14 : fSlow13) : ((iZec12[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow10 : fSlow9) : ((iZec12[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Recursive loop 14 */
			/* Pre code */
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
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec1_perm[j5] = fRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow4) ? ((iSlow15) ? fSlow16 : fZec4[i] + 0.5f * fZec9[i] * (fZec6[i] + fZec9[i] * (2.0f * fZec11[i] + 4.0f * fZec6[i] + fZec9[i] * (fZec13[i] + 3.0f * (fZec4[i] - fZec6[i]) - fZec11[i]) - (5.0f * fZec4[i] + fZec13[i])) - fZec11[i])) : ((iSlow5) ? fZec4[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec4[i] + fZec9[i] * fZec7[i]))));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec14[i] + ftbl0RobotFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] * fZec14[i];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec15[i]);
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
				fZec2[i] = fSlow6 * float(iRec0[i]);
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = int(fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = iZec3[i] + 1;
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec3[i] >= 4) ? ((iZec3[i] >= 6) ? ((iZec3[i] >= 7) ? fSlow14 : fSlow13) : ((iZec3[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec3[i] >= 2) ? ((iZec3[i] >= 3) ? fSlow10 : fSlow9) : ((iZec3[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((iZec5[i] >= 4) ? ((iZec5[i] >= 6) ? ((iZec5[i] >= 7) ? fSlow14 : fSlow13) : ((iZec5[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec5[i] >= 2) ? ((iZec5[i] >= 3) ? fSlow10 : fSlow9) : ((iZec5[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(iZec3[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = iZec3[i] + -1;
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec12[i] = iZec3[i] + 2;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fZec6[i] - fZec4[i];
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec2[i] - ((fZec2[i] == fZec8[i]) ? fZec2[i] : ((fZec2[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = ((iZec10[i] >= 4) ? ((iZec10[i] >= 6) ? ((iZec10[i] >= 7) ? fSlow14 : fSlow13) : ((iZec10[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec10[i] >= 2) ? ((iZec10[i] >= 3) ? fSlow10 : fSlow9) : ((iZec10[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((iZec12[i] >= 4) ? ((iZec12[i] >= 6) ? ((iZec12[i] >= 7) ? fSlow14 : fSlow13) : ((iZec12[i] >= 5) ? fSlow12 : fSlow11)) : ((iZec12[i] >= 2) ? ((iZec12[i] >= 3) ? fSlow10 : fSlow9) : ((iZec12[i] >= 1) ? fSlow8 : fSlow7)));
			}
			/* Recursive loop 14 */
			/* Pre code */
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
			for (int j5 = 0; j5 < 4; j5 = j5 + 1) {
				fRec1_perm[j5] = fRec1_tmp[vsize + j5];
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::min<float>(1.0f, std::max<float>(0.0f, ((iSlow4) ? ((iSlow15) ? fSlow16 : fZec4[i] + 0.5f * fZec9[i] * (fZec6[i] + fZec9[i] * (2.0f * fZec11[i] + 4.0f * fZec6[i] + fZec9[i] * (fZec13[i] + 3.0f * (fZec4[i] - fZec6[i]) - fZec11[i]) - (5.0f * fZec4[i] + fZec13[i])) - fZec11[i])) : ((iSlow5) ? fZec4[i] + 0.5f * fZec7[i] * (1.0f - std::cos(3.1415927f * fZec9[i])) : fZec4[i] + fZec9[i] * fZec7[i]))));
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = 1.0f - fZec14[i] + ftbl0RobotFAUSTSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] * fZec14[i];
			}
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec15[i]);
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec15[i]);
			}
		}
	}

};

#endif
