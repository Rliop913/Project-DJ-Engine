/* ------------------------------------------------------------
name: "ROBOT"
Code generated with Faust 2.81.2 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn RobotFAUSTMan -scn RobotMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __RobotFAUSTMan_H__
#define  __RobotFAUSTMan_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "robotMan.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS RobotFAUSTMan
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


class RobotFAUSTMan final : public RobotMan {
	
 private:
	
	int iVec0_perm[4];
	int fSampleRate;
	float fConst0;
	float fRec0_perm[4];
	float ftbl0RobotFAUSTManSIG0[65536];
	int iVec1[2];
	int iRec1[2];
	
 public:
	RobotFAUSTMan() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn RobotFAUSTMan -scn RobotMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("filename", "ROBOT.dsp");
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
		for (int l2_re0 = 0; l2_re0 < 2; l2_re0 = l2_re0 + 1) {
			iVec1[l2_re0] = 0;
		}
		for (int l3_re0 = 0; l3_re0 < 2; l3_re0 = l3_re0 + 1) {
			iRec1[l3_re0] = 0;
		}
		for (int i1_re0 = 0; i1_re0 < 65536; i1_re0 = i1_re0 + 1) {
			iVec1[0] = 1;
			iRec1[0] = (iVec1[1] + iRec1[1]) % 65536;
			ftbl0RobotFAUSTManSIG0[i1_re0] = std::sin(9.58738e-05f * float(iRec1[0]));
			iVec1[1] = iVec1[0];
			iRec1[1] = iRec1[0];
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
			fRec0_perm[l1] = 0.0f;
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
	
	RobotFAUSTMan* clone() {
		return new RobotFAUSTMan();
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
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		float fSlow0 = fConst0 * float(robotFreq);
		float fZec0[32];
		float fZec1[32];
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		float fSlow1 = std::min<float>(1.0f, std::max<float>(0.0f, RobotDryWet));
		float fSlow2 = 1.0f - fSlow1;
		float fZec2[32];
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
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec0[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec0[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow2 + fSlow1 * ftbl0RobotFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec2[i]);
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
				fRec0_tmp[j2] = fRec0_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = ((1 - iVec0[i - 1]) ? 0.0f : fSlow0 + fRec0[i - 1]);
				fZec1[i] = float(int(fZec0[i]));
				fRec0[i] = fZec0[i] - ((fZec0[i] == fZec1[i]) ? fZec0[i] : ((fZec0[i] >= 0.0f) ? fZec1[i] : fZec1[i] + -1.0f));
			}
			/* Post code */
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec0_perm[j3] = fRec0_tmp[vsize + j3];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = fSlow2 + fSlow1 * ftbl0RobotFAUSTManSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[i]), 65535))];
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * fZec2[i]);
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * fZec2[i]);
			}
		}
	}

};

#endif
