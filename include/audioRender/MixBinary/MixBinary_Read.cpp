#include "MixBinary.hpp"

template<>
MixBinary<READ_MODE::READ_ONLY>::MixBinary()
{

}

template<>
MixBinary<READ_MODE::READ_ONLY>::~MixBinary()
{
    
}


template<>
bool
MixBinary<READ_MODE::READ_ONLY>::open(const std::vector<kj::byte>& capnpBinary)
{
    try
    {
        Origin.resize(capnpBinary.size());
        memcpy(Origin.data(), capnpBinary.data(), capnpBinary.size());
        kj::ArrayPtr<const kj::byte> arr(Origin.data(), Origin.size());
        capreader.emplace(
            ::kj::arrayPtr(
                reinterpret_cast<const capnp::word*>(arr.begin()), 
                arr.size() / sizeof(capnp::word)
            )
        );
        readerOBJ = capreader->getRoot<MixBinaryCapnpData>();
        D = reinterpret_cast<void*>(&(readerOBJ.value()));
        return true;
    }
    catch(...)
    {
        return false;
    }
}

template<>
std::vector<kj::byte> 
MixBinary<READ_MODE::READ_ONLY>::out()
{
    return Origin;
}

template<>
bool
MixBinary<READ_MODE::READ_ONLY>::open()
{
    return false;
}