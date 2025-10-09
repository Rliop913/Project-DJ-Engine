#pragma once

#include "CapnpBinary.hpp"
#include "NoteBinary.capnp.h"
#include <optional>
#include <string>
#include <vector>

#include "FrameCalc.hpp"

#include "PDJE_EXPORT_SETTER.hpp"
#include <functional>
#include "PDJE_OBJ_SETTER.hpp"


/**
 * @brief the note translator.
 * this class reads the capnp binary datas and bpm datas, and parse them.
 */
class PDJE_API NoteTranslator {
  public:
    /// the note object's bpm datas
    BpmStruct noteBpms;
    /**
     * @brief Reads and parse capnp object.
     * this function reads capnp binary data and note object's bpm data and
     * track data's bpm data. after parse all of them, it calls the note object
     * setter callback.
     * @param binary the capnp binary Reader object
     * @param mainBpm the track data's bpm data
     * @param lambdaCallback the object setter lambda
     * @return true
     * @return false
     */
    bool
    Read(const CapReader<NoteBinaryCapnpData> &binary,
         const BpmStruct                      &mainBpm,
         OBJ_SETTER_CALLBACK                  &lambdaCallback);

    NoteTranslator()  = default;
    ~NoteTranslator() = default;
};