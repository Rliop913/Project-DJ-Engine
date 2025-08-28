#pragma once

#include "ManualMix.hpp"
#include "MixMachine.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
/**
 * @brief Top-level class for audio rendering
 *
 * \dot
 * digraph audioRender{
 *      audioRender -> MixMachine;
 *      MixMachine -> MixTranslator;
 *      MixTranslator -> CapReader;
 *      CapReader -> MixBinaryCapnpData;
 * }
 * \enddot
 *
 */
class PDJE_API audioRender {
  public:
    std::optional<std::vector<float>> rendered_frames;

    /**
     * @brief Loads Track Data
     *
     * @param db RootDB
     * @param td trackdata
     * @return true
     * @return false
     */
    bool
    LoadTrack(litedb &db, trackdata &td);
    /**
     * @brief Loads Track Data from a MixData object.
     *
     * @param db The database instance.
     * @param mixData The binary mix data.
     * @return true if the track data was loaded successfully, false otherwise.
     */
    bool
    LoadTrackFromMixData(litedb &db, BIN &mixData);
    audioRender()  = default;
    ~audioRender() = default;
};

enum ITPL_ENUM {
  ITPL_LINEAR =0,
  ITPL_COSINE,
  ITPL_CUBIC,
  ITPL_FLAT
};
