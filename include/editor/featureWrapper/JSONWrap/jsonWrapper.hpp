#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <functional>
#include <ranges>
#include <memory>

#include <nlohmann/json.hpp>

#include "MixTranslator.hpp"

#define PDJEARR "PDJE_MIX"

using nj = nlohmann::json;
namespace fs = std::filesystem;
namespace vs = std::views;

struct MixArgs{
    TypeEnum type;
    DetailEnum details;
    int ID = -1;
    const std::string& first = "";
    const std::string& second = "";
    const std::string& third = "";
    long long bar = -1;
    long long beat = -1;
    long long separate = -1;
    long long Ebar = -1;
    long long Ebeat = -1;
    long long Eseparate = -1;
};

class PDJE_JSONHandler{
private:
    nj ROOT;
public:

    std::unique_ptr<CapWriter<MixBinaryCapnpData>> render();

    int deleteLine(
        const MixArgs& args, 
        bool skipType = false, 
        bool skipDetail = false);

    bool add(const MixArgs& args);

    void getAll(
        std::function<void(const MixArgs& args)> jsonCallback);

    bool save(const std::string& path);
    bool load(const std::string& path);
    bool deleteFile(const std::string& path);


    PDJE_JSONHandler() = default;
    ~PDJE_JSONHandler() = default;
    


};