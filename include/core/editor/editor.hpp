/**
 * @file editor.hpp
 * @brief High level editor interface for PDJE projects.
 */
#pragma once

#include "EditorArgs.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "dbRoot.hpp"

#include "TimeLine.hpp"

#include <exception>
#include <filesystem>
#include <memory>
#include <system_error>

#include "PDJE_EXPORT_SETTER.hpp"
#include "fileNameSanitizer.hpp"

static bool
MKDirs(const fs::path &path)
{
    std::error_code ec;
    try {

        if (!fs::create_directories(path, ec)) {
            if (ec) {

                critlog("failed to create music directories. Path: ");
                critlog(path.generic_string());
                return false;
            } else {
                return true;
            }
        } else {
            return true;
        }
    } catch (const std::exception &e) {
        critlog("failed to create directories. What: ");
        critlog(e.what());
        return false;
    }
}

/**
 * @brief Central class coordinating Git and JSON handlers for editing data.
 */
class PDJE_API PDJE_Editor {
  private:
    fs::path    pt;
    fs::path    music_root;
    std::string name;
    std::string email;
    // fs::path      mixp;
    // fs::path      notep;
    // fs::path      musicp;
    // fs::path      kvp;

  public:
    /// Pair of Git handler and JSON handler for mix data
    std::unique_ptr<PDJE_TIMELINE::TimeLine<MIX_W>> mixHandle;
    /// Pair of Git handler and JSON handler for note data
    std::unique_ptr<PDJE_TIMELINE::TimeLine<NOTE_W>> noteHandle;
    /// Pair of Git handler and JSON handler for key/value data
    std::unique_ptr<PDJE_TIMELINE::TimeLine<KV_W>> KVHandle;
    /**
     * @brief Holds Git and JSON handlers for individual music data.
     */
    struct MusicHandleStruct {

        std::unique_ptr<PDJE_TIMELINE::TimeLine<MUSIC_W>> handle;
        MusicHandleStruct()                          = delete;
        MusicHandleStruct(const MusicHandleStruct &) = delete;

        MusicHandleStruct(MusicHandleStruct &&) noexcept = default;
        MusicHandleStruct &
        operator=(MusicHandleStruct &&) noexcept = default;

        MusicHandleStruct(const fs::path      &path,
                          SANITIZED            musicName,
                          const DONT_SANITIZE &auth_name,
                          const DONT_SANITIZE &auth_email)
        {
            if (MKDirs(path)) {
                return;
            }
            handle = std::make_unique<PDJE_TIMELINE::TimeLine<MUSIC_W>>(
                path, musicName, auth_name, auth_email);
        }
    };
    /// List of music handles owned by this editor
    std::vector<MusicHandleStruct> musicHandle;

    /**
     * @brief Adds a music configuration to the project.
     * @param NewMusicName Sanitized name for the music.
     * @param dir_name Directory containing the music data.
     * @return true on success.
     */
    bool
    AddMusicConfig(const SANITIZED &NewMusicName, const std::string &dir_name);

    /**
     * @brief Opens or creates an editor project at the given path.
     * @param projectPath Directory of the project.
     * @return true when initialization succeeds.
     */
    // void
    // openProject();

    /**
     * @brief Constructs an editor with author information for Git.
     */
    PDJE_Editor(const fs::path      &root,
                const DONT_SANITIZE &auth_name,
                const DONT_SANITIZE &auth_email)
    {
        pt         = root;
        auto mixr  = root / "Mixes";
        auto noter = root / "Notes";
        auto kvr   = root / "KeyValues";
        music_root = root / "Musics";
        if (MKDirs(mixr) && MKDirs(noter) && MKDirs(kvr) &&
            MKDirs(music_root)) {
            mixHandle = std::make_unique<PDJE_TIMELINE::TimeLine<MIX_W>>(
                mixr, "mixmetadata.PDJE", auth_name, auth_email);
            noteHandle = std::make_unique<PDJE_TIMELINE::TimeLine<NOTE_W>>(
                noter, "notemetadata.PDJE", auth_name, auth_email);
            KVHandle = std::make_unique<PDJE_TIMELINE::TimeLine<KV_W>>(
                kvr, "keyvaluemetadata.PDJE", auth_name, auth_email);
            for (const auto &musicSubpath :
                 fs::directory_iterator(music_root)) {
                musicHandle.emplace_back(musicSubpath.path(),
                                         "musicmetadata.PDJE",
                                         auth_name,
                                         auth_email);
            }
        }
        name  = auth_name;
        email = auth_email;
    }
    /// Default destructor
    ~PDJE_Editor() = default;
};
