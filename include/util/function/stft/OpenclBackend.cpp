#include "OpenclBackend.hpp"

#include "BackendLess.hpp"
#include "CL/opencl.hpp"
#include "MelFilterBank.hpp"
#include "OpenclArgChains.hpp"
#include "STFT_Parallel.hpp"

#include <CL/cl.h>
#include <algorithm>
#include <cstddef>
#include <exception>
#include <optional>
#include <stdexcept>

namespace PDJE_PARALLEL {

namespace {

constexpr float        kDefaultGaussianSigma = 0.4f;
constexpr unsigned int kLocalSize64          = 64;
constexpr unsigned int kLocalSize256         = 256;

unsigned int
RoundUpToLocalSize(const unsigned int size, const unsigned int localSize)
{
    if (localSize == 0 || (size % localSize) == 0) {
        return size;
    }

    return size + (localSize - (size % localSize));
}

} // namespace

void
OPENCL_STFT::EnsureMelFilterBank(const int windowSize)
{
    if (prev_fft_size == windowSize && memories.mel_filter_bank.has_value()) {
        return;
    }

    mel_filter_bank_host = GenMelFilterBank(kDefaultSampleRate,
                                            windowSize,
                                            static_cast<int>(kMelBins),
                                            0.0f,
                                            -1.0f,
                                            MelFormula::Slaney,
                                            MelNorm::Slaney);
    if (mel_filter_bank_host.empty()) {
        throw std::runtime_error("Failed to Generate Mel Filter Bank.");
    }

    memories.mel_filter_bank.reset();
    memories.mel_filter_bank.emplace(gpu_ctxt.value(),
                                     CL_MEM_READ_ONLY,
                                     sizeof(float) *
                                         mel_filter_bank_host.size());
    CQ->enqueueWriteBuffer(memories.mel_filter_bank.value(),
                           CL_FALSE,
                           0,
                           sizeof(float) * mel_filter_bank_host.size(),
                           mel_filter_bank_host.data());
    prev_fft_size = windowSize;
}

std::pair<REAL_VEC, IMAG_VEC>
OPENCL_STFT::Execute(REAL_VEC          &origin_cpu_memory,
                     const WINDOW_LIST  window,
                     POST_PROCESS       post_process,
                     const unsigned int win_expsz,
                     const StftArgs    &args)
{
    post_process.check_values();

    if (win_expsz < 6) {
        return {};
    }

    const bool needSubBuffer = win_expsz > 11;
    if (!SetMemory(static_cast<uint32_t>(origin_cpu_memory.size()),
                   args,
                   post_process,
                   needSubBuffer)) {
        throw std::runtime_error("Failed to Init Gpu Memory.");
    }

    CQ->enqueueWriteBuffer(memories.origin.value(),
                           CL_FALSE,
                           0,
                           sizeof(float) * origin_cpu_memory.size(),
                           origin_cpu_memory.data());

    REAL_VEC zeroImag(args.OFullSize, 0.0f);
    CQ->enqueueWriteBuffer(memories.imag.value(),
                           CL_FALSE,
                           0,
                           sizeof(float) * zeroImag.size(),
                           zeroImag.data());

    auto buildKernel = [this](std::optional<Kernel> &kernel,
                              const char            *kernelName) {
        if (!kernel.has_value()) {
            kernel.emplace(gpu_codes.value(), kernelName);
        }
    };

    buildKernel(built_kernels.Overlap, "_occa_Overlap_Common_0");
    setArgChain6(built_kernels.Overlap,
                 memories.origin.value(),
                 args.OFullSize,
                 args.FullSize,
                 static_cast<unsigned int>(win_expsz),
                 args.OMove,
                 memories.real.value(),
                 args.OFullSize,
                 64);

    buildKernel(built_kernels.DCRemove, "_occa_DCRemove_Common_0");
    setArgChain3(built_kernels.DCRemove,
                 memories.real.value(),
                 args.OFullSize,
                 args.windowSize,
                 args.qtConst * 64,
                 64);

    auto enqueueWindow = [&](std::optional<Kernel> &kernel,
                             const char            *kernelName) {
        buildKernel(kernel, kernelName);
        setArgChain3(kernel,
                     memories.real.value(),
                     args.OFullSize,
                     args.windowSize,
                     args.OFullSize,
                     64);
    };

    switch (window) {
    case WINDOW_LIST::BLACKMAN:
        enqueueWindow(built_kernels.Blackman, "_occa_Window_Blackman_0");
        break;
    case WINDOW_LIST::BLACKMAN_HARRIS:
        enqueueWindow(built_kernels.Blackman_Harris,
                      "_occa_Window_Blackman_harris_0");
        break;
    case WINDOW_LIST::BLACKMAN_NUTTALL:
        enqueueWindow(built_kernels.Blackman_Nuttall,
                      "_occa_Window_Blackman_Nuttall_0");
        break;
    case WINDOW_LIST::NUTTALL:
        enqueueWindow(built_kernels.Nuttall, "_occa_Window_Nuttall_0");
        break;
    case WINDOW_LIST::FLATTOP:
        enqueueWindow(built_kernels.FlatTop, "_occa_Window_FlatTop_0");
        break;
    case WINDOW_LIST::GAUSSIAN:
        buildKernel(built_kernels.Gaussian, "_occa_Window_Gaussian_0");
        setArgChain4(built_kernels.Gaussian,
                     memories.real.value(),
                     args.OFullSize,
                     args.windowSize,
                     kDefaultGaussianSigma,
                     args.OFullSize,
                     64);
        break;
    case WINDOW_LIST::HAMMING:
        enqueueWindow(built_kernels.Hamming, "_occa_Window_Hamming_0");
        break;
    case WINDOW_LIST::HANNING:
        enqueueWindow(built_kernels.Hanning, "_occa_Window_Hanning_0");
        break;
    default:
        break;
    }

    switch (win_expsz) {
    case 6:
        buildKernel(built_kernels.EXP6STFT, "_occa_Stockhoptimized6_0");
        setArgChain3(built_kernels.EXP6STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     32);
        break;
    case 7:
        buildKernel(built_kernels.EXP7STFT, "_occa_Stockhoptimized7_0");
        setArgChain3(built_kernels.EXP7STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     64);
        break;
    case 8:
        buildKernel(built_kernels.EXP8STFT, "_occa_Stockhoptimized8_0");
        setArgChain3(built_kernels.EXP8STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     128);
        break;
    case 9:
        buildKernel(built_kernels.EXP9STFT, "_occa_Stockhoptimized9_0");
        setArgChain3(built_kernels.EXP9STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     256);
        break;
    case 10:
        buildKernel(built_kernels.EXP10STFT, "_occa_Stockhoptimized10_0");
        setArgChain3(built_kernels.EXP10STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     512);
        break;
    case 11:
        buildKernel(built_kernels.EXP11STFT, "_occa_Stockhoptimized11_0");
        setArgChain3(built_kernels.EXP11STFT,
                     memories.real.value(),
                     memories.imag.value(),
                     args.OHalfSize,
                     args.OHalfSize,
                     1024);
        break;
    default:
        buildKernel(built_kernels.EXPCommon, "_occa_StockHamCommon_0");
        const unsigned int halfWindowSize =
            static_cast<unsigned int>(args.windowSize >> 1);
        for (unsigned int stage = 0; stage < win_expsz; ++stage) {
            if ((stage % 2) == 0) {
                setArgChain8(built_kernels.EXPCommon,
                             memories.real.value(),
                             memories.imag.value(),
                             memories.subreal.value(),
                             memories.subimag.value(),
                             halfWindowSize,
                             stage,
                             args.OHalfSize,
                             static_cast<unsigned int>(win_expsz),
                             args.OHalfSize,
                             kLocalSize256);
            } else {
                setArgChain8(built_kernels.EXPCommon,
                             memories.subreal.value(),
                             memories.subimag.value(),
                             memories.real.value(),
                             memories.imag.value(),
                             halfWindowSize,
                             stage,
                             args.OHalfSize,
                             static_cast<unsigned int>(win_expsz),
                             args.OHalfSize,
                             kLocalSize256);
            }
        }
        break;
    }

    Buffer *activeReal = &memories.real.value();
    Buffer *activeImag = &memories.imag.value();
    if (needSubBuffer && (win_expsz % 2) != 0) {
        activeReal = &memories.subreal.value();
        activeImag = &memories.subimag.value();
    }

    unsigned int activeSize = args.OFullSize;
    bool         hasImag    = true;

    const uint32_t binSize = static_cast<uint32_t>((args.windowSize >> 1) + 1);
    const uint32_t binFullSize = binSize * static_cast<uint32_t>(args.qtConst);
    const uint32_t melFullSize = kMelBins * static_cast<uint32_t>(args.qtConst);

    if (post_process.Chainable_BIN_POWER()) {
        buildKernel(built_kernels.BinPowerChain, "_occa_BinPowerChain_0");
        setArgChain6(built_kernels.BinPowerChain,
                     *activeReal,
                     *activeImag,
                     memories.bin_real.value(),
                     binSize,
                     args.windowSize,
                     binFullSize,
                     RoundUpToLocalSize(binFullSize, kLocalSize64),
                     kLocalSize64);
        activeReal = &memories.bin_real.value();
        activeSize = binFullSize;
        hasImag    = false;
    } else if (post_process.to_bin) {
        buildKernel(built_kernels.toBinOnly, "_occa_toBinOnly_0");
        setArgChain7(built_kernels.toBinOnly,
                     *activeReal,
                     *activeImag,
                     memories.bin_real.value(),
                     memories.bin_imag.value(),
                     binSize,
                     args.windowSize,
                     binFullSize,
                     RoundUpToLocalSize(binFullSize, kLocalSize64),
                     kLocalSize64);
        activeReal = &memories.bin_real.value();
        activeImag = &memories.bin_imag.value();
        activeSize = binFullSize;
        hasImag    = true;
    } else if (post_process.toPower) {
        buildKernel(built_kernels.toPower, "_occa_toPower_0");
        setArgChain4(built_kernels.toPower,
                     memories.power.value(),
                     *activeReal,
                     *activeImag,
                     args.OFullSize,
                     args.OFullSize,
                     kLocalSize64);
        activeReal = &memories.power.value();
        activeSize = args.OFullSize;
        hasImag    = false;
    }

    if (post_process.mel_scale) {
        EnsureMelFilterBank(static_cast<int>(args.windowSize));
    }

    if (post_process.Chainable_MEL_DB()) {
        buildKernel(built_kernels.MelDBChain, "_occa_MelDBChain_0");
        setArgChain6(built_kernels.MelDBChain,
                     memories.mel.value(),
                     *activeReal,
                     memories.mel_filter_bank.value(),
                     melFullSize,
                     binSize,
                     kMelBins,
                     RoundUpToLocalSize(melFullSize, kLocalSize64),
                     kLocalSize64);
        activeReal = &memories.mel.value();
        activeSize = melFullSize;
        hasImag    = false;
    } else if (post_process.mel_scale) {
        buildKernel(built_kernels.MelScale, "_occa_MelScale_0");
        setArgChain6(built_kernels.MelScale,
                     memories.mel.value(),
                     *activeReal,
                     memories.mel_filter_bank.value(),
                     melFullSize,
                     binSize,
                     kMelBins,
                     RoundUpToLocalSize(melFullSize, kLocalSize64),
                     kLocalSize64);
        activeReal = &memories.mel.value();
        activeSize = melFullSize;
        hasImag    = false;
    } else if (post_process.to_db) {
        buildKernel(built_kernels.toDB, "_occa_toDB_0");
        setArgChain2(built_kernels.toDB,
                     *activeReal,
                     activeSize,
                     RoundUpToLocalSize(activeSize, kLocalSize64),
                     kLocalSize64);
        hasImag = false;
    }

    REAL_VEC rout(activeSize, 0.0f);
    IMAG_VEC iout;

    CQ->enqueueReadBuffer(
        *activeReal, CL_FALSE, 0, sizeof(float) * rout.size(), rout.data());
    if (hasImag) {
        iout.resize(activeSize);
        CQ->enqueueReadBuffer(
            *activeImag, CL_FALSE, 0, sizeof(float) * iout.size(), iout.data());
    }

    if (!GetResult()) {
        throw std::runtime_error("Failed to Retrieve Result.");
    }

    if (post_process.normalize_min_max) {
        const uint32_t chunkSize =
            post_process.mel_scale
                ? kMelBins
                : (post_process.to_bin
                       ? binSize
                       : static_cast<uint32_t>(args.windowSize));
        Normalize_minmax(rout, chunkSize);
    }

    if (post_process.to_rgb) {
        return { TO_RGB(rout, kMelBins), {} };
    }

    if (hasImag) {
        return { std::move(rout), std::move(iout) };
    }

    return { std::move(rout), {} };
}

bool
OPENCL_STFT::SetMemory(const uint32_t      origin_cpu_memory_sz,
                       const StftArgs     &args,
                       const POST_PROCESS &post_process,
                       const bool          needSubBuffer)
{
    if (prev_origin_size != origin_cpu_memory_sz) {
        memories.origin.reset();
        memories.origin.emplace(gpu_ctxt.value(),
                                CL_MEM_READ_ONLY,
                                sizeof(float) * origin_cpu_memory_sz);
        prev_origin_size = origin_cpu_memory_sz;
    }

    if (prev_overlap_fullsize != args.OFullSize) {
        memories.real.reset();
        memories.imag.reset();
        memories.subreal.reset();
        memories.subimag.reset();
        memories.power.reset();

        memories.real.emplace(gpu_ctxt.value(),
                              CL_MEM_READ_WRITE,
                              sizeof(float) * args.OFullSize);
        memories.imag.emplace(gpu_ctxt.value(),
                              CL_MEM_READ_WRITE,
                              sizeof(float) * args.OFullSize);
        prev_overlap_fullsize = args.OFullSize;
    }

    if (!needSubBuffer) {
        memories.subreal.reset();
        memories.subimag.reset();
        prev_overlap_subbuffer_fullsize = 0;
    }

    if (needSubBuffer &&
        (!memories.subreal.has_value() || !memories.subimag.has_value() ||
         prev_overlap_subbuffer_fullsize != args.OFullSize)) {
        memories.subreal.reset();
        memories.subimag.reset();
        memories.subreal.emplace(gpu_ctxt.value(),
                                 CL_MEM_READ_WRITE,
                                 sizeof(float) * args.OFullSize);
        memories.subimag.emplace(gpu_ctxt.value(),
                                 CL_MEM_READ_WRITE,
                                 sizeof(float) * args.OFullSize);
        prev_overlap_subbuffer_fullsize = args.OFullSize;
    }

    if (post_process.toPower && !post_process.to_bin &&
        !memories.power.has_value()) {
        memories.power.emplace(gpu_ctxt.value(),
                               CL_MEM_READ_WRITE,
                               sizeof(float) * args.OFullSize);
    }

    const uint32_t binSize = static_cast<uint32_t>((args.windowSize >> 1) + 1);
    const uint32_t binFullSize = binSize * static_cast<uint32_t>(args.qtConst);
    if (post_process.to_bin && prev_bin_fullsize != binFullSize) {
        memories.bin_real.reset();
        memories.bin_imag.reset();
        memories.bin_real.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * binFullSize);
        memories.bin_imag.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * binFullSize);
        prev_bin_fullsize = binFullSize;
    }

    const uint32_t melFullSize = kMelBins * static_cast<uint32_t>(args.qtConst);
    if (post_process.mel_scale && prev_mel_fullsize != melFullSize) {
        memories.mel.reset();
        memories.mel.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * melFullSize);
        prev_mel_fullsize = melFullSize;
    }

    return true;
}

OPENCL_STFT::~OPENCL_STFT() = default;

} // namespace PDJE_PARALLEL
