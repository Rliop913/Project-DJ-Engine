#include "OpenclBackend.hpp"
#include "CL/opencl.hpp"
#include "OpenclArgChains.hpp"
#include "STFT_Parallel.hpp"
#include <CL/cl.h>
#include <exception>
#include <stdexcept>

namespace PDJE_PARALLEL {

namespace {

constexpr float kDefaultGaussianSigma = 0.4f;

} // namespace

std::pair<REAL_VEC, IMAG_VEC>
OPENCL_STFT::Execute(REAL_VEC         &origin_cpu_memory,
                     const WINDOW_LIST window,
                     POST_PROCESS      post_process,
                     const unsigned int win_expsz,
                     const StftArgs   &args)
{
    (void)post_process;
    if (win_expsz < 6) {
        return {};
    }
    bool IsNeedSubBuffer = win_expsz > 11;
    if (!SetMemory(static_cast<uint32_t>(origin_cpu_memory.size()),
                   args.OFullSize,
                   IsNeedSubBuffer)) {
        throw std::runtime_error("Failed to Init Gpu Memory.");
    }
    CQ->enqueueWriteBuffer(memories.origin.value(),
                           CL_FALSE,
                           0,
                           sizeof(float) * origin_cpu_memory.size(),
                           origin_cpu_memory.data());
    auto buildKernel = [this](std::optional<Kernel> &kernel,
                              const char            *kernelName) {
        if (!kernel) {
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
                             256);
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
                             256);
            }
        }
        break;
    }

    Buffer *resultReal = &memories.real.value();
    Buffer *resultImag = &memories.imag.value();
    if (IsNeedSubBuffer && (win_expsz % 2) != 0) {
        resultReal = &memories.subreal.value();
        resultImag = &memories.subimag.value();
    }

    // switch (effective_post_process) {
    // case POST_PROCESS::POWER:
    //     buildKernel(built_kernels.toPower, "_occa_toPower_0");
    //     if (!memories.power) {
    //         memories.power.emplace(gpu_ctxt.value(),
    //                                CL_MEM_READ_WRITE,
    //                                sizeof(float) * args.OFullSize);
    //     }
    //     setArgChain4(built_kernels.toPower,
    //                  memories.power.value(),
    //                  *resultReal,
    //                  *resultImag,
    //                  args.OFullSize,
    //                  args.OFullSize,
    //                    64);
    //     break;
    // case POST_PROCESS::NONE:
    // default:
    //     break;
    // }
    // REAL_VEC rout;
    // IMAG_VEC iout;
    // switch (effective_post_process) {
    // case POST_PROCESS::POWER:
    //     rout.resize(args.OFullSize);
    //     CQ->enqueueReadBuffer(memories.power.value(),
    //                           CL_FALSE,
    //                           0,
    //                           sizeof(float) * args.OFullSize,
    //                           rout.data());
    //     break;
    // case POST_PROCESS::NONE:
    // default:
    //     rout.resize(args.OFullSize);
    //     iout.resize(args.OFullSize);
    //     CQ->enqueueReadBuffer(*resultReal,
    //                           CL_FALSE,
    //                           0,
    //                           sizeof(float) * args.OFullSize,
    //                           rout.data());
    //     CQ->enqueueReadBuffer(*resultImag,
    //                           CL_FALSE,
    //                           0,
    //                           sizeof(float) * args.OFullSize,
    //                           iout.data());
    //     break;
    // }
    if (GetResult()) {
        return {};
        // return std::pair(rout, iout);
    } else {
        throw std::runtime_error("Failed to Retrieve Result.");
    }
}

bool
OPENCL_STFT::SetMemory(const uint32_t     origin_cpu_memory_sz,
                       const unsigned int OFullSize,
                       const bool         needSubBuffer)
{
    if (prev_origin_size != origin_cpu_memory_sz) {
        memories.origin.reset();
        memories.origin.emplace(gpu_ctxt.value(),
                                CL_MEM_READ_ONLY,
                                sizeof(float) * origin_cpu_memory_sz);
        prev_origin_size = origin_cpu_memory_sz;
    }
    if (prev_overlap_fullsize != OFullSize) {
        memories.real.reset();
        memories.imag.reset();
        memories.subreal.reset();
        memories.subimag.reset();
        memories.power.reset();
        prev_overlap_subbuffer_fullsize = 0;

        memories.real.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * OFullSize);
        memories.imag.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * OFullSize);
        prev_overlap_fullsize = OFullSize;
    }
    if (!needSubBuffer) {
        memories.subreal.reset();
        memories.subimag.reset();
        prev_overlap_subbuffer_fullsize = 0;
    }
    if (needSubBuffer &&
        (!memories.subreal.has_value() || !memories.subimag.has_value() ||
         prev_overlap_subbuffer_fullsize != OFullSize)) {

        memories.subreal.reset();
        memories.subimag.reset();
        memories.subreal.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * OFullSize);
        memories.subimag.emplace(
            gpu_ctxt.value(), CL_MEM_READ_WRITE, sizeof(float) * OFullSize);
        prev_overlap_subbuffer_fullsize = OFullSize;
    }

    return true;
}

OPENCL_STFT::~OPENCL_STFT() = default;

} // namespace PDJE_PARALLEL
