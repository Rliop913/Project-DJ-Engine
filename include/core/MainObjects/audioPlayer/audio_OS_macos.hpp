#pragma once

#ifdef PDJE_MACOS

#include <CoreAudio/CoreAudio.h>
#include <optional>

namespace OS_IMPL {

static std::optional<Float64>
get_macos_sample_time(AudioDeviceID device) noexcept
{
    AudioTimeStamp timestamp{};
    if (AudioDeviceGetCurrentTime(device, &timestamp) != noErr ||
        (timestamp.mFlags & kAudioTimeStampSampleTimeValid) == 0) {
        return std::nullopt;
    }
    return timestamp.mSampleTime;
}

} // namespace OS_IMPL

#endif
