#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include "dbRoot.hpp"

class tempDB{
private:
    std::optional<litedb> tempROOT;
public:
    bool Open(fs::path& projectRoot);
    bool BuildProject(trackdata& td, std::vector<musdata>& mds);
    litedb& GetBuildedProject(){
        return tempROOT.value();
    }
    tempDB() = default;
    ~tempDB() = default;
    
};