#include "MixBinary.hpp"



template<>
MixBinary<READ_MODE::READ_WRITE>::MixBinary()
{

}

template<>
MixBinary<READ_MODE::READ_WRITE>::~MixBinary()
{
    
}

template<>
bool
MixBinary<READ_MODE::READ_WRITE>::open(const std::vector<kj::byte>& capnpBinary)
{
    try
    {
        kj::ArrayPtr<const kj::byte> arr(capnpBinary.data(), capnpBinary.size());
        capwriter.emplace();
        readerOBJ = capwriter->initRoot<MixBinaryCapnpData>();
        D = reinterpret_cast<void*>(&(readerOBJ.value()));
        capnp::FlatArrayMessageReader readed(
            ::kj::arrayPtr(
                reinterpret_cast<const capnp::word*>(arr.begin()), 
                arr.size() / sizeof(capnp::word)
            )
        );
        auto readroot = readed.getRoot<MixBinaryCapnpData>();
        reinterpret_cast<MixBinaryCapnpData::Builder*>(D)->initDatas(readroot.getDatas().size());
        reinterpret_cast<MixBinaryCapnpData::Builder*>(D)->setDatas(readroot.getDatas());
        return true;
    }
    catch(...)
    {
        return false;
    }
}

template<>
bool
MixBinary<READ_MODE::READ_WRITE>::open()
{
    try
    {
        capnp::MallocMessageBuilder build;
        capwriter.emplace();
        writerOBJ = capwriter->initRoot<MixBinaryCapnpData>();
        D = reinterpret_cast<void*>(&(writerOBJ.value()));
        return true;
    }
    catch(...)
    {
        return false;
    }
}

template<>
std::vector<kj::byte> 
MixBinary<READ_MODE::READ_WRITE>::out()
{
    try
    {   
        auto farr = capnp::messageToFlatArray(capwriter.value());
        auto fbyte = farr.asBytes();
        std::vector<kj::byte> buffer(fbyte.begin(), fbyte.end());
        return buffer;
    } 
    catch(...)
    {
        return std::vector<kj::byte>();
    }
}