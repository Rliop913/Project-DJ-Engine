#include "MixBinary.hpp"
#include "errorTable.hpp"
template<>
MixBinary<READ_MODE::READ_ONLY>::MixBinary()
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
        D = reinterpret_cast<void*>(&(capreader.value().getRoot<MixBinaryCapnpData>()));
    }
    catch(const std::exception& e)
    {
        errpdje::ereport(
            "ERR on MixBinary Read ERRMSG: " + std::string(e.what()),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read open()"
        );
        return false;
    } catch(...)
    {
        errpdje::ereport(
            "ERR on MixBinary Read ERRMSG: " + std::string("UNKNOWN ERR"),
            errpdje::ERR_TYPE::MIX_BIN_ERR,
            "MixBinary_Read open()"
        );
        return false;
    }
    return true;
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