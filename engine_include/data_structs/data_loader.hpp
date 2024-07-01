#pragma once
#include <string>
#include <unordered_map>
#include "GlobalStructs.hpp"
#include <memory>
#include "PDJS.pb.h"
using KV = std::unordered_map<std::string, std::string>;

struct data_manager{
    function_pointers* DFP;
    void* pfile = nullptr;
    void operator[](const std::string& file_path);
    bool operator>>=(KV& output);
    std::unique_ptr<data_manager> clone_self();
    data_manager(function_pointers&fp);
    ~data_manager();
private:
    void line_decoder(const std::string& line, KV& output);
};

