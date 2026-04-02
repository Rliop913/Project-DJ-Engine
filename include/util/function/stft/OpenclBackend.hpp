#pragma once

#include "STFT_Parallel.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <cmrc/cmrc.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

CMRC_DECLARE(pdje_okl);

namespace PDJE_PARALLEL {

using namespace cl;
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
    Program  opencl_kernel_code;
    struct {
        std::optional<Kernel> EXP6STFT;
        std::optional<Kernel> EXP7STFT;
        std::optional<Kernel> EXP8STFT;
        std::optional<Kernel> EXP9STFT;
        std::optional<Kernel> EXP10STFT;
        std::optional<Kernel> EXP11STFT;

        std::optional<Kernel> EXPCommon;
        std::optional<Kernel> Overlap;

        std::optional<Kernel> DCRemove;
        std::optional<Kernel> Hanning;
        std::optional<Kernel> Hamming;
        std::optional<Kernel> Blackman;
        std::optional<Kernel> Nuttall;
        std::optional<Kernel> Blackman_Nuttall;
        std::optional<Kernel> Blackman_Harris;
        std::optional<Kernel> FlatTop;
        std::optional<Kernel> Gaussian;
        std::optional<Kernel> toBinOnly;
        std::optional<Kernel> BinPowerChain;
        std::optional<Kernel> toPower;
        std::optional<Kernel> MelScale;
        std::optional<Kernel> MelDBChain;
        std::optional<Kernel> toDB;
    } built_kernels;

    struct {
        std::optional<Buffer> origin;
        std::optional<Buffer> real;
        std::optional<Buffer> imag;
        std::optional<Buffer> subreal;
        std::optional<Buffer> subimag;
        std::optional<Buffer> bin_real;
        std::optional<Buffer> bin_imag;
        std::optional<Buffer> power;
        std::optional<Buffer> mel;
        std::optional<Buffer> mel_filter_bank;
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
    OPENCL_STFT()
    {

        std::vector<cl::Platform> platforms;

        int device_power_score = 0;
        cl::Platform::get(&platforms);
        for (auto &i : platforms) {
            std::vector<cl::Device> calc_devs;
            i.getDevices(CL_DEVICE_TYPE_ALL, &calc_devs);
            for (auto target_dev : calc_devs) {
                int local_power_score = 0;
                target_dev.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS,
                                   &local_power_score);
                if (local_power_score > device_power_score) {

                    gpu                = std::move(target_dev);
                    device_power_score = local_power_score;
                }
            }
        }
        if (device_power_score == 0 || !gpu.has_value()) {
            throw std::runtime_error("failed to load opencl device.");
        } else {
            gpu_ctxt = cl::Context(gpu.value());
        }
        auto fs   = cmrc::pdje_okl::get_filesystem();
        auto file = fs.open("STFT_MAIN.cl");

        std::string cl_codes(file.begin(), file.end());
        gpu_codes.emplace(gpu_ctxt.value(), cl_codes);
        if (gpu_codes->build(gpu.value()) != CL_SUCCESS) {
            throw std::runtime_error("failed to build cl kernel codes.");
        }
        CQ.emplace(gpu_ctxt.value(), gpu.value());
    }
    ~OPENCL_STFT() override;
};

} // namespace PDJE_PARALLEL
