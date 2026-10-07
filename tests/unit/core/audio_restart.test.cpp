#include "audioCallbacks.hpp"
#include <doctest/doctest.h>

#include <atomic>
#include <memory>
#include <vector>

TEST_CASE(
    "core/audio: pre-render restart resets progress without replacing PCM")
{
    std::vector<float> pcm(32, 0.25f);
    // Embedded DSP state exceeds the default Windows test-thread stack.
    auto  owner       = std::make_unique<audioEngineDataStruct>();
    auto &data        = *owner;
    data.pcmDataPoint = &pcm;
    data.maxCursor    = pcm.size() / 2;
    data.nowCursor    = 12;
    data.cacheSync    = audioSyncData{ .consumed_frames              = 120,
                                       .pre_calculated_unused_frames = 8,
                                       .microsecond                  = 123456 };
    data.syncData.store(data.cacheSync, std::memory_order_release);
    auto *const sync_address   = &data.syncData;
    auto *const cursor_address = &data.nowCursor;
    auto *const pcm_address    = pcm.data();

    static_assert(noexcept(data.ResetPrerenderProgress()));
    for (int attempt = 0; attempt != 2; ++attempt) {
        data.ResetPrerenderProgress();
        const auto sync = data.syncData.load(std::memory_order_acquire);
        CHECK(data.nowCursor == 0);
        CHECK(data.cacheSync.consumed_frames == 0);
        CHECK(data.cacheSync.pre_calculated_unused_frames == 0);
        CHECK(data.cacheSync.microsecond == 0);
        CHECK(sync.consumed_frames == 0);
        CHECK(sync.pre_calculated_unused_frames == 0);
        CHECK(sync.microsecond == 0);
        CHECK(&data.syncData == sync_address);
        CHECK(&data.nowCursor == cursor_address);
        CHECK(data.pcmDataPoint == &pcm);
        CHECK(pcm.data() == pcm_address);
        CHECK(data.maxCursor == 16);
        CHECK(pcm.front() == 0.25f);
        CHECK_FALSE(data.MusCtrPanel.has_value());
        CHECK_FALSE(data.FXManualPanel.has_value());
    }
}
