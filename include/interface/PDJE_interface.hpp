#pragma once

#include "audioPlayer.hpp"
#include "dbRoot.hpp"
#include "NoteTranslator.hpp"
#include "editorObject.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
/**
 * @brief the play mode
 * you can use this to initialize the player(music handler)
 * to use fx control in real time, do not use Full pre render.
 * 
 */
enum PLAY_MODE{
    FULL_PRE_RENDER,
    HYBRID_RENDER,
    FULL_MANUAL_RENDER
};
/**
 * @brief the main Interface of this Engine
 * PDJE gets music data and track data from database. 
 * from that datas, you can activate music player and you can get a music player handler.
 * with this player handler, you can control music's mixing in real time.
 * 
 * to-use
 * 1. make PDJE object
 * 2. call SearchTrack
 * 3. call InitPlayer
 * 4. use player. this is handler.
 * 
 * \dot
 * digraph PDJE_Interface_Tree{
 *      PDJE -> Search_Tools;
 *      PDJE -> Player;
 *      Player -> Player_ON_OFF;
 *      Player -> FXController;
 *      Player -> MusicController;
 *      FXController -> FX_ON_OFF;
 *      FXController -> FX_arg_setter;
 *      FX_arg_setter -> change_FX_value;
 *      MusicController -> Load_Music;
 *      MusicController -> Unload_Music;
 *      MusicController -> ON_OFF_Music;
 *      MusicController -> Cue_Music;
 *      MusicController -> FXController;
 * }
 * \enddot
 * 
 */
class PDJE_API PDJE{
private:
public:
    /// @brief this is the Root Database.
    /// check before use.
    /// it contains music metadatas and trackdatas.
    std::optional<litedb> DBROOT;
    /**
     * @brief Construct a new PDJE object
     * 
     * @param rootPath the path to the Root Database. 
     */
    PDJE(const fs::path& rootDir);
    ~PDJE() = default;
    
    /// this is the music handler. you can play music, stop music, fx control, play/stop music manually in realtime.
    std::optional<audioPlayer> player;
    std::optional<editorObject> editor;
    /**
     * @brief this inits the music handler. the music handler called a "player"
     * it initializes the player
     * @param mode the play modes. you can choose "FULL_PRE_RENDER", "HYBRID_RENDER", "FULL_MANUAL_RENDER"
     * @param td the track data. you can get this from SearchTrack() 
     * @param FrameBufferSize the buffersize. in this project, it uses 48000 samplerate. if you use 48 as a value, in theory, it calls mixing function 1000 times per second.
     * @return true  no error
     * @return false  error
     */
    bool
    InitPlayer(
        PLAY_MODE mode, 
        trackdata& td, 
        const unsigned int FrameBufferSize);


    bool
    InitEditor(
        const std::string &auth_name, 
        const std::string &auth_email,
        const fs::path& projectRoot
    );
    /**
     * @brief Parse Note data and calls received function.
     * this function parse the note datas in the database.
     * after parsing the note datas, this function calls received function.
     * you should make your custom function that can make your own note object
     * @param td the track data. you can get this from SearchTrack()
     * @param ObjectSetCallback the lambda callback. give a function that could make a new note objects. it's totally depends on you.
     * @return true 
     * @return false 
     */
    bool
    GetNoteObjects(
        trackdata& td,
        OBJ_SETTER_CALLBACK& ObjectSetCallback
    );
    /**
     * @brief searches musics and metadatas from database.
     * if you don't need to filter, send "" to the values
     * @param Title the title of the music
     * @param composer the composer of the music
     * @param bpm the bpm of the music. send under zero to skip filter
     * @return MUS_VEC 
     */
    MUS_VEC 
    SearchMusic(
        const std::string& Title, 
        const std::string& composer, 
        const double bpm = -1);
    /**
     * @brief searches track
     * the track contains the note data, mix data and included music lists.
     * @param Title the tile of the track. send "" to skip filter
     * @return TRACK_VEC the array of the track_data. find what you want
     */
    TRACK_VEC SearchTrack(const std::string& Title);
    /**
     * @brief music handler getter api for binded codes.
     * this function gives you a music handler.
     * you can access player directly in cpp, but not in binded languages.
     * so this function exists.
     * 
     * @return audioPlayer* the player object.check nullptr before use.
     */
    audioPlayer* GetPlayerObject();

    
};
/**
 * @brief the realtime fx controller wrapper class for binded languages.
 * the argsetter is basically unordered_map. but the binded languges can't use that, so this wrapper exists.
 * with this, you can control fx in realtime.
 */
class PDJE_API ARGSETTER_WRAPPER{
private:
    FXControlPannel* fxp;
public:
    /**
     * @brief Construct a new argsetter wrapper object
     * wraps the FXControlPannel object.
     * @param pointer the controlpannel object, you can get this from music handler.
     */
    ARGSETTER_WRAPPER(FXControlPannel* pointer): fxp(pointer){};
    ~ARGSETTER_WRAPPER() = default;
    /**
     * @brief gets the available fx list
     * returns available fx key list.
     * @param fx the type of fx. check the FXList.
     * @return std::vector<std::string> the key list.
     */
    std::vector<std::string> GetFXArgKeys(FXList fx);
    /**
     * @brief set fx in realtime.
     * sets the fx in realtime. better to check the fx key before use if you don't know.
     * @param fx the tyoe of fx.
     * @param key the key of the fx arg
     * @param arg value to change
     */
    void
    SetFXArg(
        FXList fx, 
        const std::string& key, 
        double arg);

};