#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include "PDJE_EXPORT_SETTER.hpp"
#include "dbRoot.hpp"

class PDJE_API tempDB{
private:
    std::optional<litedb> tempROOT;
public:
    bool Open(const fs::path& projectRoot);
    bool BuildProject(trackdata& td, std::vector<musdata>& mds);
    litedb& GetBuildedProject(){
        return tempROOT.value();
    }
    tempDB() = default;
    ~tempDB() = default;
    
};