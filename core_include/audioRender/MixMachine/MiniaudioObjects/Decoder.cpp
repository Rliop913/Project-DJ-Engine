#include "Decoder.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <filesystem>
#include <fstream>

Decoder::Decoder()
{
    ;
}

bool
Decoder::init(litedb &db, const SANITIZED_ORNOT &KeyOrPath)
{
    ma_decoder_config dconf =
        ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);

    if (KeyOrPath.find(".") != std::string::npos or
        KeyOrPath.find("/") != std::string::npos) {
        fs::path songPath = fs::path(KeyOrPath).lexically_normal();
        if (!fs::exists(songPath)) {
            critlog("failed to find music in findpath mode. Err from Decoder "
                    "init. ErrPath: ");
            critlog(KeyOrPath);
            return false;
        }
        std::ifstream        musicFile(songPath, std::ios::binary);
        std::vector<uint8_t> fileData{ std::istreambuf_iterator<char>(
                                           musicFile),
                                       std::istreambuf_iterator<char>() };

        if (fileData.empty()) {
            critlog("failed to read music binary data in findpath mode. Err "
                    "from Decoder init. ErrPath: ");
            critlog(KeyOrPath);
            return false;
        }
        musicBinary = std::move(fileData);
    } else {
        std::string tempBinary;
        if (!db.KVGet(KeyOrPath, tempBinary)) {
            critlog("failed to get music from rocksdb. Err from Decoder init. "
                    "ErrKey: ");
            critlog(KeyOrPath);
            return false;
        }

        musicBinary =
            std::vector<uint8_t>(tempBinary.begin(), tempBinary.end());
    }
    return ma_decoder_init_memory(
               musicBinary.data(), musicBinary.size(), &dconf, &dec) ==
           MA_SUCCESS;
}

bool
Decoder::changePos(FRAME_POS Pos)
{
    bool chposRes = ma_decoder_seek_to_pcm_frame(&dec, Pos) == MA_SUCCESS;
    if (!chposRes) {
        critlog(
            "failed to change music play position. from Decoder changePos. ");
    }
    return chposRes;
}

bool
Decoder::getPos(FRAME_POS &pos)
{
    bool getPosRes =
        ma_decoder_get_cursor_in_pcm_frames(&dec, &pos) == MA_SUCCESS;
    if (!getPosRes) {
        critlog("failed to get music play position. from Decoder getPos");
    }
    return getPosRes;
}

bool
Decoder::getRange(FRAME_POS numFrames, std::vector<float> &buffer)
{
    FRAME_POS BufferSize = numFrames * CHANNEL;
    if (buffer.size() < BufferSize) {
        buffer.resize(BufferSize);
    }
    if (ma_decoder_read_pcm_frames(&dec, buffer.data(), numFrames, NULL) !=
        MA_SUCCESS) {
        critlog(
            "failed to read pcm frames from musicData. from Decoder getRange");
        return false;
    }
    return true;
}

Decoder::~Decoder()
{
    ma_decoder_uninit(&dec);
}
