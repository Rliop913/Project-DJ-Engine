#pragma once

#include "util/function/stft/detail/StftBackend.hpp"

#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <cmrc/cmrc.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

CMRC_DECLARE(pdje_okl);

namespace PDJE_PARALLEL::detail {

using REAL_VEC = std::vector<float>;
using IMAG_VEC = std::vector<float>;

class OPENCL_STFT final : public IStftBackend {

  private:
    static constexpr uint32_t kMelBins           = 80;
    static constexpr int      kDefaultSampleRate = 48000;

    uint32_t prev_origin_size                = 0;
    uint32_t prev_overlap_fullsize           = 0;
    uint32_t prev_overlap_subbuffer_fullsize = 0;
    uint32_t prev_bin_fullsize               = 0;
    uint32_t prev_mel_fullsize               = 0;
    int      prev_fft_size                   = 0;
    cl::Program opencl_kernel_code;
    struct {
        std::optional<cl::Kernel> EXP6STFT;
        std::optional<cl::Kernel> EXP7STFT;
        std::optional<cl::Kernel> EXP8STFT;
        std::optional<cl::Kernel> EXP9STFT;
        std::optional<cl::Kernel> EXP10STFT;
        std::optional<cl::Kernel> EXP11STFT;

        std::optional<cl::Kernel> EXPCommon;
        std::optional<cl::Kernel> Overlap;

        std::optional<cl::Kernel> DCRemove;
        std::optional<cl::Kernel> Hanning;
        std::optional<cl::Kernel> Hamming;
        std::optional<cl::Kernel> Blackman;
        std::optional<cl::Kernel> Nuttall;
        std::optional<cl::Kernel> Blackman_Nuttall;
        std::optional<cl::Kernel> Blackman_Harris;
        std::optional<cl::Kernel> FlatTop;
        std::optional<cl::Kernel> Gaussian;
        std::optional<cl::Kernel> toBinOnly;
        std::optional<cl::Kernel> BinPowerChain;
        std::optional<cl::Kernel> toPower;
        std::optional<cl::Kernel> MelScale;
        std::optional<cl::Kernel> MelDBChain;
        std::optional<cl::Kernel> toDB;
    } built_kernels;

    struct {
        std::optional<cl::Buffer> origin;
        std::optional<cl::Buffer> real;
        std::optional<cl::Buffer> imag;
        std::optional<cl::Buffer> subreal;
        std::optional<cl::Buffer> subimag;
        std::optional<cl::Buffer> bin_real;
        std::optional<cl::Buffer> bin_imag;
        std::optional<cl::Buffer> power;
        std::optional<cl::Buffer> mel;
        std::optional<cl::Buffer> mel_filter_bank;
    } memories;

    std::optional<cl::Device>       gpu;
    std::optional<cl::CommandQueue> CQ;
    std::optional<cl::Context>      gpu_ctxt;
    std::optional<cl::Program>      gpu_codes;
    std::vector<float>              mel_filter_bank_host;

    bool
    SetMemory(const uint32_t      origin_cpu_memory_sz,
              const StftArgs     &args,
              const POST_PROCESS &post_process,
              const bool          needSubBuffer);

    void
    EnsureMelFilterBank(int windowSize);

    bool
    GetResult()
    {
        if (CQ->flush() != CL_SUCCESS) {
            return false;
        }
        if (CQ->finish() != CL_SUCCESS) {
            return false;
        }
        return true;
    }

  public:
    StftResult
    Execute(REAL_VEC       &origin_cpu_memory,
            WINDOW_LIST     window,
            POST_PROCESS    post_process,
            unsigned int    win_expsz,
            const StftArgs &args) override;
    OPENCL_STFT();
    ~OPENCL_STFT() override;
};

} // namespace PDJE_PARALLEL::detail
