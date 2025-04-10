#pragma once
#include <sqlite3.h>
#include <string>

#include "dbState.hpp"

/**
 * @brief the music meta data's struct
 * 
 */
struct trackdata{
public:
    std::string trackTitle;
    BIN mixBinary;
    BIN noteBinary;
    /// @brief Included music list
    std::string cachedMixList;

    /**
     * @brief Constructor. typically used for returning search results.
     * 
     * @param dbstate the sqlite statement
     */
    trackdata(stmt* dbstate);

    /**
     * @brief Construct used for filtering track entries.
     * 
     * Use this constructor to specify filtering conditions for a search query.
     * Leave parameters blank 
     * 
     * @param trackTitle__ the title of the track
     */
    trackdata(
        const std::string& trackTitle__ = ""
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
     * @brief generates the insert statements
     * 
     * @param dbstate statement pointer
     * @param db Root DB
     * @return true 
     * @return false 
     */
    bool GenInsertSTMT(stmt& dbstate, sqlite3* db);
};