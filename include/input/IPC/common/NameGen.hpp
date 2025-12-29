#pragma once
#include "PDJE_Crypto.hpp"
#include <botan/hex.h>
namespace PDJE_CRYPTO {
class RANDOM_GEN {
  private:
    Botan::AutoSeeded_RNG rng;

  public:
    RANDOM_GEN() = default;
    std::string
    Gen(const std::string &frontName, size_t bytes = 32)
    {
        std::vector<uint8_t> buf(bytes);
        rng.randomize(buf.data(), buf.size());
        return frontName + Botan::hex_encode(buf);
    }
};
}; // namespace PDJE_CRYPTO