/* ------------------------------------------------------------
name: "COMPRESSOR"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64
------------------------------------------------------------ */

#ifndef  __FilterFAUST_H__
#define  __FilterFAUST_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS FilterFAUST
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

static float FilterFAUST_faustpower2_f(float value) {
	return value * value;
}

class FilterFAUST final : public FilterVal {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fRec0_perm[4];
	float fRec1_perm[4];
	
 public:
	FilterFAUST() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-lang cpp -light -it -nvi -ct 1 -mapp -cn FilterFAUST -scn FilterVal -es 1 -exp10 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0 -vec -lv 0 -vs 64");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_compression_gain_mono:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono:license", "GPLv3");
		m->declare("compressors.lib/peak_compression_gain_mono_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono_db:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("filename", "COMPRESSOR.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("name", "COMPRESSOR");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
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
			fRec0_perm[l0] = 0.0f;
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
	
	FilterFAUST* clone() {
		return new FilterFAUST();
	}
	
	int getSampleRate() {
		return fSampleRate;
	}
	
	void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("COMPRESSOR");
		ui_interface->closeBox();
	}
	
	void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fZec0[64];
		int iSlow0 = releaseMS;
		int iSlow1 = float(std::abs(iSlow0)) < 1.1920929e-07f;
		float fSlow2 = ((iSlow1) ? 0.0f : std::exp(-(fConst0 / ((iSlow1) ? 1.0f : float(iSlow0)))));
		int iSlow3 = attackMS;
		int iSlow4 = float(std::abs(iSlow3)) < 1.1920929e-07f;
		float fSlow5 = ((iSlow4) ? 0.0f : std::exp(-(fConst0 / ((iSlow4) ? 1.0f : float(iSlow3)))));
		float fZec1[64];
		float fRec0_tmp[68];
		float* fRec0 = &fRec0_tmp[4];
		float fSlow6 = float(kneeDB);
		float fSlow7 = 0.5f * fSlow6;
		float fSlow8 = float(threshDB);
		float fSlow9 = fSlow8 + fSlow7;
		float fZec2[64];
		float fSlow10 = fSlow8 - fSlow7;
		int iZec3[64];
		float fZec4[64];
		float fSlow11 = 0.5f / std::max<float>(1.1920929e-07f, fSlow6);
		float fSlow12 = 0.05f * strength;
		float fZec5[64];
		float fZec6[64];
		float fRec1_tmp[68];
		float* fRec1 = &fRec1_tmp[4];
		float fZec7[64];
		int iZec8[64];
		float fZec9[64];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; vindex <= (count - 64); vindex = vindex + 64) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 64;
			/* Vectorizable loop 0 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::fabs(float(input0[i]));
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = std::fabs(float(input1[i]));
			}
			/* Recursive loop 2 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((fZec0[i] > fRec0[i - 1]) ? fSlow5 : fSlow2);
				fRec0[i] = fZec0[i] * (1.0f - fZec1[i]) + fRec0[i - 1] * fZec1[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec0_perm[j1] = fRec0_tmp[vsize + j1];
			}
			/* Recursive loop 3 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec5[i] > fRec1[i - 1]) ? fSlow5 : fSlow2);
				fRec1[i] = fZec5[i] * (1.0f - fZec6[i]) + fRec1[i - 1] * fZec6[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec1[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = (fZec2[i] > fSlow10) + (fZec2[i] > fSlow9);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = fZec2[i] - fSlow8;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = (fZec7[i] > fSlow10) + (fZec7[i] > fSlow9);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec7[i] - fSlow8;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * std::pow(1e+01f, -(fSlow12 * std::max<float>(0.0f, ((iZec3[i] == 0) ? 0.0f : ((iZec3[i] == 1) ? fSlow11 * FilterFAUST_faustpower2_f(fSlow7 + fZec4[i]) : fZec4[i]))))));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * std::pow(1e+01f, -(fSlow12 * std::max<float>(0.0f, ((iZec8[i] == 0) ? 0.0f : ((iZec8[i] == 1) ? fSlow11 * FilterFAUST_faustpower2_f(fSlow7 + fZec9[i]) : fZec9[i]))))));
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
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec0[i] = std::fabs(float(input0[i]));
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec5[i] = std::fabs(float(input1[i]));
			}
			/* Recursive loop 2 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; j0 < 4; j0 = j0 + 1) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((fZec0[i] > fRec0[i - 1]) ? fSlow5 : fSlow2);
				fRec0[i] = fZec0[i] * (1.0f - fZec1[i]) + fRec0[i - 1] * fZec1[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; j1 < 4; j1 = j1 + 1) {
				fRec0_perm[j1] = fRec0_tmp[vsize + j1];
			}
			/* Recursive loop 3 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; j2 < 4; j2 = j2 + 1) {
				fRec1_tmp[j2] = fRec1_perm[j2];
			}
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = ((fZec5[i] > fRec1[i - 1]) ? fSlow5 : fSlow2);
				fRec1[i] = fZec5[i] * (1.0f - fZec6[i]) + fRec1[i - 1] * fZec6[i];
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; j3 < 4; j3 = j3 + 1) {
				fRec1_perm[j3] = fRec1_tmp[vsize + j3];
			}
			/* Vectorizable loop 4 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec0[i]));
			}
			/* Vectorizable loop 5 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec1[i]));
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec3[i] = (fZec2[i] > fSlow10) + (fZec2[i] > fSlow9);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = fZec2[i] - fSlow8;
			}
			/* Vectorizable loop 8 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				iZec8[i] = (fZec7[i] > fSlow10) + (fZec7[i] > fSlow9);
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = fZec7[i] - fSlow8;
			}
			/* Vectorizable loop 10 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(float(input0[i]) * std::pow(1e+01f, -(fSlow12 * std::max<float>(0.0f, ((iZec3[i] == 0) ? 0.0f : ((iZec3[i] == 1) ? fSlow11 * FilterFAUST_faustpower2_f(fSlow7 + fZec4[i]) : fZec4[i]))))));
			}
			/* Vectorizable loop 11 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(float(input1[i]) * std::pow(1e+01f, -(fSlow12 * std::max<float>(0.0f, ((iZec8[i] == 0) ? 0.0f : ((iZec8[i] == 1) ? fSlow11 * FilterFAUST_faustpower2_f(fSlow7 + fZec9[i]) : fZec9[i]))))));
			}
		}
	}

};

#endif
