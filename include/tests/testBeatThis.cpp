#include "util/ai/beat_this/BeatThis.hpp"

#include "miniaudio.h"

#include <cstddef>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace {

struct CliArgs {
    fs::path                audio_path;
    std::optional<fs::path> model_path;
    bool                    show_help = false;
};

struct DecodedStereoPcm {
    std::vector<float> samples;
    int                sample_rate = 0;
    ma_uint32          channels    = 0;
};

class DecoderHandle {
  public:
    DecoderHandle() = default;

    ~DecoderHandle()
    {
        if (initialized_) {
            ma_decoder_uninit(&decoder_);
        }
    }

    DecoderHandle(const DecoderHandle &)            = delete;
    DecoderHandle &operator=(const DecoderHandle &) = delete;

    ma_decoder *
    init_file(const fs::path &audio_path)
    {
        ma_decoder_config config =
            ma_decoder_config_init(ma_format_f32, 2, 0);
        const ma_result result =
            ma_decoder_init_file(audio_path.string().c_str(), &config, &decoder_);
        if (result != MA_SUCCESS) {
            throw std::runtime_error("could not initialize decoder (error code " +
                                     std::to_string(static_cast<int>(result)) +
                                     ")");
        }

        initialized_ = true;
        return &decoder_;
    }

  private:
    ma_decoder decoder_     = {};
    bool       initialized_ = false;
};

void
print_usage(std::ostream &out, const std::string_view program_name)
{
    out << "Usage: " << program_name << " [--model MODEL_PATH] AUDIO_PATH\n"
        << "\n"
        << "Run the BeatThis detector against one audio file.\n"
        << "\n"
        << "Options:\n"
        << "  --model MODEL_PATH   Override the default BeatThis ONNX model path.\n"
        << "  --help               Show this help text.\n";
}

bool
parse_args(const int argc, char **argv, CliArgs &args, std::string &error)
{
    if (argc <= 1) {
        error = "missing AUDIO_PATH.";
        return false;
    }

    for (int idx = 1; idx < argc; ++idx) {
        const std::string_view arg = argv[idx];
        if (arg == "--help") {
            args.show_help = true;
            return true;
        }

        if (arg == "--model") {
            ++idx;
            if (idx >= argc) {
                error = "--model requires a path value.";
                return false;
            }

            if (args.model_path.has_value()) {
                error = "--model can only be provided once.";
                return false;
            }

            args.model_path = fs::path(argv[idx]);
            continue;
        }

        if (!arg.empty() && arg.front() == '-') {
            error = "unknown option: " + std::string(arg);
            return false;
        }

        if (!args.audio_path.empty()) {
            error = "only one AUDIO_PATH is supported.";
            return false;
        }

        args.audio_path = fs::path(arg);
    }

    if (args.audio_path.empty()) {
        error = "missing AUDIO_PATH.";
        return false;
    }

    return true;
}

bool
validate_input_path(const fs::path &path, const char *label)
{
    std::error_code ec;
    if (!fs::exists(path, ec) || ec) {
        std::cerr << "failed: " << label << " does not exist: " << path.string()
                  << std::endl;
        return false;
    }

    if (!fs::is_regular_file(path, ec) || ec) {
        std::cerr << "failed: " << label << " is not a regular file: "
                  << path.string() << std::endl;
        return false;
    }

    return true;
}

DecodedStereoPcm
decode_stereo_pcm(const fs::path &audio_path)
{
    DecoderHandle decoder_handle;
    ma_decoder   *decoder = decoder_handle.init_file(audio_path);

    ma_format  format      = ma_format_unknown;
    ma_uint32  channels    = 0;
    ma_uint32  sample_rate = 0;
    const auto format_result =
        ma_decoder_get_data_format(decoder,
                                   &format,
                                   &channels,
                                   &sample_rate,
                                   nullptr,
                                   0);
    if (format_result != MA_SUCCESS) {
        throw std::runtime_error("could not query decoder output format");
    }
    if (format != ma_format_f32) {
        throw std::runtime_error("decoder output format was not float32");
    }
    if (channels != 2) {
        throw std::runtime_error("decoder output channel count was not stereo");
    }
    if (sample_rate == 0) {
        throw std::runtime_error("decoder reported an invalid sample rate");
    }

    std::vector<float> pcm;
    ma_uint64          total_frames = 0;
    if (ma_decoder_get_length_in_pcm_frames(decoder, &total_frames) ==
            MA_SUCCESS &&
        total_frames > 0) {
        const auto max_frames =
            std::numeric_limits<std::size_t>::max() / channels;
        if (total_frames > max_frames) {
            throw std::runtime_error("audio file is too large to decode");
        }

        pcm.reserve(static_cast<std::size_t>(total_frames) * channels);
    }

    constexpr ma_uint64 chunk_frames = 4096;
    std::vector<float>  chunk(static_cast<std::size_t>(chunk_frames) * channels,
                             0.0f);

    while (true) {
        ma_uint64 frames_read = 0;
        const auto read_result = ma_decoder_read_pcm_frames(
            decoder, chunk.data(), chunk_frames, &frames_read);
        if (read_result != MA_SUCCESS) {
            throw std::runtime_error("could not decode PCM frames");
        }
        if (frames_read == 0) {
            break;
        }

        const auto sample_count = static_cast<std::size_t>(frames_read) * channels;
        pcm.insert(pcm.end(), chunk.begin(), chunk.begin() + sample_count);
    }

    if (pcm.empty()) {
        throw std::runtime_error("decoded PCM buffer was empty");
    }

    return DecodedStereoPcm{ .samples     = std::move(pcm),
                             .sample_rate = static_cast<int>(sample_rate),
                             .channels    = channels };
}

std::vector<float>
downmix_to_mono(const DecodedStereoPcm &stereo_pcm)
{
    if (stereo_pcm.samples.empty()) {
        throw std::runtime_error("stereo PCM buffer was empty");
    }

    if (stereo_pcm.channels != 2) {
        throw std::runtime_error("decoded PCM buffer was not stereo");
    }

    if ((stereo_pcm.samples.size() % stereo_pcm.channels) != 0u) {
        throw std::runtime_error(
            "decoded PCM buffer was not a valid interleaved stereo stream");
    }

    std::vector<float> mono(stereo_pcm.samples.size() / stereo_pcm.channels,
                            0.0f);
    for (std::size_t frame_idx = 0; frame_idx < mono.size(); ++frame_idx) {
        const std::size_t stereo_idx = frame_idx * stereo_pcm.channels;
        mono[frame_idx] = (stereo_pcm.samples[stereo_idx] +
                           stereo_pcm.samples[stereo_idx + 1u]) *
                          0.5f;
    }

    return mono;
}

PDJE_UTIL::ai::BeatDetectionResult
run_detector(const CliArgs           &args,
             const std::vector<float> &mono_pcm,
             const int                input_sample_rate,
             fs::path                &resolved_model_path)
{
    if (args.model_path.has_value()) {
        PDJE_UTIL::ai::BeatThisDetector detector(args.model_path.value());
        resolved_model_path = detector.model_path();
        return detector.detect(mono_pcm, input_sample_rate);
    }

    PDJE_UTIL::ai::BeatThisDetector detector;
    resolved_model_path = detector.model_path();
    return detector.detect(mono_pcm, input_sample_rate);
}

void
print_times(const std::string_view label, const std::vector<double> &times)
{
    std::cout << label << " (" << times.size() << ")";
    if (times.empty()) {
        std::cout << ": none" << std::endl;
        return;
    }

    std::cout << ":" << std::endl;
    for (std::size_t idx = 0; idx < times.size(); ++idx) {
        std::cout << "  [" << idx << "] " << std::fixed << std::setprecision(6)
                  << times[idx] << " s" << std::endl;
    }
}

} // namespace

int
main(const int argc, char **argv)
{
    const std::string_view program_name =
        (argc > 0 && argv[0] != nullptr) ? std::string_view(argv[0])
                                         : std::string_view("testBeatThis");

    CliArgs     args;
    std::string error;
    if (!parse_args(argc, argv, args, error)) {
        std::cerr << "failed: " << error << std::endl;
        print_usage(std::cerr, program_name);
        return 1;
    }

    if (args.show_help) {
        print_usage(std::cout, program_name);
        return 0;
    }

    if (!validate_input_path(args.audio_path, "audio path")) {
        return 1;
    }
    if (args.model_path.has_value() &&
        !validate_input_path(args.model_path.value(), "model path")) {
        return 1;
    }

    try {
        const DecodedStereoPcm stereo_pcm = decode_stereo_pcm(args.audio_path);
        const std::vector<float> mono_pcm = downmix_to_mono(stereo_pcm);

        fs::path resolved_model_path;
        const auto detection_result = run_detector(
            args, mono_pcm, stereo_pcm.sample_rate, resolved_model_path);

        std::cout << "BeatThis CLI probe succeeded." << std::endl;
        std::cout << "audio path: " << args.audio_path.string() << std::endl;
        std::cout << "model path: " << resolved_model_path.string() << std::endl;
        std::cout << "decoded sample rate: " << stereo_pcm.sample_rate << " Hz"
                  << std::endl;
        std::cout << "decoded stereo samples: " << stereo_pcm.samples.size()
                  << std::endl;
        std::cout << "decoded stereo frames: "
                  << (stereo_pcm.samples.size() / stereo_pcm.channels)
                  << std::endl;
        std::cout << "mono samples: " << mono_pcm.size() << std::endl;

        print_times("beats", detection_result.beats);
        print_times("downbeats", detection_result.downbeats);
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "failed: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "failed: unknown exception" << std::endl;
        return 1;
    }
}
