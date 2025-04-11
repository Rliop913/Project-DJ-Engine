#pragma once

#include <optional>
#include <vector>

#include "CapnpBinary.hpp"
#include "NoteBinary.capnp.h"

#include "FrameCalc.hpp"

#include <functional>


/** @brief note object setter callback
 *  >>>REMEMBER<<<
 * ======================================================================================================================
 * The function passes only the arguments for creating an object.
 * Created objects must be stored in global variables or variables captured by reference when creating a lambda function.
 * ======================================================================================================================
 * Args describe
 *
 *
 * 1 noteType, the note type. e.g) hold, tap, slide
 *
 * 2 noteDetail, the detail
 *
 * 3 firstArg
 *
 * 4 secondArg
 *
 * 5 thirdArg
 *
 *
 * 1~5 these are meta datas for note.
 *
 *6 Y axis position, the first logical position of note
 *7 Y axis position 2, the second logical position of note, use for long notes or else
 *
 */
using OBJ_SETTER_CALLBACK = 
std::function<void(
    ///noteType
    const std::string,
    ///note Detail
    const std::string, 
    ///firstArg
    const std::string, 
    ///secondArg
    const std::string, 
    ///thirdArg
    const std::string, 
    ///Y axis position
    const unsigned long long,
    ///Y axis position 2, 0 as default
    const unsigned long long
)>;


/**
 * @brief the note translator.
 * this class reads the capnp binary datas and bpm datas, and parse them.
 */
class NoteTranslator{
public:
    /// the note object's bpm datas
    BpmStruct noteBpms;
    /**
     * @brief Reads and parse capnp object.
     * this function reads capnp binary data and note object's bpm data and track data's bpm data.
     * after parse all of them, it calls the note object setter callback.
     * @param binary the capnp binary Reader object
     * @param mainBpm the track data's bpm data
     * @param lambdaCallback the object setter lambda
     * @return true 
     * @return false 
     */
    bool Read(
        const CapReader<NoteBinaryCapnpData>& binary, 
        const BpmStruct& mainBpm,
        OBJ_SETTER_CALLBACK& lambdaCallback);

    NoteTranslator() = default;
    ~NoteTranslator() = default;
};