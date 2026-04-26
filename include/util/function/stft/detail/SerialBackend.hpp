#pragma once

#include "util/function/stft/detail/StftBackend.hpp"

#include <cstdint>
#include <optional>
#include <vector>

namespace PDJE_PARALLEL::detail {

class SERIAL_STFT final : public IStftBackend {
  private:
    uint32_t prev_overlap_fullsize           = 0;
    uint32_t prev_overlap_subbuffer_fullsize = 0;
    uint32_t prev_bin_fullsize               = 0;
    uint32_t prev_mel_fullsize               = 0;

    std::vector<float> real;
    std::vector<float> imag;
    std::vector<float> subreal;
    std::vector<float> subimag;
    std::vector<float> bin_real;
    std::vector<float> bin_imag;
    std::vector<float> mel;
    std::vector<float> rgb;
    std::vector<float> mel_filter_bank;
    std::optional<MelFilterBankSpec> prev_mel_filter_bank_spec;

    void
    EnsureMemory(const StftArgs &gargs,
                 const POST_PROCESS &post_process,
                 bool needSubBuffer);

    void
    EnsureMelFilterBank(const StftArgs &gargs);

    void
    ApplyWindow(WINDOW_LIST target_window, const StftArgs &gargs);

    void
    RunFft(unsigned int windowSizeEXP, const StftArgs &gargs);

  public:
    StftResult
    Execute(std::vector<float> &PCMdata,
            WINDOW_LIST         target_window,
            POST_PROCESS        post_process,
            unsigned int        windowSizeEXP,
            const StftArgs     &gargs) override;

    ~SERIAL_STFT() override;
};

} // namespace PDJE_PARALLEL::detail
