#include "MixBinary.hpp"
#include "errorTable.hpp"

template<>
MixBinary<READ_MODE::READ_WRITE>::MixBinary()
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
        D = reinterpret_cast<void*>(&(capwriter->initRoot<MixBinaryCapnpData>()));
        capnp::FlatArrayMessageReader readed(
            ::kj::arrayPtr(
                reinterpret_cast<const capnp::word*>(arr.begin()), 
                arr.size() / sizeof(capnp::word)
            )
        );
        auto readroot = readed.getRoot<MixBinaryCapnpData>();
        reinterpret_cast<MixBinaryCapnpData::Builder*>(D)->initDatas(readroot.getDatas().size());
        reinterpret_cast<MixBinaryCapnpData::Builder*>(D)->setDatas(readroot.getDatas());
    }
    catch(const std::exception& e)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string(e.what()),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read_Write open()"
        );
        return false;
    } catch(...)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string("UNKNOWN ERR"),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read_Write open()"
        );
        return false;
    }
    return true;
}

template<>
bool
MixBinary<READ_MODE::READ_WRITE>::open()
{
    try
    {
        capnp::MallocMessageBuilder build;
        capwriter.emplace();
        D = reinterpret_cast<void*>(&(capwriter->initRoot<MixBinaryCapnpData>()));
    }
    catch(const std::exception& e)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string(e.what()),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read_Write open(void)"
        );
        return false;
    } catch(...)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string("UNKNOWN ERR"),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read_Write open(void)"
        );
        return false;
    }
    return true;
    
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
    catch(const std::exception& e)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string(e.what()),
            errpdje::ERR_TYPE::MIX_BIN_ARR_OUT_ERR,
            "MixBinary_Read_Write out()"
        );
    } catch(...)
    {
        errpdje::ereport(
            "ERR on MixBinary Read_Write ERRMSG: " + std::string("UNKNOWN"),
            errpdje::ERR_TYPE::MIX_BIN_ARR_OUT_ERR,
            "MixBinary_Read_Write out()"
        );
    }
    return std::vector<kj::byte>();
}