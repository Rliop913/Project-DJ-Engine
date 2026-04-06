#include "PDJE_interface.hpp"
#include "util/function/image/WaveformWebp.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr std::array<char, 4> kRiffSignature{ 'R', 'I', 'F', 'F' };
constexpr std::array<char, 4> kWebPSignature{ 'W', 'E', 'B', 'P' };

std::optional<fs::path>
find_project_root()
{
    std::error_code ec;

    fs::path probe = fs::current_path(ec);
    if (!ec) {
        for (int i = 0; i < 8; ++i) {
            if (fs::is_directory(probe / "DMCA_FREE_DEMO_MUSIC", ec)) {
                return probe;
            }
            if (!probe.has_parent_path()) {
                break;
            }
            probe = probe.parent_path();
        }
    }

    probe = fs::absolute(fs::path(__FILE__), ec).parent_path();
    if (!ec) {
        for (int i = 0; i < 8; ++i) {
            if (fs::is_directory(probe / "DMCA_FREE_DEMO_MUSIC", ec)) {
                return probe;
            }
            if (!probe.has_parent_path()) {
                break;
            }
            probe = probe.parent_path();
        }
    }

    return std::nullopt;
}

std::optional<fs::path>
pick_demo_music(const fs::path &project_root)
{
    const fs::path  demo_dir  = project_root / "DMCA_FREE_DEMO_MUSIC";
    const fs::path  preferred = demo_dir / "over.wav";
    std::error_code ec;

    if (fs::is_regular_file(preferred, ec)) {
        return preferred;
    }

    std::vector<fs::path> candidates;
    for (const auto &entry : fs::directory_iterator(demo_dir, ec)) {
        if (ec) {
            return std::nullopt;
        }
        if (!entry.is_regular_file(ec)) {
            continue;
        }
        if (entry.path().extension() == ".wav") {
            candidates.push_back(entry.path());
        }
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    std::sort(candidates.begin(), candidates.end());
    return candidates.front();
}

bool
write_binary_file(const fs::path &path, const std::vector<std::uint8_t> &bytes)
{
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);
    if (ec) {
        return false;
    }

    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    output.write(reinterpret_cast<const char *>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
    return output.good();
}

bool
has_webp_signature(const std::vector<std::uint8_t> &bytes)
{
    if (bytes.size() < 12) {
        return false;
    }

    return std::equal(kRiffSignature.begin(),
                      kRiffSignature.end(),
                      reinterpret_cast<const char *>(bytes.data())) &&
           std::equal(kWebPSignature.begin(),
                      kWebPSignature.end(),
                      reinterpret_cast<const char *>(bytes.data() + 8));
}

} // namespace

class SectionTimer {
  public:
    using Clock     = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration  = std::chrono::duration<double, std::milli>;

    SectionTimer(std::string name)
        : name_(std::move(name)), start_(Clock::now())
    {
    }

    ~SectionTimer()
    {
        auto elapsed = Duration(Clock::now() - start_).count();
        std::cout << "[TIMING] " << name_ << ": " << std::fixed
                  << std::setprecision(2) << elapsed << " ms" << std::endl;
    }

    double
    elapsed_ms() const
    {
        return Duration(Clock::now() - start_).count();
    }

  private:
    std::string name_;
    TimePoint   start_;
};

void
print_timing_summary(double      pcm_decode_ms,
                     double      waveform_encode_ms,
                     double      file_write_ms,
                     std::size_t pcm_samples,
                     std::size_t file_count)
{
    std::cout << "\n"
              << "==============================================\n"
              << "          WAVEFORM GENERATION TIMING          \n"
              << "==============================================\n"
              << std::fixed << std::setprecision(2)
              << "  PCM Decoding:      " << std::setw(10) << pcm_decode_ms
              << " ms\n"
              << "  Waveform Encode:   " << std::setw(10) << waveform_encode_ms
              << " ms\n"
              << "  File Write:        " << std::setw(10) << file_write_ms
              << " ms\n"
              << "  -----------------------------------------\n"
              << "  Total Time:        " << std::setw(10)
              << (pcm_decode_ms + waveform_encode_ms + file_write_ms) << " ms\n"
              << "==============================================\n"
              << "\n"
              << "  PCM Samples:       " << pcm_samples << "\n"
              << "  Files Generated:    " << file_count << "\n"
              << "  Avg Time per File: " << std::setprecision(2)
              << (file_count > 0 ? file_write_ms / file_count : 0) << " ms\n"
              << "==============================================\n"
              << std::endl;
}

int
main()
{
    const auto project_root = find_project_root();
    if (!project_root.has_value()) {
        std::cerr << "failed: could not locate project root containing "
                     "DMCA_FREE_DEMO_MUSIC"
                  << std::endl;
        return 1;
    }

    const auto music_path = pick_demo_music(project_root.value());
    if (!music_path.has_value()) {
        std::cerr << "failed: no demo wav file found in "
                  << (project_root.value() / "DMCA_FREE_DEMO_MUSIC").string()
                  << std::endl;
        return 1;
    }

    const fs::path output_dir =
        fs::current_path() / "music_to_waveform_webp_output";
    const fs::path db_root = output_dir / "music_to_waveform_webp_db";

    PDJE engine(db_root.string());

    musdata source_music;
    source_music.musicPath = music_path->string();

    double pcm_decode_ms = 0.0;
    {
        SectionTimer timer("PCM Decoding");
        auto         pcm = engine.GetPCMFromMusData(source_music);
        pcm_decode_ms    = timer.elapsed_ms();

        if (pcm.empty()) {
            std::cerr << "failed: could not decode pcm from "
                      << music_path->string() << std::endl;
            return 1;
        }

        constexpr std::size_t kChannelCount    = 2;
        constexpr std::size_t kYPixels         = 512;
        constexpr std::size_t kPcmPerPixel     = 48;
        constexpr std::size_t kXPixelsPerImage = 4096;

        const PDJE_UTIL::function::image::EncodeWaveformWebpArgs waveform_args{
            .pcm                = pcm,
            .channel_count      = kChannelCount,
            .y_pixels           = kYPixels,
            .pcm_per_pixel      = kPcmPerPixel,
            .x_pixels_per_image = kXPixelsPerImage,
            .compression_level  = 3,
        };

        const PDJE_UTIL::function::image::EncodeWaveformWebpStftArgs stft_args{
            .target_window   = PDJE_PARALLEL::WINDOW_LIST::HANNING,
            .window_size_exp = 10,
            .overlap_ratio   = 0.5f,
            .post_process    = {},
        };

        double waveform_encode_ms = 0.0;
        {
            SectionTimer timer("Waveform Encoding (RGB STFT)");
            auto waveform = PDJE_UTIL::function::image::encode_waveform_webps(
                waveform_args, stft_args);
            waveform_encode_ms = timer.elapsed_ms();

            if (!waveform.ok()) {
                std::cerr << "failed: waveform encode error: "
                          << waveform.status().message << std::endl;
                return 1;
            }

            double file_write_ms = 0.0;
            {
                SectionTimer      timer("File Writing");
                std::size_t       file_count = 0;
                const std::string music_stem = music_path->stem().string();
                const auto       &batch      = waveform.value();
                for (std::size_t channel_index = 0;
                     channel_index < batch.size();
                     ++channel_index) {
                    for (std::size_t image_index = 0;
                         image_index < batch[channel_index].size();
                         ++image_index) {
                        const auto &webp_bytes =
                            batch[channel_index][image_index];
                        if (!has_webp_signature(webp_bytes)) {
                            std::cerr << "failed: generated bytes are not a "
                                         "webp for channel "
                                      << channel_index << " index "
                                      << image_index << std::endl;
                            return 1;
                        }

                        const fs::path output_path =
                            output_dir /
                            (music_stem + "_channel_" +
                             std::to_string(channel_index) + "_part_" +
                             std::to_string(image_index) + ".webp");

                        if (!write_binary_file(output_path, webp_bytes)) {
                            std::cerr << "failed: could not write "
                                      << output_path.string() << std::endl;
                            return 1;
                        }

                        ++file_count;
                    }
                }
                file_write_ms = timer.elapsed_ms();

                if (file_count == 0) {
                    std::cerr
                        << "failed: waveform encoder returned no webp files"
                        << std::endl;
                    return 1;
                }

                print_timing_summary(pcm_decode_ms,
                                     waveform_encode_ms,
                                     file_write_ms,
                                     pcm.size(),
                                     file_count);
            }
        }
    }

    std::cout << "source: " << music_path->string() << std::endl;
    std::cout << "proof: first generated payload passed webp signature check"
              << std::endl;

    return 0;
}
