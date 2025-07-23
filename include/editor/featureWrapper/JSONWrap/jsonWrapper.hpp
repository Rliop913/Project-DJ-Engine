/**
 * @file jsonWrapper.hpp
 * @brief JSON based storage helpers for editor data.
 */
#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <functional>
#include <ranges>
#include <memory>

#include "fileNameSanitizer.hpp"

#include <nlohmann/json.hpp>

#include "MixTranslator.hpp"
#include "PDJE_EXPORT_SETTER.hpp"

#include "PDJE_LOG_SETTER.hpp"

#define PDJEARR "PDJE_MIX"
#define PDJENOTE "PDJE_NOTE"
#define PDJEMUSICBPM "PDJE_MUSIC_BPM"
using nj = nlohmann::json;
namespace fs = std::filesystem;
namespace vs = std::views;

/// Arguments describing a mix entry
struct PDJE_API MixArgs{
    TypeEnum type           = TypeEnum::EQ      ;
    DetailEnum details      = DetailEnum::HIGH  ;
    int ID                  = -1                ;
    SANITIZED_ORNOT first   = ""                ;
    SANITIZED_ORNOT second  = ""                ;
    SANITIZED_ORNOT third   = ""                ;
    long long bar           = -1                ;
    long long beat          = -1                ;
    long long separate      = -1                ;
    long long Ebar          = -1                ;
    long long Ebeat         = -1                ;
    long long Eseparate     = -1                ;
};

/// Arguments describing a note entry
struct PDJE_API NoteArgs{
    SANITIZED_ORNOT Note_Type   = "";
    SANITIZED_ORNOT Note_Detail = "";
    SANITIZED_ORNOT first       = "";
    SANITIZED_ORNOT second      = "";
    SANITIZED_ORNOT third       = "";
    long long bar               = -1;
    long long beat              = -1;
    long long separate          = -1;
    long long Ebar              = -1;
    long long Ebeat             = -1;
    long long Eseparate         = -1;
};

/// Arguments describing a music entry
struct PDJE_API MusicArgs{
    DONT_SANITIZE bpm       = ""                ;
    long long bar           = -1                ;
    long long beat          = -1                ;
    long long separate      = -1                ;
};

using MIX_W = CapWriter<MixBinaryCapnpData>;
using NOTE_W = CapWriter<NoteBinaryCapnpData>;
using MUSIC_W = CapWriter<MusicBinaryCapnpData>;

using KEY = DONT_SANITIZE;
using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
using KV_W = std::vector<KEY_VALUE>;

/**
 * @brief Generic handler for reading/writing editor JSON files.
 */
template<typename CapnpWriterType>
class PDJE_API PDJE_JSONHandler{
private:
    nj ROOT;
public:

    /// Convert the current JSON data to a capnp writer object
    std::unique_ptr<CapnpWriterType> render();

    template<typename Target>
    int deleteLine(
        const Target& args,
        bool skipType,
        bool skipDetail);

    template<typename Target> 
    int deleteLine(const Target& args);

    template<typename Target>
    bool add(const Target& args);

    template<typename Target>
    void getAll(std::function<void(const Target& args)> jsonCallback);
        
    bool load(const fs::path& path);





    /// Access underlying JSON data by key
    inline nj& operator[](const DONT_SANITIZE& key){
        return ROOT[key];
    }

    /// Save the JSON data to disk
    bool save(const fs::path& path){
        std::ofstream jfile(path);
        if(jfile.is_open()){
            jfile << std::setw(4) << ROOT;
            return true;
        }
        else{
            return false;
        }
    }
    

    /// Delete a JSON file from disk
    bool deleteFile(const fs::path& path){
        try{ return fs::remove_all(path) > 0; }
        catch(...) { return false; }
    }


    PDJE_JSONHandler() = default;
    ~PDJE_JSONHandler() = default;
};

