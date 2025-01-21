#include "data_loader.hpp"




data_manager::data_manager(function_pointers& fp):DFP(&fp) {}
data_manager::~data_manager()
{
    if (pfile != nullptr)
    {
        DFP->uninit_fileitr(pfile);
    }
}
std::unique_ptr<data_manager>
data_manager::clone_self()
{
    return std::make_unique<data_manager>(*DFP);
}

void
data_manager::operator[](const std::string& file_path)
{
    if (pfile != nullptr)
    {
        DFP->uninit_fileitr(pfile);
    }
    pfile = DFP->init_fileitr(file_path);
}

bool
data_manager::operator>>=(KV& output)
{
    std::string line;
    if (DFP->line_getter(pfile, line)){
        line_decoder(line, output);
        return true;
    }
    else{
        return false;
    }
}

void
data_manager::line_decoder(const std::string& line, KV& output)
{
    size_t pos_accum = 0;
    while (true){
        size_t read_s = line.find("_", pos_accum);
        size_t read_e = line.find("_", read_s+1);
        if(read_e == std::string::npos){
            break;
        }
        std::string kv_inside = line.substr(read_s+1, read_e-read_s-1);
        size_t kv_split = kv_inside.find("-");
        output[kv_inside.substr(0,kv_split)] = kv_inside.substr(kv_split+1);
        pos_accum = ++read_s;
    }
}