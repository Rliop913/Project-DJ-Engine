#include "PDJE_Crypto.hpp"
#include <botan/auto_rng.h>
#include <botan/cipher_mode.h>
#include <botan/hex.h>
namespace PDJE_CRYPTO {

PSK::PSK()
{
    startlog();
}

bool
PSK::Gen(const std::string &algo)
{
    try {

        Botan::AutoSeeded_RNG rng;
        auto                  enc = Botan::Cipher_Mode::create_or_throw(
            algo, Botan::Cipher_Dir::Encryption);
        psk.resize(enc->maximum_keylength());
        rng.randomize(psk.data(), psk.size());
        return true;
    } catch (const std::exception &e) {
        critlog("failed to generate psk. why: ");
        critlog(e.what());
        return false;
    }
}

std::string
PSK::Encode()
{
    try {

        return Botan::hex_encode(psk);
    } catch (const std::exception &e) {
        critlog("failed to encode psk. Why: ");
        critlog(e.what());
        return {};
    }
}

bool
PSK::Decode(const std::string &hex)
{
    try {
        psk = Botan::hex_decode(hex);
        return true;
    } catch (const std::exception &e) {
        critlog("failed to decode psk. Why: ");
        critlog(e.what());
        return false;
    }
}

}; // namespace PDJE_CRYPTO