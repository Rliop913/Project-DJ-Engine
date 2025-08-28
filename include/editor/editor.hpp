/**
 * @file editor.hpp
 * @brief High level editor interface for PDJE projects.
 */
#pragma once

#include "dbRoot.hpp"
#include "gitWrapper.hpp"
#include "jsonWrapper.hpp"
#include <filesystem>

#include "PDJE_EXPORT_SETTER.hpp"
#include "fileNameSanitizer.hpp"

/**
 * @brief Central class coordinating Git and JSON handlers for editing data.
 */
class PDJE_API PDJE_Editor {
  private:
    fs::path      pt;
    fs::path      mixp;
    fs::path      notep;
    fs::path      musicp;
    fs::path      kvp;
    DONT_SANITIZE name;
    DONT_SANITIZE email;

  public:
    /// Pair of Git handler and JSON handler for mix data
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<MIX_W>>
        mixHandle;
    /// Pair of Git handler and JSON handler for note data
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<NOTE_W>>
        noteHandle;
    /// Pair of Git handler and JSON handler for key/value data
    std::pair<std::unique_ptr<PDJE_GitHandler>, PDJE_JSONHandler<KV_W>>
        KVHandler;
    /**
     * @brief Holds Git and JSON handlers for individual music data.
     */
    struct MusicHandleStruct {
        std::unique_ptr<PDJE_GitHandler> gith;
        PDJE_JSONHandler<MUSIC_W>        jsonh;
        SANITIZED                        musicName;
        fs::path                         dataPath;
        MusicHandleStruct()                          = delete;
        MusicHandleStruct(const MusicHandleStruct &) = delete;
        // MusicHandleStruct& operator=(const MusicHandleStruct&) = delete;
        MusicHandleStruct(MusicHandleStruct &&) noexcept = default;
        MusicHandleStruct &
        operator=(MusicHandleStruct &&) noexcept = default;

        MusicHandleStruct(const DONT_SANITIZE &auth_name,
                          const DONT_SANITIZE &auth_email)
            : jsonh()
        {
            gith = std::make_unique<PDJE_GitHandler>(auth_name, auth_email);
        }
    };
    /// List of music handles owned by this editor
    std::vector<MusicHandleStruct> musicHandle;

    /**
     * @brief Adds a music configuration to the project.
     * @param NewMusicName Sanitized name for the music.
     * @param DataPath Directory containing the music data.
     * @return true on success.
     */
    bool
    AddMusicConfig(const SANITIZED &NewMusicName, fs::path &DataPath);

    /**
     * @brief Opens or creates an editor project at the given path.
     * @param projectPath Directory of the project.
     * @return true when initialization succeeds.
     */
    bool
    openProject(const fs::path &projectPath);

    /**
     * @brief Constructs an editor with author information for Git.
     */
    PDJE_Editor(const DONT_SANITIZE &auth_name, const DONT_SANITIZE &auth_email)
        : mixHandle{ std::make_unique<PDJE_GitHandler>(auth_name, auth_email),
                     PDJE_JSONHandler<MIX_W>{} },
          noteHandle{ std::make_unique<PDJE_GitHandler>(auth_name, auth_email),
                      PDJE_JSONHandler<NOTE_W>{} },
          KVHandler{ std::make_unique<PDJE_GitHandler>(auth_name, auth_email),
                     PDJE_JSONHandler<KV_W>{} },
          name(auth_name), email(auth_email)
    {
    }
    /// Default destructor
    ~PDJE_Editor() = default;
};
