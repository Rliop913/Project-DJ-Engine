#pragma once

#include "util/function/stft/MelFilterBank.hpp"
#include "global/PDJE_EXPORT_SETTER.hpp"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace PDJE_PARALLEL {

namespace detail {
class STFTImpl;
}

inline int
toQuot(const unsigned int fullSize,
       const float        overlapRatio,
       const int          windowSize)
{
    if (overlapRatio == 0.0f) {
        return static_cast<int>(fullSize / static_cast<unsigned int>(windowSize)) +
               1;
    }

    const float stepSize = static_cast<float>(windowSize) * (1.0f - overlapRatio);
    return static_cast<int>(static_cast<float>(fullSize) / stepSize) + 1;
}

enum class BACKEND_T { OPENCL, METAL, SERIAL };

enum class FRAME_POLICY {
    LEGACY_ZERO_PAD = 0,
    EXACT_WINDOWED
};

enum WINDOW_LIST {
    BLACKMAN,
    BLACKMAN_HARRIS,
    BLACKMAN_NUTTALL,
    HANNING,
    NUTTALL,
    FLATTOP,
    GAUSSIAN,
    HAMMING,
    NONE
};

struct POST_PROCESS {
    bool to_bin            = false;
    bool toPower           = false;
    bool mel_scale         = false;
    bool to_db             = false;
    bool normalize_min_max = false;
    bool to_rgb            = false;

    void
    check_values()
    {
        if (to_rgb) {
            mel_scale = true;
        }
        if (mel_scale) {
            to_bin  = true;
            toPower = true;
        }
    }

    bool
    Chainable_BIN_POWER() const
    {
        return to_bin && toPower;
    }

    bool
    Chainable_MEL_DB() const
    {
        return mel_scale && to_db;
    }
};

using StftResult = std::pair<std::vector<float>, std::vector<float>>;

struct STFTRequest {
    int                            sample_rate  = 22050;
    int                            n_fft        = 1024;
    unsigned int                   hop_length   = 441u;
    WINDOW_LIST                    target_window = WINDOW_LIST::HANNING;
    POST_PROCESS                   post_process {};
    FRAME_POLICY                   frame_policy = FRAME_POLICY::EXACT_WINDOWED;
    std::optional<MelFilterBankSpec> mel_filter_bank {};
    bool                           dc_remove = true;
};

class PDJE_API STFT {
  public:
    STFT();
    ~STFT();

    STFT(STFT &&) noexcept;
    STFT &
    operator=(STFT &&) noexcept;

    STFT(const STFT &)            = delete;
    STFT &operator=(const STFT &) = delete;

    BACKEND_T
    active_backend() const noexcept;

    static BACKEND_T
    detect_available_backend() noexcept;

    StftResult
    calculate(std::vector<float> &PCMdata, const STFTRequest &request);

    StftResult
    calculate(std::vector<float> &PCMdata,
              WINDOW_LIST         target_window = WINDOW_LIST::HANNING,
              int                 windowSizeEXP = 10,
              float               overlapRatio  = 0.5f,
              POST_PROCESS        post_process  = POST_PROCESS());

  private:
    std::unique_ptr<detail::STFTImpl> impl_;
};

} // namespace PDJE_PARALLEL
