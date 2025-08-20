/* ------------------------------------------------------------
name: "FLANGER"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUSTMan -scn FlangerMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __FlangerFAUSTMan_H__
#define  __FlangerFAUSTMan_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "flangerMan.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS FlangerFAUSTMan
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


class FlangerFAUSTMan final : public FlangerMan {
	
 private:
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec1_perm[4];
	float fYec0[64];
	int fYec0_idx;
	int fYec0_idx_save;
	float ftbl0FlangerFAUSTManSIG0[65536];
	float fRec0_perm[4];
	float fRec4_perm[4];
	float fYec1[64];
	int fYec1_idx;
	int fYec1_idx_save;
	float fRec3_perm[4];
	int iVec1[2];
	int iRec2[2];
	
 public:
	FlangerFAUSTMan() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FlangerFAUSTMan -scn FlangerMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "FLANGER.dsp");
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
			ftbl0FlangerFAUSTManSIG0[i1_re0] = std::sin(9.58738e-05f * float(iRec2[0]));
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
			fRec4_perm[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 64; l7 = l7 + 1) {
			fYec1[l7] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		for (int l8 = 0; l8 < 4; l8 = l8 + 1) {
			fRec3_perm[l8] = 0.0f;
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
	
	FlangerFAUSTMan* clone() {
		return new FlangerFAUSTMan();
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
		float fSlow2 = std::max<float>(0.0f, std::min<float>(1.0f, FlangerDryWet));
		float fSlow3 = 1.0f - fSlow2;
		float fSlow4 = 0.5f * fSlow2;
		float fSlow5 = fConst0 * fSlow0;
		float fZec7[32];
		float fZec8[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fZec9[32];
		int iZec10[32];
		float fZec11[32];
		float fZec12[32];
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
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
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 2 */
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
			/* Recursive loop 3 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = ((iZec0[i]) ? 0.0f : fSlow5 + fRec4[i - 1]);
				fZec8[i] = float(int(fZec7[i]));
				fRec4[i] = fZec7[i] - ((fZec7[i] == fZec8[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec4_perm[j7] = fRec4_tmp[vsize + j7];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = 1e+01f * (ftbl0FlangerFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec4[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = int(fZec9[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(iZec10[i]);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((fZec9[i] == fZec11[i]) ? fZec9[i] : ((fZec9[i] >= 0.0f) ? fZec11[i] : fZec11[i] + -1.0f));
			}
			/* Recursive loop 12 */
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
			/* Recursive loop 13 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 63;
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec3_tmp[j8] = fRec3_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 63] = 0.8f * fRec3[i - 1] - float(input1[i]);
				fRec3[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec10[i]))) & 63] * (fZec12[i] + (1.0f - fZec9[i])) + (fZec9[i] - fZec12[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec10[i] + 1))) & 63];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec3_perm[j9] = fRec3_tmp[vsize + j9];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow4 * (float(input0[i]) + fRec0[i]) + fSlow3 * float(input0[i]));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow4 * (float(input1[i]) + fRec3[i]) + fSlow3 * float(input1[i]));
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
			/* Vectorizable loop 1 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[i - 1];
			}
			/* Recursive loop 2 */
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
			/* Recursive loop 3 */
			/* Pre code */
			for (int j6 = 0; j6 < 4; j6 = j6 + 1) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = ((iZec0[i]) ? 0.0f : fSlow5 + fRec4[i - 1]);
				fZec8[i] = float(int(fZec7[i]));
				fRec4[i] = fZec7[i] - ((fZec7[i] == fZec8[i]) ? fZec7[i] : ((fZec7[i] >= 0.0f) ? fZec8[i] : fZec8[i] + -1.0f));
			}
			/* Post code */
			for (int j7 = 0; j7 < 4; j7 = j7 + 1) {
				fRec4_perm[j7] = fRec4_tmp[vsize + j7];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = 1e+01f * (ftbl0FlangerFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = 1e+01f * (ftbl0FlangerFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec4[i]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec4[i] = int(fZec3[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = int(fZec9[i]);
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = float(iZec4[i]);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = float(iZec10[i]);
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec3[i] == fZec5[i]) ? fZec3[i] : ((fZec3[i] >= 0.0f) ? fZec5[i] : fZec5[i] + -1.0f));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = ((fZec9[i] == fZec11[i]) ? fZec9[i] : ((fZec9[i] >= 0.0f) ? fZec11[i] : fZec11[i] + -1.0f));
			}
			/* Recursive loop 12 */
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
			/* Recursive loop 13 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 63;
			for (int j8 = 0; j8 < 4; j8 = j8 + 1) {
				fRec3_tmp[j8] = fRec3_perm[j8];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 63] = 0.8f * fRec3[i - 1] - float(input1[i]);
				fRec3[i] = fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec10[i]))) & 63] * (fZec12[i] + (1.0f - fZec9[i])) + (fZec9[i] - fZec12[i]) * fYec1[(i + fYec1_idx - std::min<int>(21, std::max<int>(0, iZec10[i] + 1))) & 63];
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j9 = 0; j9 < 4; j9 = j9 + 1) {
				fRec3_perm[j9] = fRec3_tmp[vsize + j9];
			}
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow4 * (float(input0[i]) + fRec0[i]) + fSlow3 * float(input0[i]));
			}
			/* Vectorizable loop 15 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow4 * (float(input1[i]) + fRec3[i]) + fSlow3 * float(input1[i]));
			}
		}
	}

};

#endif
