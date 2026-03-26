#define _USE_MATH_DEFINES
#include <math.h>

typedef struct complex_t {
  float real, imag;
} complex;

typedef struct pairs_t {
  unsigned int first, second;
} pairs;

inline float window_func(const int index,
                         const int window_size) {
  float normalized_index = (float) index;
  normalized_index /= ((float) (window_size - 1));
  float angle = 2.0f * M_PI * normalized_index;
  return 0.5f * (1.0f - cos(angle));
}

inline float hanning_window(const int index,
                            const int windowSize) {
  float angle = 2.0 * M_PI * index / ((float) (windowSize - 1));
  return 0.5 * (1.0 - cos(angle));
}

inline float hamming_window(const int index,
                            const int windowSize) {
  float angle = 2.0 * M_PI * index / ((float) (windowSize - 1));
  return 0.54 - (0.46 * cos(angle));
}

inline float blackman_window(const int index,
                             const int windowSize) {
  float Fangle = 2.0 * M_PI * index / ((float) (windowSize));
  float Sangle = Fangle * 2.0;
  return 0.42 - 0.5 * cos(Fangle) + 0.08 * cos(Sangle);
}

inline float nuttall_window(const int index,
                            const int windowSize) {
  float Fangle = 2.0 * M_PI * index / ((float) (windowSize));
  float Sangle = Fangle * 2.0;
  float Tangle = Fangle * 3.0;
  return 0.355768 - 0.487396 * cos(Fangle) + 0.144232 * cos(Sangle) - 0.012604 * cos(
    Tangle
  );
}

inline float blackman_nuttall_window(const int index,
                                     const int windowSize) {
  float Fangle = 2.0 * M_PI * index / ((float) (windowSize));
  float Sangle = Fangle * 2.0;
  float Tangle = Fangle * 3.0;
  return 0.3635819 - 0.4891775 * cos(Fangle) + 0.1365995 * cos(Sangle) - 0.0106411 * cos(
    Tangle
  );
}

inline float blackman_harris_window(const int index,
                                    const int windowSize) {
  float Fangle = 2.0 * M_PI * index / ((float) (windowSize));
  float Sangle = Fangle * 2.0;
  float Tangle = Fangle * 3.0;
  return 0.35875 - 0.48829 * cos(Fangle) + 0.14128 * cos(Sangle) - 0.01168 * cos(
    Tangle
  );
}

inline float flatTop_window(const int index,
                            const int windowSize) {
  float Fangle = 2.0 * M_PI * index / ((float) (windowSize));
  float Sangle = Fangle * 2.0;
  float Tangle = Fangle * 3.0;
  float FFangle = Fangle * 4.0;
  return 0.21557895 - 0.41663158 * cos(Fangle) + 0.277263158 * cos(Sangle) - 0.083578947 * cos(
    Tangle
  ) + 0.006947368 * cos(FFangle);
}

inline float gaussian_window(const int index,
                             const int windowSize,
                             const float sigma) {
  const int HWinSize = windowSize >> 1;
  float angle = ((float) (index - HWinSize)) / ((float) (HWinSize * sigma));
  angle *= angle;
  angle *= -0.5;
  return exp(angle);
}

pairs indexer(const unsigned int firstMaximumID,
              const int powed_stage) {
  pairs temp;
  temp.first = firstMaximumID + (firstMaximumID & (~(powed_stage - 1)));
  temp.second = temp.first + powed_stage;
  return temp;
}

inline int segmentK(const int lsave,
                    const int segmentSize,
                    const int HwindowSize) {
  return ((lsave % segmentSize) * HwindowSize) / segmentSize;
}

complex twiddle(int k,
                int windowSize) {
  complex temp;
  float angle = -2.0 * M_PI * ((float) k / (float) windowSize);
  temp.real = cos(angle);
  temp.imag = sin(angle);
  return temp;
}

inline float RMult(const float Ra,
                   const float Rb,
                   const float Ia,
                   const float Ib) {
  return (Ra * Rb) - (Ia * Ib);
}

inline float IMult(const float Ra,
                   const float Rb,
                   const float Ia,
                   const float Ib) {
  return (Ra * Ib) + (Ia * Rb);
}

extern "C" void toPower(float * out,
                        float * Real,
                        float * Imag,
                        const unsigned int & OFullSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      const unsigned int GID = o_itr + i_itr;
      float R = Real[GID];
      float I = Imag[GID];
      out[GID] = sqrt(R * R + I * I);
    }
  }
  return;
}

extern "C" void toHalfComplexFormat(complex * out,
                                    float * Real,
                                    float * Imag,
                                    const unsigned int & OHalfSize,
                                    const int & windowRadix) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 32) {
    for (unsigned int i_itr = 0; i_itr < 32; ++i_itr) {
      const unsigned int GID = o_itr + i_itr;
      const unsigned int Mradix = windowRadix - 1;
      const unsigned int HwindowSize = 1 << Mradix;
      const unsigned int windowItr = GID % (HwindowSize);
      const unsigned int FwindowedLoc = GID >> Mradix;
      const unsigned int ReadIdx = FwindowedLoc * HwindowSize + windowItr;
      out[GID].real = Real[ReadIdx];
      out[GID].imag = Imag[ReadIdx];
    }
  }
  return;
}

extern "C" void Stockhoptimized6(float * Rout,
                                 float * Iout,
                                 const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 32) {
    float FRBank[64];
    float FIBank[64];
    float SRBank[64];
    float SIBank[64];
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 5;
      unsigned int GlobalIndex = (Gidx & (32 - 1));
      Gidx = GlobalItr * 64 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 32];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 32];
      float RightImag = FIBank[i_itr + 32];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 32];
      float RightImag = SIBank[i_itr + 32];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 32];
      float RightImag = FIBank[i_itr + 32];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 32];
      float RightImag = SIBank[i_itr + 32];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 32; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 5;
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 32];
      float RightImag = FIBank[i_itr + 32];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 64;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 32), 64);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void Stockhoptimized7(float * Rout,
                                 float * Iout,
                                 const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 64) {
    float FRBank[128];
    float FIBank[128];
    float SRBank[128];
    float SIBank[128];
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 6;
      unsigned int GlobalIndex = (Gidx & (64 - 1));
      Gidx = GlobalItr * 128 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 64];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 64];
      float RightImag = FIBank[i_itr + 64];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 64];
      float RightImag = SIBank[i_itr + 64];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 64];
      float RightImag = FIBank[i_itr + 64];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 64];
      float RightImag = SIBank[i_itr + 64];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 64];
      float RightImag = FIBank[i_itr + 64];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 6;
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 64];
      float RightImag = SIBank[i_itr + 64];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 128;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 64), 128);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void Stockhoptimized8(float * Rout,
                                 float * Iout,
                                 const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 128) {
    float FRBank[256];
    float FIBank[256];
    float SRBank[256];
    float SIBank[256];
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 7;
      unsigned int GlobalIndex = (Gidx & (128 - 1));
      Gidx = GlobalItr * 256 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 128];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 128];
      float RightImag = FIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 128];
      float RightImag = SIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 128];
      float RightImag = FIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 128];
      float RightImag = SIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 128];
      float RightImag = FIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 128];
      float RightImag = SIBank[i_itr + 128];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 128; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 7;
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 128];
      float RightImag = FIBank[i_itr + 128];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 256;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 128), 256);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void Stockhoptimized9(float * Rout,
                                 float * Iout,
                                 const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 256) {
    float FRBank[512];
    float FIBank[512];
    float SRBank[512];
    float SIBank[512];
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 8;
      unsigned int GlobalIndex = (Gidx & (256 - 1));
      Gidx = GlobalItr * 512 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 256];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 256];
      float RightImag = FIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 256];
      float RightImag = SIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 256];
      float RightImag = FIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 256];
      float RightImag = SIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 256];
      float RightImag = FIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 256];
      float RightImag = SIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 256];
      float RightImag = FIBank[i_itr + 256];
      unsigned int segmentItr = i_itr >> 7;
      unsigned int segmentIndex = (i_itr & (128 - 1));
      unsigned int LeftStoreIdx = segmentItr * 256 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 256; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 8;
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 256];
      float RightImag = SIBank[i_itr + 256];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 512;
      unsigned int RightStoreIdx = LeftStoreIdx + 256;
      complex tw = twiddle(segmentK(i_itr, 256, 256), 512);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void preprocessed_ODW10_STH_STFT(float * inData,
                                            const unsigned int & qtConst,
                                            const unsigned int & fullSize,
                                            const unsigned int & OMove,
                                            const unsigned int & OHalfSize,
                                            float * Rout,
                                            float * Iout) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 512) {
    float FRBank[1024];
    float FIBank[1024];
    float SRBank[1024];
    float SIBank[1024];
    float windowAdded[512];
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int q_itr = o_itr >> 9;
      unsigned int idx = q_itr * OMove + i_itr;
      unsigned int Ridx = q_itr * OMove + i_itr + 512;
      int isOverflowed = (idx < fullSize);
      int RisOverflowed = (Ridx < fullSize);
      idx *= isOverflowed;
      Ridx *= RisOverflowed;
      FRBank[i_itr] = inData[idx] * isOverflowed;
      FIBank[i_itr] = 0;
      FRBank[i_itr + 512] = inData[Ridx] * RisOverflowed;
      FIBank[i_itr + 512] = 0;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float Dpoint = FRBank[i_itr];
      float Apoint = FRBank[i_itr + (512)];
      windowAdded[i_itr] = (Dpoint + Apoint);
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 256;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (256 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 128;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (128 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 64;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (64 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 32;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (32 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 16;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (16 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 8;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (8 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 4;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (4 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 2;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (2 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 1;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (1 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      FRBank[i_itr] -= (windowAdded[0] / 1024.0);
      FRBank[i_itr] *= window_func(i_itr, 1024);
      FRBank[i_itr + 512] -= (windowAdded[0] / 1024.0);
      FRBank[i_itr + 512] *= window_func(i_itr + 512, 1024);
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 0;
      unsigned int segmentIndex = (i_itr & (1 - 1));
      unsigned int LeftStoreIdx = segmentItr * 2 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 7;
      unsigned int segmentIndex = (i_itr & (128 - 1));
      unsigned int LeftStoreIdx = segmentItr * 256 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 8;
      unsigned int segmentIndex = (i_itr & (256 - 1));
      unsigned int LeftStoreIdx = segmentItr * 512 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 256;
      complex tw = twiddle(segmentK(i_itr, 256, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 9;
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 1024;
      unsigned int RightStoreIdx = LeftStoreIdx + 512;
      complex tw = twiddle(segmentK(i_itr, 512, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void preprocesses_ODW_10(float * inData,
                                    const unsigned int & qtConst,
                                    const unsigned int & fullSize,
                                    const unsigned int & OMove,
                                    float * Rout) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < qtConst; ++o_itr) {
    float wr[1024];
    float windowAdded[512];
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int idx = o_itr * OMove + i_itr;
      unsigned int Ridx = o_itr * OMove + i_itr + 512;
      int isOverflowed = (idx < fullSize);
      int RisOverflowed = (Ridx < fullSize);
      idx *= isOverflowed;
      Ridx *= RisOverflowed;
      wr[i_itr] = inData[idx] * isOverflowed;
      wr[i_itr + 512] = inData[Ridx] * RisOverflowed;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 512;
      float Dpoint = wr[i_itr];
      float Apoint = wr[i_itr + (512 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 256;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (256 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 128;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (128 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 64;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (64 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 32;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (32 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 16;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (16 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 8;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (8 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 4;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (4 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 2;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (2 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int inRange = i_itr < 1;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (1 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      wr[i_itr] -= (windowAdded[0] / 1024.0);
      wr[i_itr + 512] -= (windowAdded[0] / 1024.0);
      wr[i_itr] *= window_func(i_itr, 1024);
      wr[i_itr + 512] *= window_func(i_itr + 512, 1024);
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      Rout[o_itr * 1024 + i_itr] = wr[i_itr];
      Rout[o_itr * 1024 + i_itr + 512] = wr[i_itr + 512];
    }
  }
}

extern "C" void Stockhoptimized10(float * Rout,
                                  float * Iout,
                                  const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 512) {
    float FRBank[1024];
    float FIBank[1024];
    float SRBank[1024];
    float SIBank[1024];
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 9;
      unsigned int GlobalIndex = (Gidx & (512 - 1));
      Gidx = GlobalItr * 1024 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 512];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 7;
      unsigned int segmentIndex = (i_itr & (128 - 1));
      unsigned int LeftStoreIdx = segmentItr * 256 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 512];
      float RightImag = SIBank[i_itr + 512];
      unsigned int segmentItr = i_itr >> 8;
      unsigned int segmentIndex = (i_itr & (256 - 1));
      unsigned int LeftStoreIdx = segmentItr * 512 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 256;
      complex tw = twiddle(segmentK(i_itr, 256, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 512; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 9;
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 512];
      float RightImag = FIBank[i_itr + 512];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 1024;
      unsigned int RightStoreIdx = LeftStoreIdx + 512;
      complex tw = twiddle(segmentK(i_itr, 512, 512), 1024);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void preprocesses_ODW_11(float * inData,
                                    const unsigned int & qtConst,
                                    const unsigned int & fullSize,
                                    const unsigned int & OMove,
                                    float * Rout) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < qtConst; ++o_itr) {
    float wr[2048];
    float windowAdded[1024];
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int idx = o_itr * OMove + i_itr;
      unsigned int Ridx = o_itr * OMove + i_itr + 1024;
      int isOverflowed = (idx < fullSize);
      int RisOverflowed = (Ridx < fullSize);
      idx *= isOverflowed;
      Ridx *= RisOverflowed;
      wr[i_itr] = inData[idx] * isOverflowed;
      wr[i_itr] = 0;
      wr[i_itr + 1024] = inData[Ridx] * RisOverflowed;
      wr[i_itr + 1024] = 0;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float Dpoint = wr[i_itr];
      float Apoint = wr[i_itr + (1024)];
      windowAdded[i_itr] = (Dpoint + Apoint);
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 512;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (512 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 256;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (256 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 128;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (128 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 64;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (64 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 32;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (32 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 16;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (16 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 8;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (8 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 4;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (4 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 2;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (2 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 1;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (1 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      wr[i_itr] -= (windowAdded[0] / 2048.0);
      wr[i_itr + 1024] -= (windowAdded[0] / 2048.0);
      wr[i_itr] *= window_func(i_itr, 2048);
      wr[i_itr + 1024] *= window_func(i_itr + 1024, 2048);
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      Rout[o_itr * 2048 + i_itr] = wr[i_itr];
      Rout[o_itr * 2048 + i_itr + 1024] = wr[i_itr + 1024];
    }
  }
}

extern "C" void Stockhoptimized11(float * Rout,
                                  float * Iout,
                                  const unsigned int & OHalfSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 1024) {
    float FRBank[2048];
    float FIBank[2048];
    float SRBank[2048];
    float SIBank[2048];
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 10;
      unsigned int GlobalIndex = (Gidx & (1024 - 1));
      Gidx = GlobalItr * 2048 + GlobalIndex;
      float LeftReal = Rout[Gidx];
      float LeftImag = 0;
      float RightReal = Rout[Gidx + 1024];
      float RightImag = 0;
      unsigned int LeftStoreIdx = i_itr * 2;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 7;
      unsigned int segmentIndex = (i_itr & (128 - 1));
      unsigned int LeftStoreIdx = segmentItr * 256 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 8;
      unsigned int segmentIndex = (i_itr & (256 - 1));
      unsigned int LeftStoreIdx = segmentItr * 512 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 256;
      complex tw = twiddle(segmentK(i_itr, 256, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 9;
      unsigned int segmentIndex = (i_itr & (512 - 1));
      unsigned int LeftStoreIdx = segmentItr * 1024 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 512;
      complex tw = twiddle(segmentK(i_itr, 512, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 10;
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 2048;
      unsigned int RightStoreIdx = LeftStoreIdx + 1024;
      complex tw = twiddle(segmentK(i_itr, 1024, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void preprocessed_ODW11_STH_STFT(float * inData,
                                            const unsigned int & qtConst,
                                            const unsigned int & fullSize,
                                            const unsigned int & OMove,
                                            const unsigned int & OHalfSize,
                                            float * Rout,
                                            float * Iout) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 1024) {
    float FRBank[2048];
    float FIBank[2048];
    float SRBank[2048];
    float SIBank[2048];
    float windowAdded[1024];
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int q_itr = o_itr >> 10;
      unsigned int idx = q_itr * OMove + i_itr;
      unsigned int Ridx = q_itr * OMove + i_itr + 1024;
      int isOverflowed = (idx < fullSize);
      int RisOverflowed = (Ridx < fullSize);
      idx *= isOverflowed;
      Ridx *= RisOverflowed;
      FRBank[i_itr] = inData[idx] * isOverflowed;
      FIBank[i_itr] = 0;
      FRBank[i_itr + 1024] = inData[Ridx] * RisOverflowed;
      FIBank[i_itr + 1024] = 0;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float Dpoint = FRBank[i_itr];
      float Apoint = FRBank[i_itr + (1024)];
      windowAdded[i_itr] = (Dpoint + Apoint);
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 512;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (512 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 256;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (256 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 128;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (128 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 64;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (64 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 32;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (32 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 16;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (16 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 8;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (8 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 4;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (4 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 2;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (2 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int inRange = i_itr < 1;
      float Dpoint = windowAdded[i_itr];
      float Apoint = windowAdded[i_itr + (1 * inRange)];
      windowAdded[i_itr] = (Dpoint + Apoint) * inRange;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      FRBank[i_itr] -= (windowAdded[0] / 2048.0);
      FRBank[i_itr] *= window_func(i_itr, 2048);
      FRBank[i_itr + 1024] -= (windowAdded[0] / 2048.0);
      FRBank[i_itr + 1024] *= window_func(i_itr + 1024, 2048);
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 0;
      unsigned int segmentIndex = (i_itr & (1 - 1));
      unsigned int LeftStoreIdx = segmentItr * 2 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 1;
      complex tw = twiddle(segmentK(i_itr, 1, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 1;
      unsigned int segmentIndex = (i_itr & (2 - 1));
      unsigned int LeftStoreIdx = segmentItr * 4 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 2;
      complex tw = twiddle(segmentK(i_itr, 2, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 2;
      unsigned int segmentIndex = (i_itr & (4 - 1));
      unsigned int LeftStoreIdx = segmentItr * 8 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 4;
      complex tw = twiddle(segmentK(i_itr, 4, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 3;
      unsigned int segmentIndex = (i_itr & (8 - 1));
      unsigned int LeftStoreIdx = segmentItr * 16 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 8;
      complex tw = twiddle(segmentK(i_itr, 8, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 4;
      unsigned int segmentIndex = (i_itr & (16 - 1));
      unsigned int LeftStoreIdx = segmentItr * 32 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 16;
      complex tw = twiddle(segmentK(i_itr, 16, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 5;
      unsigned int segmentIndex = (i_itr & (32 - 1));
      unsigned int LeftStoreIdx = segmentItr * 64 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 32;
      complex tw = twiddle(segmentK(i_itr, 32, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 6;
      unsigned int segmentIndex = (i_itr & (64 - 1));
      unsigned int LeftStoreIdx = segmentItr * 128 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 64;
      complex tw = twiddle(segmentK(i_itr, 64, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 7;
      unsigned int segmentIndex = (i_itr & (128 - 1));
      unsigned int LeftStoreIdx = segmentItr * 256 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 128;
      complex tw = twiddle(segmentK(i_itr, 128, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 8;
      unsigned int segmentIndex = (i_itr & (256 - 1));
      unsigned int LeftStoreIdx = segmentItr * 512 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 256;
      complex tw = twiddle(segmentK(i_itr, 256, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      SRBank[LeftStoreIdx] = LeftReal + RTwid;
      SIBank[LeftStoreIdx] = LeftImag + ITwid;
      SRBank[RightStoreIdx] = LeftReal - RTwid;
      SIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      float LeftReal = SRBank[i_itr];
      float LeftImag = SIBank[i_itr];
      float RightReal = SRBank[i_itr + 1024];
      float RightImag = SIBank[i_itr + 1024];
      unsigned int segmentItr = i_itr >> 9;
      unsigned int segmentIndex = (i_itr & (512 - 1));
      unsigned int LeftStoreIdx = segmentItr * 1024 + segmentIndex;
      unsigned int RightStoreIdx = LeftStoreIdx + 512;
      complex tw = twiddle(segmentK(i_itr, 512, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      FRBank[LeftStoreIdx] = LeftReal + RTwid;
      FIBank[LeftStoreIdx] = LeftImag + ITwid;
      FRBank[RightStoreIdx] = LeftReal - RTwid;
      FIBank[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
    for (int i_itr = 0; i_itr < 1024; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      unsigned int GlobalItr = Gidx >> 10;
      float LeftReal = FRBank[i_itr];
      float LeftImag = FIBank[i_itr];
      float RightReal = FRBank[i_itr + 1024];
      float RightImag = FIBank[i_itr + 1024];
      unsigned int LeftStoreIdx = i_itr + GlobalItr * 2048;
      unsigned int RightStoreIdx = LeftStoreIdx + 1024;
      complex tw = twiddle(segmentK(i_itr, 1024, 1024), 2048);
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      Rout[LeftStoreIdx] = LeftReal + RTwid;
      Iout[LeftStoreIdx] = LeftImag + ITwid;
      Rout[RightStoreIdx] = LeftReal - RTwid;
      Iout[RightStoreIdx] = LeftImag - ITwid;
      ;
    }
  }
}

extern "C" void Overlap_Common(float * inData,
                               const unsigned int & OFullSize,
                               const unsigned int & fullSize,
                               const unsigned int & windowRadix,
                               const unsigned int & OMove,
                               float * outReal) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      const unsigned int overlapIdx = o_itr + i_itr;
      const unsigned int windowIdx = (overlapIdx >> windowRadix);
      const unsigned int windowLocalIdx = overlapIdx & ((1 << windowRadix) - 1);
      const unsigned int originIdx = windowIdx * OMove + windowLocalIdx;
      const unsigned int exceeded = originIdx < fullSize;
      outReal[overlapIdx] = inData[originIdx * exceeded] * exceeded;
    }
  }
}

extern "C" void Window_Hanning(float * outReal,
                               const unsigned int & OFullSize,
                               const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      const unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= hanning_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_Hamming(float * outReal,
                               const unsigned int & OFullSize,
                               const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= hamming_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_Blackman(float * outReal,
                                const unsigned int & OFullSize,
                                const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= blackman_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_Nuttall(float * outReal,
                               const unsigned int & OFullSize,
                               const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= nuttall_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_Blackman_Nuttall(float * outReal,
                                        const unsigned int & OFullSize,
                                        const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= blackman_nuttall_window(
        (Gidx & (windowSize - 1)),
        windowSize
      );
    }
  }
}

extern "C" void Window_Blackman_harris(float * outReal,
                                       const unsigned int & OFullSize,
                                       const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= blackman_harris_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_FlatTop(float * outReal,
                               const unsigned int & OFullSize,
                               const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= flatTop_window((Gidx & (windowSize - 1)), windowSize);
    }
  }
}

extern "C" void Window_Gaussian(float * outReal,
                                const unsigned int & OFullSize,
                                const unsigned int & windowSize,
                                const float & sigma) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += 64) {
    for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
      unsigned int Gidx = o_itr + i_itr;
      outReal[Gidx] *= gaussian_window((Gidx & (windowSize - 1)), windowSize, sigma);
    }
  }
}

extern "C" void DCRemove_Common(float * outReal,
                                const unsigned int & OFullSize,
                                const unsigned int & windowSize) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OFullSize; o_itr += windowSize) {
    float added[128];
    //for removing DC
    for (unsigned int inititr = 0; inititr < 64; ++inititr) {
      added[inititr] = 0;
    }
    for (unsigned int windowItr = 0; windowItr < windowSize; windowItr += 64) {
      for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
        added[i_itr + 64] = outReal[o_itr + windowItr + i_itr];
      }
      for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
        added[i_itr] += added[i_itr + 64];
      }
    }
    for (unsigned int segment = 32; segment > 0; segment >>= 1) {
      for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
        unsigned int inSegment = i_itr < segment;
        float left = added[i_itr];
        float right = added[i_itr + segment];
        added[i_itr] = (left + right) * inSegment;
      }
    }
    for (unsigned int windowItr = 0; windowItr < windowSize; windowItr += 64) {
      for (unsigned int i_itr = 0; i_itr < 64; ++i_itr) {
        outReal[o_itr + windowItr + i_itr] -= (added[0] / (float) windowSize);
      }
    }
  }
}

extern "C" void StockHamCommon(float * inReal,
                               float * inImag,
                               float * outReal,
                               float * outImag,
                               const unsigned int & HwindowSize,
                               const unsigned int & stageRadix,
                               const unsigned int & OHalfSize,
                               const unsigned int & radixData) {
#pragma omp parallel for
  for (unsigned int o_itr = 0; o_itr < OHalfSize; o_itr += 256) {
    for (unsigned int i_itr = 0; i_itr < 256; ++i_itr) {
      unsigned int OIdx = o_itr + i_itr;
      unsigned int FwindowSize = HwindowSize << 1;
      unsigned int GlobalItr = OIdx >> (radixData - 1);
      unsigned int GlobalIndex = (OIdx & (HwindowSize - 1));
      OIdx = GlobalItr * FwindowSize + GlobalIndex;
      float LeftReal = inReal[OIdx];
      float LeftImag = inImag[OIdx];
      float RightReal = inReal[OIdx + HwindowSize];
      float RightImag = inImag[OIdx + HwindowSize];
      unsigned int segmentSize = 1 << stageRadix;
      unsigned int segmentItr = GlobalIndex >> stageRadix;
      unsigned int segmentIndex = (GlobalIndex & (segmentSize - 1));
      //OIdx & (segmentSize - 1);
      unsigned int LeftStoreIdx = segmentItr * (segmentSize << 1) + segmentIndex + GlobalItr * FwindowSize;
      unsigned int RightStoreIdx = LeftStoreIdx + segmentSize;
      complex tw = twiddle(
        segmentK(OIdx, segmentSize, HwindowSize),
        FwindowSize
      );
      float RTwid = RMult(RightReal, tw.real, RightImag, tw.imag);
      float ITwid = IMult(RightReal, tw.real, RightImag, tw.imag);
      outReal[LeftStoreIdx] = LeftReal + RTwid;
      outImag[LeftStoreIdx] = LeftImag + ITwid;
      outReal[RightStoreIdx] = LeftReal - RTwid;
      outImag[RightStoreIdx] = LeftImag - ITwid;
    }
  }
}

