#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>

#include "dbState.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "fileNameSanitizer.hpp"

/**
 * @brief the music meta data's struct
 * 
 */
struct PDJE_API musdata{
public:
    SANITIZED title;
    SANITIZED composer;
    SANITIZED_ORNOT musicPath;
    BIN bpmBinary;
    double bpm = -1.0;
    /// @brief this means the first beat's position. The measurement unit is a PCM frame. 
    /// For higher precision, double value is stored as strings in Cap’n Proto, so the corresponding variable is also a string.
    DONT_SANITIZE firstBar;
    /**
     * @brief Constructor. typically used for returning search results.
     * 
     * @param dbstate the sqlite statement
     */
    musdata(stmt* dbstate);

    /**
     * @brief Constructor used for filtering music entries.
     * 
     * Use this constructor to specify filtering conditions for a search query.
     * Leave parameters blank (or set to a negative value for bpm) if filtering by that field is not required.
     *
     * @param title__ The title of the music.
     * @param composer__ The composer of the music.
     * @param musicPath__ Path to the music file, relative to the root of the database.
     * @param bpm__ The BPM (beats per minute) of the music. Use a negative value to ignore this field.
     */
    musdata(
        const UNSANITIZED& title__ = "",
        const UNSANITIZED& composer__ = "",
        const SANITIZED_ORNOT& musicPath__ = "",
        const double bpm__ = -1.0
    );
    /**
     * @brief generates the search statement
     * 
     * @param dbstate statement pointer
     * @param db Root DB
     * @return true 
     * @return false 
     */
    bool GenSearchSTMT(stmt& dbstate, sqlite3* db);

    /**
     * @brief generates the insert statement
     * 
     * @param dbstate statement pointer
     * @param db Root DB
     * @return true 
     * @return false 
     */
    bool GenInsertSTMT(stmt& dbstate, sqlite3* db);

    /**
     * @brief Generates the edit statement.
     *
     * @param dbstate The statement pointer.
     * @param db The root database.
     * @param toEdit The music data to edit.
     * @return `true` if the statement was generated successfully, `false` otherwise.
     */
    bool GenEditSTMT(stmt& dbstate, sqlite3* db, musdata& toEdit);

    /**
     * @brief Generates the delete statement.
     *
     * @param dbstate The statement pointer.
     * @param db The root database.
     * @return `true` if the statement was generated successfully, `false` otherwise.
     */
    bool GenDeleteSTMT(stmt& dbstate, sqlite3* db);

};


