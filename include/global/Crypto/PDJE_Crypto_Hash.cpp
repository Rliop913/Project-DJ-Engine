#include "PDJE_Crypto.hpp"
#include <botan/hex.h>
#include <fstream>
namespace PDJE_CRYPTO{
Hash::Hash()
{
    startlog();
    try
    {
        hashEngine = Botan::HashFunction::create_or_throw("SHA-256");
    }
    catch(const std::exception& e)
    {
        critlog("failed to init hash engine. Why: ");
        critlog(e.what());
    }
}

std::string
Hash::TextHash(const std::string& txt)
{
    try{

        hashEngine->update(reinterpret_cast<const uint8_t*>(txt.data()), txt.size());
        return Botan::hex_encode(hashEngine->final());
    }catch(const std::exception& e){
        critlog("failed to hash text. Why: ");
        critlog(e.what());
        return {};
    }
}

constexpr size_t HashChunkSZ = 64 * 1024;

std::string
Hash::FileHash(const fs::path& fp)
{
    try{

        std::vector<uint8_t> buf(HashChunkSZ);
        
        std::ifstream file(fp, std::ios::binary);
        if(!file){
            return {};
        }
        
        while(file){
            file.read(reinterpret_cast<char*>(buf.data()), buf.size());
            std::streamsize got = file.gcount();
            if(got > 0){
                hashEngine->update(buf.data(), static_cast<size_t>(got));
            }
        }
        return Botan::hex_encode(hashEngine->final());
    }catch(const std::exception& e){
        critlog("failed to hash file. Why: ");
        critlog(e.what());
        return {};
    }
}
};