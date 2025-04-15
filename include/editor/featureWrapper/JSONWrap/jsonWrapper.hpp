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

class PDJE_JSONHandler{
private:
    nj ROOT;
public:

    std::unique_ptr<CapWriter<MixBinaryCapnpData>> render();

    int deleteLine(
        TypeEnum type, 
        DetailEnum details,
        int ID = -1,
        const std::string& first = "",
        const std::string& second = "",
        const std::string& third = "",
        long long bar = -1,
        long long beat = -1,
        long long separate = -1,
        long long Ebar = -1,
        long long Ebeat = -1,
        long long Eseparate = -1
    );

    bool add(
        TypeEnum type, 
        DetailEnum details,
        int ID,
        const std::string& first,
        const std::string& second,
        const std::string& third,
        long long bar,
        long long beat,
        long long separate,
        long long Ebar,
        long long Ebeat,
        long long Eseparate
    );

    void getAll(
        std::function<void(
            TypeEnum type, 
            DetailEnum details,
            int ID,
            const std::string& first,
            const std::string& second,
            const std::string& third,
            long long bar,
            long long beat,
            long long separate,
            long long Ebar,
            long long Ebeat,
            long long Eseparate
        )> jsonCallback
    );

    bool save(const std::string& path);
    bool load(const std::string& path);
    bool deleteFile(const std::string& path);


    PDJE_JSONHandler() = default;
    ~PDJE_JSONHandler() = default;
    


};