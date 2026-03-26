#include "STFT_Parallel.hpp"

#include "opencl_compiled.hpp"
#include "openmp_compiled.hpp"

#include <vector>

namespace PDJE_PARALLEL {

namespace {

constexpr float kDefaultGaussianSigma = 0.4f;

} // namespace

STFT::STFT()
{
    backendinfo.LoadBackend();
}

STFT::~STFT() = default;

std::pair<std::vector<float>, std::vector<float>>
STFT::calculate(std::vector<float> &PCMdata,
                const WINDOW_LIST   target_window,
                const int           windowSizeEXP,
                const float         overlapRatio)
{
    if (PCMdata.empty() || overlapRatio < 0 || overlapRatio >= 1.0 ||
        windowSizeEXP < 6) {
        return {};
    }

    auto gargs = GenArgs(PCMdata, windowSizeEXP, overlapRatio);

    std::vector<float> realVec(gargs.OFullSize, 0.0f);
    std::vector<float> imagVec(gargs.OFullSize, 0.0f);

    Overlap_Common(PCMdata.data(),
                   gargs.OFullSize,
                   gargs.FullSize,
                   windowSizeEXP,
                   gargs.OMove,
                   realVec.data());

    DCRemove_Common(realVec.data(), gargs.OFullSize, gargs.windowSize);

    switch (target_window) {
    case WINDOW_LIST::BLACKMAN:
        Window_Blackman(realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::BLACKMAN_HARRIS:
        Window_Blackman_harris(
            realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::BLACKMAN_NUTTALL:
        Window_Blackman_Nuttall(
            realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::NUTTALL:
        Window_Nuttall(realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::FLATTOP:
        Window_FlatTop(realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::GAUSSIAN:
        Window_Gaussian(realVec.data(),
                        gargs.OFullSize,
                        gargs.windowSize,
                        kDefaultGaussianSigma);
        break;
    case WINDOW_LIST::HAMMING:
        Window_Hamming(realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::HANNING:
    default:
        Window_Hanning(realVec.data(), gargs.OFullSize, gargs.windowSize);
        break;
    }

    switch (windowSizeEXP) {
    case 6:
        Stockhoptimized6(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    case 7:
        Stockhoptimized7(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    case 8:
        Stockhoptimized8(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    case 9:
        Stockhoptimized9(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    case 10:
        Stockhoptimized10(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    case 11:
        Stockhoptimized11(realVec.data(), imagVec.data(), gargs.OHalfSize);
        break;
    default: {
        std::vector<float> subrealVec(gargs.OFullSize, 0.0f);
        std::vector<float> subimagVec(gargs.OFullSize, 0.0f);
        const unsigned int HWindowSize = gargs.windowSize >> 1;
        for (unsigned int stage = 0; stage < windowSizeEXP; ++stage) {
            if (stage % 2 == 0) {
                StockHamCommon(realVec.data(),
                               imagVec.data(),
                               subrealVec.data(),
                               subimagVec.data(),
                               HWindowSize,
                               stage,
                               gargs.OHalfSize,
                               windowSizeEXP);
            } else {
                StockHamCommon(subrealVec.data(),
                               subimagVec.data(),
                               realVec.data(),
                               imagVec.data(),
                               HWindowSize,
                               stage,
                               gargs.OHalfSize,
                               windowSizeEXP);
            }
        }
        if (windowSizeEXP % 2 != 0) {
            realVec = std::move(subrealVec);
            imagVec = std::move(subimagVec);
        }
    }
    }

    return std::pair(std::move(realVec), std::move(imagVec));
}
}; // namespace PDJE_PARALLEL
