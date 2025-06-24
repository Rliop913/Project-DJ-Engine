#include "tempDB.hpp"

bool
tempDB::Open(const fs::path& projectRoot)
{
    if(tempROOT.has_value()) tempROOT.reset();
    
    tempROOT.emplace();
    return 
    tempROOT->openDB(
        (projectRoot / fs::path("LOCALDB.pdjedb")).u8string()
    );
}

bool
tempDB::BuildProject(trackdata& td, std::vector<musdata>& mds)
{
    auto dbposition = tempROOT->getRoot();
    tempROOT.reset();
    if(!fs::remove(dbposition)){
        return false;
    }
    tempROOT.emplace();

    if(!tempROOT->openDB(dbposition))   return false;
    if(!(tempROOT.value() <= td))       return false;
    for(auto& i : mds){
        if(!(tempROOT.value() <= i))      return false;
    }
    return true;
}