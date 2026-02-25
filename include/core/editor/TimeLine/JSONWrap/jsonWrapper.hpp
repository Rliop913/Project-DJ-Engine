/**
 * @file jsonWrapper.hpp
 * @brief JSON based storage helpers for editor data.
 */
#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>

#include "fileNameSanitizer.hpp"

#include <nlohmann/json.hpp>

#include "MixTranslator.hpp"
#include "PDJE_EXPORT_SETTER.hpp"

#include "PDJE_LOG_SETTER.hpp"

#define PDJEARR "PDJE_MIX"
#define PDJENOTE "PDJE_NOTE"
#define PDJEMUSICBPM "PDJE_MUSIC_BPM"
using nj     = nlohmann::json;
namespace fs = std::filesystem;
namespace vs = std::views;
#include "EditorArgs.hpp"

using MIX_W   = CapWriter<MixBinaryCapnpData>;
using NOTE_W  = CapWriter<NoteBinaryCapnpData>;
using MUSIC_W = CapWriter<MusicBinaryCapnpData>;

using KEY       = DONT_SANITIZE;
using KEY_VALUE = std::pair<DONT_SANITIZE, DONT_SANITIZE>;
using KV_W      = std::vector<KEY_VALUE>;

namespace PDJE_JSON_IO_DETAIL {

inline void
DumpDiffFriendlyArray(std::ostream &os, const nj &arr, int indent);

inline void
DumpDiffFriendlyObject(std::ostream &os, const nj &obj, int indent)
{
    os << "{";
    if (!obj.is_object() || obj.empty()) {
        os << "}";
        return;
    }

    os << "\n";
    bool first = true;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        os << std::string(static_cast<std::size_t>(indent + 2), ' ');
        if (!first) {
            os << ", ";
        }
        os << nj(it.key()).dump() << ": ";
        if (it.value().is_array()) {
            DumpDiffFriendlyArray(os, it.value(), indent + 2);
        } else {
            os << it.value().dump();
        }
        os << "\n";
        first = false;
    }
    os << std::string(static_cast<std::size_t>(indent), ' ') << "}";
}

inline void
DumpDiffFriendlyArray(std::ostream &os, const nj &arr, int indent)
{
    os << "[";
    if (!arr.is_array() || arr.empty()) {
        os << "]";
        return;
    }

    os << "\n";
    bool first = true;
    for (const auto &elem : arr) {
        os << std::string(static_cast<std::size_t>(indent + 2), ' ');
        if (!first) {
            os << ", ";
        }
        os << elem.dump();
        os << "\n";
        first = false;
    }
    os << std::string(static_cast<std::size_t>(indent), ' ') << "]";
}

inline std::string
DumpDiffFriendlyJson(const nj &root)
{
    std::ostringstream oss;
    if (root.is_object()) {
        DumpDiffFriendlyObject(oss, root, 0);
    } else if (root.is_array()) {
        DumpDiffFriendlyArray(oss, root, 0);
    } else {
        oss << root.dump();
    }
    oss << "\n";
    return oss.str();
}

inline bool
WriteDiffFriendlyJsonToFile(const fs::path &path, const nj &root)
{
    try {
        std::ofstream jfile(path);
        if (!jfile.is_open()) {
            return false;
        }
        jfile << DumpDiffFriendlyJson(root);
        return jfile.good();
    } catch (...) {
        return false;
    }
}

} // namespace PDJE_JSON_IO_DETAIL

/**
 * @brief Generic handler for reading/writing editor JSON files.
 */
template <typename CapnpWriterType> class PDJE_JSONHandler {
  private:
    nj ROOT;

  public:
    /// Convert the current JSON data to a capnp writer object
    std::unique_ptr<CapnpWriterType>
    render();

    template <typename Target>
    int
    deleteLine(const Target &args, bool skipType, bool skipDetail);

    template <typename Target>
    int
    deleteLine(const Target &args);

    template <typename Target>
    bool
    add(const Target &args);

    template <typename Target>
    void
    getAll(std::function<void(const Target &args)> jsonCallback);
    // add multi-threaded faster getter later

    bool
    load(const fs::path &filepath);

    /// Access underlying JSON data by key
    inline nj &
    operator[](const DONT_SANITIZE &key)
    {
        return ROOT[key];
    }

    /// Save the JSON data to disk
    bool
    save(const fs::path &path)
    {
        if (PDJE_JSON_IO_DETAIL::WriteDiffFriendlyJsonToFile(path, ROOT)) {
            return true;
        } else {
            critlog("failed to save json file. json file is not opened. "
                    "FilePath: ");
            critlog(path.generic_string());
            return false;
        }
    }

    /// Delete a JSON file from disk
    bool
    deleteFile(const fs::path &path)
    {
        try {
            return fs::remove_all(path) > 0;
        } catch (...) {
            return false;
        }
    }

    PDJE_JSONHandler()  = default;
    ~PDJE_JSONHandler() = default;
};

#define PDJE_JSON_TYPE "type"
#define PDJE_JSON_DETAILS "details"
#define PDJE_JSON_ID "id"
#define PDJE_JSON_FIRST "first"
#define PDJE_JSON_SECOND "second"
#define PDJE_JSON_THIRD "third"
#define PDJE_JSON_BEAT "beat"
#define PDJE_JSON_SUBBEAT "sub_beat"
#define PDJE_JSON_SEPARATE "separate"
#define PDJE_JSON_EBEAT "e_beat"
#define PDJE_JSON_ESUBBEAT "e_subBeat"
#define PDJE_JSON_ESEPARATE "e_separate"
#define PDJE_JSON_RAILID "rail_id"

#define PDJE_JSON_BPM "bpm"

#define PDJE_JSON_TITLE "title"
#define PDJE_JSON_COMPOSER "composer"
#define PDJE_JSON_PATH "path"

#define PDJE_JSON_NOTE_TYPE "note_type"
#define PDJE_JSON_NOTE_DETAIL "note_detail"

#define PDJE_JSON_FIRST_BEAT "first_beat"
