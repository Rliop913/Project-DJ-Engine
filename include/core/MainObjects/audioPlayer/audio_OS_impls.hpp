#pragma once
#include "PDJE_LOG_SETTER.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>

#include <miniaudio.h>
#include <vector>
#if defined(_WIN32)
// #include <Windows.h>
// #include <mmdeviceapi.h>
#include <Audioclient.h>
// #pragma comment(lib, "Ole32.lib")
// #pragma comment(lib, "Uuid.lib")
#elif defined(__APPLE__)

#else
#include <alsa/asoundlib.h>
#include <jack/jack.h>
#include <jack/types.h>
#include <pulse/pulseaudio.h>
#include <pulse/stream.h>
// #include <sound/asound.h>
#endif

namespace OS_IMPL {
static std::vector<ma_backend>
get_backends()
{
    std::vector<ma_backend> backs;
#if defined(_WIN32)
    backs.push_back(ma_backend_wasapi);
#elif defined(__APPLE__)
    backs.push_back(ma_backend_coreaudio);
#else
    backs.push_back(ma_backend_alsa);
#endif
    return backs;
}

static ma_ptr
extract_backend(const ma_device &dev)
{
#if defined(_WIN32)
    return dev.wasapi.pAudioClientPlayback;

#elif defined(__APPLE__)
    return dev.coreaudio.pAudioUnit; // fix it
#else

    switch (dev.pContext->backend) {
    case ma_backend_alsa:
        return dev.alsa.pPCMPlayback;
    default:
        return nullptr;
    }

#endif
}

static std::function<uint32_t(const ma_ptr &)>
set_unused_frame_function(const ma_device &dev)
{
#if defined(_WIN32)
    return [](const ma_ptr &ptr) -> uint32_t {
        UINT32 padding_frames;
        if (FAILED(reinterpret_cast<IAudioClient *>(ptr)->GetCurrentPadding(
                &padding_frames))) {
            critlog("failed to get remain buffer size");
            return 0;
        }
        return static_cast<uint32_t>(padding_frames);
    };
#elif defined(__APPLE__)
    return [](const ma_ptr &ptr) {};
#else
    switch (dev.pContext->backend) {
    case ma_backend_alsa:
        return [](const ma_ptr &ptr) -> uint32_t {
            try {
                snd_pcm_sframes_t delayval;
                snd_pcm_delay(static_cast<snd_pcm_t *>(ptr), &delayval);
                return delayval;
            } catch (const std::exception &e) {
                critlog("failed to get delay on alsa backend. What: ");
                critlog(e.what());
                return 0;
            }
        };
    default:
        return [](const ma_ptr &ptr) -> uint32_t { return 0; };
        break;
    }

#endif
}
}; // namespace OS_IMPL