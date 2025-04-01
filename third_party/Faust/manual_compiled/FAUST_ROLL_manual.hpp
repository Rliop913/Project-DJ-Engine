/* ------------------------------------------------------------
name: "ROLL"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn RollFAUSTMan -scn RollMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __RollFAUSTMan_H__
#define  __RollFAUSTMan_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "rollMan.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS RollFAUSTMan
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


class RollFAUSTMan final : public RollMan {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	int iRec0_perm[4];
	float fConst2;
	float fRec1_perm[4];
	float ftbl0[96000];
	float ftbl1[96000];
	
 public:
	RollFAUSTMan() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn RollFAUSTMan -scn RollMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("filename", "ROLL.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "ROLL");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
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
	
	RollFAUSTMan* clone() {
		return new RollFAUSTMan();
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
		float fSlow0 = std::max<float>(0.0f, std::min<float>(1.0f, RollPower));
		int iSlow1 = fSlow0 > 0.01f;
		float fSlow2 = RollBpm;
		float fSlow3 = fConst1 / fSlow2;
		float fSlow4 = fSlow3 + -1.0f;
		int iRec0_tmp[68];
		int* iRec0 = &iRec0_tmp[4];
		float fSlow5 = float(iSlow1);
		float fZec0[64];
		float fSlow6 = fConst2 * fSlow2;
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow7 = 1.0f - fSlow0;
		int iZec1[64];
		int iZec2[64];
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
				iRec0_tmp[j0] = iRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iSlow1 * iRec0[i - 1] + 1), fSlow4));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec0_perm[j1] = iRec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow5 * fRec1[i - 1];
				fRec1[i] = fZec0[i] + (1.0f - fSlow3 * float(int(fSlow6 * (fZec0[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = std::max<int>(0, std::min<int>(int(fRec1[i]), 95999));
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec1[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(fSlow0 * ftbl0[iZec2[i]] + fSlow7 * float(input0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec1[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(fSlow0 * ftbl1[iZec2[i]] + fSlow7 * float(input1[i]));
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
				iRec0_tmp[j0] = iRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec0[i] = int(std::min<float>(float(iSlow1 * iRec0[i - 1] + 1), fSlow4));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				iRec0_perm[j1] = iRec0_tmp[vsize + j1];
			}
			/* Recursive loop 1 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = fSlow5 * fRec1[i - 1];
				fRec1[i] = fZec0[i] + (1.0f - fSlow3 * float(int(fSlow6 * (fZec0[i] + 1.0f))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec1[i] = std::max<int>(0, std::min<int>(iRec0[i], 95999));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec2[i] = std::max<int>(0, std::min<int>(int(fRec1[i]), 95999));
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl0[iZec1[i]] = float(input0[i]);
				output0[i] = FAUSTFLOAT(fSlow0 * ftbl0[iZec2[i]] + fSlow7 * float(input0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				ftbl1[iZec1[i]] = float(input1[i]);
				output1[i] = FAUSTFLOAT(fSlow0 * ftbl1[iZec2[i]] + fSlow7 * float(input1[i]));
			}
		}
	}

};

#endif
