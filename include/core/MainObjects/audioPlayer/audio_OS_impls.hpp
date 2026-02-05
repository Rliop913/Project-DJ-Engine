#pragma once
#include <vector>
#include <miniaudio.h>

#if defined(_WIN32)
// #include <Windows.h>
// #include <mmdeviceapi.h>
#include <Audioclient.h>
// #pragma comment(lib, "Ole32.lib")
// #pragma comment(lib, "Uuid.lib")
#elif defined(__APPLE__)

#else

#endif




namespace OS_IMPL{
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
        backs.push_back(ma_backend_jack);
        backs.push_back(ma_backend_pulseaudio);
#endif
        return backs;
    }

static ma_ptr
extract_backend(const ma_device& dev)
{
    #if defined(_WIN32)
        return dev.wasapi.pAudioClientPlayback;
        
#elif defined(__APPLE__)
        return dev.coreaudio.pAudioUnit;//fix it
#else
        return dev.alsa.pDevice;//fix it

#endif

}

    static std::function<uint32_t(const ma_ptr&)>
    set_unused_frame_function()
    {
            #if defined(_WIN32)
            return [](const ma_ptr& dev)-> uint32_t{
                UINT32 padding_frames;
                if(FAILED(reinterpret_cast<IAudioClient*>(dev)->GetCurrentPadding(&padding_frames))){
                    critlog("failed to get remain buffer size");
                    return 0;
                }
                return static_cast<uint32_t>(padding_frames);
            };
            #elif defined(__APPLE__)
                return [](const ma_ptr& dev){
                };
            #else
            return [](const ma_ptr& dev){};
            #endif
    }
};