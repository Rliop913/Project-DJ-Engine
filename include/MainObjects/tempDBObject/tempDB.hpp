/**
 * @file tempDB.hpp
 * @brief Defines a temporary database for managing project data.
 */
#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include "PDJE_EXPORT_SETTER.hpp"
#include "dbRoot.hpp"

/**
 * @brief A temporary database for managing project data.
 *
 * This class provides a temporary database to store and manage project data
 * during the editing and rendering process.
 */
class PDJE_API tempDB{
private:
    std::optional<litedb> tempROOT;
public:
    /// @brief deprecated Error container.
    std::string ERR = "";

    /**
     * @brief Opens the temporary database at the specified project root.
     *
     * @param projectRoot The root directory of the project.
     * @return `true` if the database was opened successfully, `false` otherwise.
     */
    bool Open(const fs::path& projectRoot);

    /**
     * @brief Builds a project in the temporary database.
     *
     * @param td The track data for the project.
     * @param mds A vector of music data for the project.
     * @return `true` if the project was built successfully, `false` otherwise.
     */
    bool BuildProject(trackdata& td, std::vector<musdata>& mds);

    /**
     * @brief Gets the underlying `litedb` object for the built project.
     *
     * @return A reference to the `litedb` object.
     */
    litedb& GetBuildedProject(){
        return tempROOT.value();
    }

    tempDB() = default;
    ~tempDB() = default;
    
};