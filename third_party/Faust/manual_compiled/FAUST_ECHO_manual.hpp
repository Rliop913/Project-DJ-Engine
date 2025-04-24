/* ------------------------------------------------------------
name: "ECHO"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn EchoFAUSTMan -scn EchoMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32
------------------------------------------------------------ */

#ifndef  __EchoFAUSTMan_H__
#define  __EchoFAUSTMan_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include "echoMan.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS EchoFAUSTMan
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


class EchoFAUSTMan final : public EchoMan {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0[2097152];
	int fRec0_idx;
	int fRec0_idx_save;
	float fRec1[2097152];
	int fRec1_idx;
	int fRec1_idx_save;
	
 public:
	EchoFAUSTMan() {
	}
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn EchoFAUSTMan -scn EchoMan -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 32");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "ECHO.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("misceffects.lib/echo:author", "Romain Michon");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.0");
		m->declare("name", "ECHO");
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
		for (int l1 = 0; l1 < 2097152; l1 = l1 + 1) {
			fRec1[l1] = 0.0f;
		}
		fRec1_idx = 0;
		fRec1_idx_save = 0;
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
	
	EchoFAUSTMan* clone() {
		return new EchoFAUSTMan();
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
		float fSlow2 = std::max<float>(0.0f, std::min<float>(1.0f, EchoDryWet));
		float fSlow3 = 1.0f - fSlow2;
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
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[(i + fRec0_idx) & 2097151] = float(input0[i]) + fSlow1 * fRec0[(i + fRec0_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec0_idx_save = vsize;
			/* Recursive loop 1 */
			/* Pre code */
			fRec1_idx = (fRec1_idx + fRec1_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[(i + fRec1_idx) & 2097151] = float(input1[i]) + fSlow1 * fRec1[(i + fRec1_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec1_idx_save = vsize;
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow2 * fRec0[(i + fRec0_idx) & 2097151] + fSlow3 * float(input0[i]));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow2 * fRec1[(i + fRec1_idx) & 2097151] + fSlow3 * float(input1[i]));
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
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec0[(i + fRec0_idx) & 2097151] = float(input0[i]) + fSlow1 * fRec0[(i + fRec0_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec0_idx_save = vsize;
			/* Recursive loop 1 */
			/* Pre code */
			fRec1_idx = (fRec1_idx + fRec1_idx_save) & 2097151;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[(i + fRec1_idx) & 2097151] = float(input1[i]) + fSlow1 * fRec1[(i + fRec1_idx - iSlow0) & 2097151];
			}
			/* Post code */
			fRec1_idx_save = vsize;
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(fSlow2 * fRec0[(i + fRec0_idx) & 2097151] + fSlow3 * float(input0[i]));
			}
			/* Vectorizable loop 3 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(fSlow2 * fRec1[(i + fRec1_idx) & 2097151] + fSlow3 * float(input1[i]));
			}
		}
	}

};

#endif
