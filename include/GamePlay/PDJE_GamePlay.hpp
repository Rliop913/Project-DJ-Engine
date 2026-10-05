#pragma once

#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_Judge_Init_Structs.hpp"
#include "PDJE_Rule.hpp"
#include "trackDB.hpp"
#include <cstdint>
#include <libremidi/libremidi.hpp>
#include <memory>
#include <string>
#include <vector>

class PDJE;
class PDJE_Input;
namespace PDJE_JUDGE {
class JUDGE;
}

namespace PDJE_GAMEPLAY {

/** @brief Chart source for the already initialized Core player. */
struct CoreReady {
    // Supply the track corresponding to the loaded music; no player reinit.
    trackdata track;
};

/** @brief Selected devices for PDJE_Input::Config; does not start capture. */
struct InputReady {
    // Leave devices empty for MIDI-only gameplay.
    std::vector<DeviceData>            devices;
    std::vector<libremidi::input_port> midi_devices;
};

/** @brief Rail mappings, judgment windows and callbacks for this session. */
struct JudgeReady {
    struct KeyboardMouseRail {
        DeviceData device{ PDJE_Dev_Type::UNKNOWN, {}, {} };
        BITMASK    device_key         = 0;
        int64_t    offset_microsecond = 0;
        uint64_t   rail_id            = 0;
    };

    struct MidiRail {
        std::string port_name;
        uint8_t     type               = 0;
        uint8_t     ch                 = 0; // Zero-based MIDI channel.
        uint8_t     pos                = 0;
        int64_t     offset_microsecond = 0;
        uint64_t    rail_id            = 0;
    };

    std::vector<KeyboardMouseRail> keyboard_mouse_rails;
    std::vector<MidiRail>          midi_rails;
    PDJE_JUDGE::EVENT_RULE         event_rule{};
    // Includes callback worker intervals and MIDI CC pairing mode.
    PDJE_JUDGE::Custom_Events events{};
};

/**
 * @brief High-level Core/Input/Judge orchestration scaffold.
 *
 * Intended flow: externally initialize modules -> Ready -> Play -> controls.
 * Ready configures a stopped session; Play/End start and tear it down.
 * Pause/Resume/Restart are still no-ops.
 *
 * Modules are shared-owned and supplied at construction; null is rejected.
 * Construction checks non-null ownership; Play checks preparation and
 * attachment. Serialize lifecycle calls and external module changes on a
 * control thread. Do not call lifecycle methods or destroy the facade from
 * Judge/audio callbacks. Shared ownership does not prevent external teardown or
 * provide synchronization. Do not replace/reset the player, input buffers, or
 * Judge configuration while playing. Call End before releasing producers or
 * destroying an active facade; the default destructor does not orchestrate
 * shutdown of externally held modules.
 */
class PDJE_API FACADE {
  private:
    // Reverse destruction order releases Judge before Input and Core.

    std::shared_ptr<PDJE>              core;
    std::shared_ptr<PDJE_Input>        input;
    std::shared_ptr<PDJE_JUDGE::JUDGE> judge;

  public:
    /** @throws std::invalid_argument if any module is null. */
    FACADE(std::shared_ptr<PDJE>              core,
           std::shared_ptr<PDJE_Input>        input,
           std::shared_ptr<PDJE_JUDGE::JUDGE> judge);

    ~FACADE() = default;

    FACADE(const FACADE &) = delete;
    FACADE &
    operator=(const FACADE &) = delete;

    /** @brief Return shared ownership of Core. */
    std::shared_ptr<PDJE>
    GetCore() const noexcept;
    /** @brief Return shared ownership of Input. */
    std::shared_ptr<PDJE_Input>
    GetInput() const noexcept;
    /** @brief Return shared ownership of Judge. */
    std::shared_ptr<PDJE_JUDGE::JUDGE>
    GetJudge() const noexcept;

    /** @brief Prepare chart, rails, callbacks and input without starting
     * playback. Requires a stopped initialized player/Judge, and Input just
     * initialized in DEVICE_CONFIG_STATE. The player's data line must satisfy
     * native Judge's all-four-pointer contract. Does not initialize, rewind or
     * replace Core. Registers rails before collecting notes; unmapped notes are
     * ignored as in Judge_Init. Empty decoded charts are allowed. Copies
     * settings/callbacks; does not retain references to the three configuration
     * objects. Sets Input CC pairing to events.midi_cc_lsb_on, then attaches
     * data lines. Success leaves Input READY and Judge stopped. Repeated Ready
     * is rejected; End and reinitialize Input before preparing another session.
     * @throws std::logic_error for invalid lifecycle/Core attachment.
     * @throws std::invalid_argument for invalid settings.
     * @throws std::runtime_error on chart loading or Input configuration
     * failure. Validation/chart staging failures leave existing module setup
     * unchanged. Once Input configuration begins, failure attempts End()
     * instead of rollback; reinitialize Input before retry. Incomplete cleanup
     * is reported as nested.
     */
    void
    Ready(const CoreReady  &core_ready,
          const InputReady &input_ready,
          const JudgeReady &judge_ready);
    /** @brief Start configured Input, then Judge, then the whole Core player.
     * Rejects missing preparation, mismatched data lines and already-running
     * modules without changing them. Clears the old sync timestamp; does not
     * seek. Returns after device startup, not after the first fresh audio sync
     * sample.
     * @throws std::logic_error for invalid preparation (no teardown).
     * @throws std::runtime_error for startup failure. After startup begins,
     * failures attempt End(); reinitialize Input and configure Judge before
     * retry. Incomplete cleanup is reported with a nested exception; retry End
     * first.
     */
    void
    Play();

    /** @brief Placeholder for pausing the whole player and judgment production.
     */
    void
    Pause();
    /** @brief Placeholder for synchronized playback and judgment resumption. */
    void
    Resume();
    /** @brief Placeholder for returning to ready; requires an explicit Play().
     */
    void
    Restart();
    /** @brief Join Judge, stop Core, then release Input. Repeat calls are safe.
     * Clears Judge setup and returns Input to DEAD; does not rewind Core.
     * Uses Judge::End semantics: pending use/miss jobs need not all be
     * delivered. If Judge join fails, producers are left alive. Otherwise Input
     * cleanup is attempted even when Core shutdown fails. Shutdown failures are
     * propagated. Not a pause or restart; subsequent playback requires
     * preparation again.
     */
    void
    End();
};

} // namespace PDJE_GAMEPLAY
