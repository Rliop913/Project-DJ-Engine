#include "tempDB.hpp"

bool
tempDB::Open(const fs::path& projectRoot)
{
    if(tempROOT.has_value()) tempROOT.reset();
    
    tempROOT.emplace();
    return 
    tempROOT->openDB(
        (projectRoot / fs::path("LOCALDB"))
    );
}

bool
tempDB::BuildProject(trackdata& td, std::vector<musdata>& mds)
{
    auto dbposition = tempROOT->getRoot();
    tempROOT.reset();
    try{

        if(!fs::remove_all(dbposition)){
            return false;
        }
    }
    catch(std::exception e){
        ERR = e.what();
    }
    tempROOT.emplace();

    if(!tempROOT->openDB(dbposition))   return false;
    if(!(tempROOT.value() <= td))       return false;
    for(auto& i : mds){
        if(!(tempROOT.value() <= i))      return false;
    }
    return true;
}