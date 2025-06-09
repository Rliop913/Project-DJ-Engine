#include "jsonWrapper.hpp"



template<>
template<>
bool
PDJE_JSONHandler<KV_W>::add(const KEY_VALUE& args)
{
    ROOT[args.first] = args.second;
    return true;
}



template<>
template<>
int
PDJE_JSONHandler<KV_W>::deleteLine(const KEY& args)
{
    ROOT.erase(args);
    return 1;
}

template<>
template<>
void
PDJE_JSONHandler<KV_W>::getAll(
    std::function<void(const KEY_VALUE& args)> jsonCallback)
{
    for(auto& [key, value] : ROOT.items()){
        jsonCallback(
            KEY_VALUE(key, value.dump())
        );
    }
}


template<>
bool
PDJE_JSONHandler<KV_W>::load(const std::u8string& path)
{
    auto filepath = fs::path(path); 
    filepath /= "keyvaluemetadata.PDJE";
    if(fs::exists(filepath)){
        if(fs::is_regular_file(filepath)){
            std::ifstream jfile(filepath);
            
            if(!jfile.is_open()) return false;

            try{ jfile >> ROOT; }
            catch(...){ return false; }

            jfile.close();
        }
        else{
            return false;
        }
    }
    else{
        fs::create_directories(filepath.parent_path());
        std::ofstream jfile(filepath);
        if(!jfile.is_open()) return false;
        jfile << std::setw(4) << ROOT;
        jfile.close();
    }
    return true;

}

template<>
template<> 
int 
PDJE_JSONHandler<KV_W>::deleteLine(
    const std::string& args,
    bool skipType, 
    bool skipDetail) = delete;
