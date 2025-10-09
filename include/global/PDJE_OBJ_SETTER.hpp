#pragma once
#include <functional>
#include <string>
/** @brief note object setter callback
 *  >>>REMEMBER<<<
 * ======================================================================================================================
 * The function passes only the arguments for creating an object.
 * Created objects must be stored in global variables or variables captured by
 *reference when creating a lambda function.
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
 *7 Y axis position 2, the second logical position of note, use for long notes
 *or else
 *
 */
using OBJ_SETTER_CALLBACK = std::function<void(
    /// noteType
    const std::string,
    /// note Detail
    const std::string,
    /// firstArg
    const std::string,
    /// secondArg
    const std::string,
    /// thirdArg
    const std::string,
    /// Y axis position
    const unsigned long long,
    /// Y axis position 2, 0 as default
    const unsigned long long)>;