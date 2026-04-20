#pragma once

#include <memory>
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

class STFT {
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
    calculate(std::vector<float> &PCMdata,
              WINDOW_LIST         target_window = WINDOW_LIST::HANNING,
              int                 windowSizeEXP = 10,
              float               overlapRatio  = 0.5f,
              POST_PROCESS        post_process  = POST_PROCESS());

  private:
    std::unique_ptr<detail::STFTImpl> impl_;
};

} // namespace PDJE_PARALLEL
