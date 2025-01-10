#pragma once

#include <vector>
#include <optional>

#include <capnp/message.h>
#include <capnp/serialize.h>

#include "MixBinary.capnp.h"

enum READ_MODE{
    READ_ONLY,
    READ_WRITE,
    NOT_OPENED
};

template<READ_MODE>
class MixBinary{
private:
    std::vector<kj::byte> Origin;
    std::optional<capnp::FlatArrayMessageReader> capreader;
    std::optional<capnp::MallocMessageBuilder> capwriter;
public:
    MixBinary();
    ~MixBinary();
    void* D;
    bool open(const std::vector<kj::byte>& capnpBinary);
    bool open();

    std::vector<kj::byte> out();
};