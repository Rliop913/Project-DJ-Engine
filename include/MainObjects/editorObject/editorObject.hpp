/**
 * @file editorObject.hpp
 * @brief Defines the main editor object for managing project data.
 */
#pragma once

#include <optional>
#include <filesystem>
#include "editor.hpp"
#include "audioPlayer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "tempDB.hpp"

/**
 * @brief Represents the arguments for editing a music entry.
 */
struct PDJE_API EDIT_ARG_MUSIC{
    UNSANITIZED musicName; ///< The unsanitized name of the music.
    
    MusicArgs arg; ///< The music arguments.
};

/// @brief Type alias for note editing arguments.
using EDIT_ARG_NOTE = NoteArgs;
/// @brief Type alias for mix editing arguments.
using EDIT_ARG_MIX  = MixArgs;
/// @brief Type alias for key-value editing arguments.
using EDIT_ARG_KEY_VALUE = KEY_VALUE;
/// @brief Type alias for a map of sanitized titles to sanitized composers.
using TITLE_COMPOSER = std::unordered_map<SANITIZED, SANITIZED>;

/**
 * @brief The main editor object for managing project data.
 *
 * This class provides a high-level interface for interacting with the editor,
 * including file I/O, version control, and data rendering.
 */
class PDJE_API editorObject {
private:
    std::optional<tempDB> projectLocalDB;
    fs::path projectRoot;
    fs::path mixFilePath;
    fs::path noteFilePath;
    fs::path kvFilePath;
    fs::path musicFileRootPath;
    std::optional<PDJE_Editor> E_obj;

    template<typename EDIT_ARG_TYPE>
    bool DefaultSaveFunction();

    template<typename EDIT_ARG_TYPE>
    bool DefaultSaveFunction(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj);

    trackdata makeTrackData(
        const UNSANITIZED& trackTitle, 
        TITLE_COMPOSER& titles);

public:
    /**
     * @brief Gets the Git repository for the mix data.
     * @return A pointer to the Git repository, or `nullptr` if not available.
     */
    git_repository* getMixRepo(){
        if(E_obj.has_value()){
            return E_obj->mixHandle.first->gw.repo;
        }
        else return nullptr;
    }

    /**
     * @brief Gets the Git repository for a specific music entry.
     * @param Title The unsanitized title of the music.
     * @return A pointer to the Git repository, or `nullptr` if not found.
     */
    git_repository* getMusicRepo(const UNSANITIZED& Title){
        auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(Title);
        if(!safeTitle){
            return nullptr;
        }
        if(E_obj.has_value()){
            for(auto& music : E_obj->musicHandle){
                if(music.musicName == safeTitle){
                    return music.gith->gw.repo;
                }
            }
        }
        else return nullptr;
    }

    /**
     * @brief Gets the Git repository for the note data.
     * @return A pointer to the Git repository, or `nullptr` if not available.
     */
    git_repository* getNoteRepo(){
        if(E_obj.has_value()){
            return E_obj->noteHandle.first->gw.repo;
        }
        else return nullptr;
    }

    /**
     * @brief Gets the Git repository for the key-value data.
     * @return A pointer to the Git repository, or `nullptr` if not available.
     */
    git_repository* getKVRepo(){
        if(E_obj.has_value()){
            return E_obj->KVHandler.first->gw.repo;
        }
        else return nullptr;
    }

    /**
     * @brief Adds a new line of data to the editor.
     * @tparam EDIT_ARG_TYPE The type of data to add.
     * @param obj The data object to add.
     * @return `true` if the line was added successfully, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE>
    bool AddLine(const EDIT_ARG_TYPE& obj);

    /**
     * @brief Adds a new music entry with a specified first bar.
     * @param musicName The unsanitized name of the music.
     * @param firstBar The first bar of the music.
     * @return `true` if the line was added successfully, `false` otherwise.
     */
    bool AddLine(const UNSANITIZED& musicName, const DONT_SANITIZE& firstBar);
    
    /**
     * @brief Deletes a line of mix data.
     * @param obj The mix data to delete.
     * @param skipType Whether to skip matching the `type` field.
     * @param skipDetail Whether to skip matching the `details` field.
     * @return The number of lines deleted.
     */
    int deleteLine(
        const EDIT_ARG_MIX& obj,
        bool skipType, 
        bool skipDetail);

    /**
     * @brief Deletes a line of data from the editor.
     * @tparam EDIT_ARG_TYPE The type of data to delete.
     * @param obj The data object to delete.
     * @return The number of lines deleted.
     */
    template<typename EDIT_ARG_TYPE> 
    int deleteLine(const EDIT_ARG_TYPE& obj);

    /**
     * @brief Renders the project data to the root database.
     * @param trackTitle The unsanitized title of the track to render.
     * @param ROOTDB The root database to render to.
     * @return `true` if the rendering was successful, `false` otherwise.
     */
    bool render(const UNSANITIZED& trackTitle, litedb& ROOTDB);

    /**
     * @brief Initializes a demo player for a given track.
     * @param[out] player A shared pointer to the audio player to initialize.
     * @param frameBufferSize The frame buffer size for the player.
     * @param trackTitle The unsanitized title of the track to play.
     */
    void demoPlayInit(
        std::shared_ptr<audioPlayer>& player, 
        unsigned int frameBufferSize, 
        const UNSANITIZED& trackTitle);

    /**
     * @brief Pushes a track to the root database.
     * @param ROOTDB The root database to push to.
     * @param trackTitleToPush The unsanitized title of the track to push.
     * @return `true` if the push was successful, `false` otherwise.
     */
    bool pushToRootDB(litedb& ROOTDB, const UNSANITIZED& trackTitleToPush);
    
    /**
     * @brief Pushes a music entry to the root database.
     * @param ROOTDB The root database to push to.
     * @param musicTitle The unsanitized title of the music.
     * @param musicComposer The unsanitized composer of the music.
     * @return `true` if the push was successful, `false` otherwise.
     */
    bool pushToRootDB(
        litedb& ROOTDB, 
        const UNSANITIZED& musicTitle, 
        const UNSANITIZED& musicComposer);

    /**
     * @brief Gets all data of a specific type from the editor.
     * @tparam EDIT_ARG_TYPE The type of data to get.
     * @param jsonCallback A callback function to process each data object.
     */
    template<typename EDIT_ARG_TYPE> 
    void getAll(std::function<void(const EDIT_ARG_TYPE& obj)> jsonCallback);
    
    /**
     * @brief Undoes the last operation.
     * @tparam EDIT_ARG_TYPE The type of data to undo.
     * @return `true` if the undo was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool Undo();
    
    /**
     * @brief Undoes the last operation for a specific music entry.
     * @tparam EDIT_ARG_TYPE The type of data to undo.
     * @param musicName The unsanitized name of the music.
     * @return `true` if the undo was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool Undo(const UNSANITIZED& musicName);
    
    /**
     * @brief Redoes the last undone operation.
     * @tparam EDIT_ARG_TYPE The type of data to redo.
     * @return `true` if the redo was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool Redo();

    /**
     * @brief Redoes the last undone operation for a specific music entry.
     * @tparam EDIT_ARG_TYPE The type of data to redo.
     * @param musicName The unsanitized name of the music.
     * @return `true` if the redo was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool Redo(const UNSANITIZED& musicName);

    /**
     * @brief Switches to a specific branch or commit.
     * @tparam EDIT_ARG_TYPE The type of data to operate on.
     * @param branchName The name of the branch to switch to.
     * @param commitID A pointer to the commit ID to switch to (optional).
     * @return `true` if the operation was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool Go(const DONT_SANITIZE& branchName, git_oid* commitID);

    /**
     * @brief Gets the commit log as a JSON graph.
     * @tparam EDIT_ARG_TYPE The type of data to get the log for.
     * @return The commit log as a JSON string.
     */
    template<typename EDIT_ARG_TYPE> 
    DONT_SANITIZE GetLogWithJSONGraph();
    
    /**
     * @brief Gets the commit log for a specific music entry as a JSON graph.
     * @tparam EDIT_ARG_TYPE The type of data to get the log for.
     * @param musicName The unsanitized name of the music.
     * @return The commit log as a JSON string.
     */
    template<typename EDIT_ARG_TYPE> 
    DONT_SANITIZE GetLogWithJSONGraph(const UNSANITIZED& musicName);
    
    /**
     * @brief Updates the commit log.
     * @tparam EDIT_ARG_TYPE The type of data to update the log for.
     * @return `true` if the update was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog();

    /**
     * @brief Updates the commit log for a specific branch.
     * @tparam EDIT_ARG_TYPE The type of data to update the log for.
     * @param branchName The name of the branch to update.
     * @return `true` if the update was successful, `false` otherwise.
     */
    template<typename EDIT_ARG_TYPE> 
    bool UpdateLog(const DONT_SANITIZE& branchName);

    /**
     * @brief Gets the diff between two timestamps.
     * @tparam EDIT_ARG_TYPE The type of data to get the diff for.
     * @param oldTimeStamp The old timestamp.
     * @param newTimeStamp The new timestamp.
     * @return A `DiffResult` object containing the diff.
     */
    template<typename EDIT_ARG_TYPE> 
    DiffResult GetDiff(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);

    /**
     * @brief Provides access to the underlying JSON data for key-value pairs.
     * @param key The key to access.
     * @return A reference to the JSON value.
     */
    nj& operator[](const DONT_SANITIZE& key);
    
    /**
     * @brief Destroys the entire project.
     * @warning This operation is irreversible.
     * @return A string indicating the result of the operation.
     */
    DONT_SANITIZE DESTROY_PROJECT();

    /**
     * @brief Configures a new music entry.
     * @param NewMusicName The unsanitized name of the new music.
     * @param composer The unsanitized composer of the new music.
     * @param musicPath The path to the music file.
     * @param firstBar The first bar of the music (defaults to "0").
     * @return `true` if the configuration was successful, `false` otherwise.
     */
    bool ConfigNewMusic(const UNSANITIZED& NewMusicName, 
                        const UNSANITIZED& composer,
                        const fs::path& musicPath,
                        const DONT_SANITIZE& firstBar = "0");

    /**
     * @brief Opens an existing editor project. if PDJE called InitEditor, you don't need to call this again.
     * @param projectPath The path to the project.
     * @return `true` if the project was opened successfully, `false` otherwise.
     */
    bool Open(const fs::path& projectPath);

    editorObject() = delete;

    /**
     * @brief Constructs a new editor object with author information.
     * @param auth_name The name of the author.
     * @param auth_email The email of the author.
     */
    editorObject(const DONT_SANITIZE &auth_name, const DONT_SANITIZE &auth_email){
        E_obj.emplace(auth_name, auth_email);
    }

    ~editorObject() = default;
};

template<> PDJE_API bool editorObject::Undo<EDIT_ARG_NOTE>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MIX>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);

template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_NOTE>(const EDIT_ARG_NOTE& obj);
template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_MIX>(const EDIT_ARG_MIX& obj);
template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_KEY_VALUE>(const EDIT_ARG_KEY_VALUE& obj);
template<> PDJE_API bool editorObject::AddLine<EDIT_ARG_MUSIC>(const EDIT_ARG_MUSIC& obj);

template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_NOTE>();
template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_MIX>();
template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API bool editorObject::DefaultSaveFunction<EDIT_ARG_MUSIC>(PDJE_Editor::MusicHandleStruct& i, const EDIT_ARG_MUSIC& obj);

template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_NOTE>(const EDIT_ARG_NOTE& obj);
template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_KEY_VALUE>(const EDIT_ARG_KEY_VALUE& obj);
template<> PDJE_API int editorObject::deleteLine<EDIT_ARG_MUSIC>(const EDIT_ARG_MUSIC& obj);

template<> PDJE_API void editorObject::getAll<EDIT_ARG_NOTE>(std::function<void(const EDIT_ARG_NOTE& obj)> jsonCallback);
template<> PDJE_API void editorObject::getAll<EDIT_ARG_MIX>(std::function<void(const EDIT_ARG_MIX& obj)> jsonCallback);
template<> PDJE_API void editorObject::getAll<EDIT_ARG_KEY_VALUE>(std::function<void(const EDIT_ARG_KEY_VALUE& obj)> jsonCallback);
template<> PDJE_API void editorObject::getAll<EDIT_ARG_MUSIC>(std::function<void(const EDIT_ARG_MUSIC& obj)> jsonCallback);

template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_NOTE>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_MIX>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_KEY_VALUE>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);
template<> PDJE_API DiffResult editorObject::GetDiff<EDIT_ARG_MUSIC>(const gitwrap::commit& oldTimeStamp, const gitwrap::commit& newTimeStamp);

template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>();
template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>();
template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API DONT_SANITIZE editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>();

template<> PDJE_API bool editorObject::Go<EDIT_ARG_NOTE>(const DONT_SANITIZE& branchName, git_oid* commitID);
template<> PDJE_API bool editorObject::Go<EDIT_ARG_MIX>(const DONT_SANITIZE& branchName, git_oid* commitID);
template<> PDJE_API bool editorObject::Go<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE& branchName, git_oid* commitID);
template<> PDJE_API bool editorObject::Go<EDIT_ARG_MUSIC>(const DONT_SANITIZE& branchName, git_oid* commitID);

template<> PDJE_API bool editorObject::Redo<EDIT_ARG_NOTE>();
template<> PDJE_API bool editorObject::Redo<EDIT_ARG_MIX>();
template<> PDJE_API bool editorObject::Redo<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API bool editorObject::Redo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);

template<> PDJE_API bool editorObject::Undo<EDIT_ARG_NOTE>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MIX>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API bool editorObject::Undo<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);

template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_NOTE>();
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_NOTE>(const DONT_SANITIZE& branchName);
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MIX>();
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MIX>(const DONT_SANITIZE& branchName);
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>();
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>(const DONT_SANITIZE& branchName);
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MUSIC>();
template<> PDJE_API bool editorObject::UpdateLog<EDIT_ARG_MUSIC>(const UNSANITIZED& musicName);
