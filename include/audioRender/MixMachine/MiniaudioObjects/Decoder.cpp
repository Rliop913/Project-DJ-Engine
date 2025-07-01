#include "Decoder.hpp"
#include <filesystem>
#include <fstream>

Decoder::Decoder()
{
    ;
}

bool 
Decoder::init(litedb& db, const SANITIZED_ORNOT& KeyOrPath)
{
    ma_decoder_config dconf = ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
    
    if(KeyOrPath.find(".") != std::string::npos or KeyOrPath.find("/") != std::string::npos){
        fs::path songPath = fs::path(KeyOrPath).lexically_normal();
        if(!fs::exists(songPath)){
            db.DB_ERROR += "\ndecoder init Error: songpath doesn't exists";
            return false;
        }
        std::ifstream musicFile(songPath, std::ios::binary);
        std::vector<uint8_t> fileData {
            std::istreambuf_iterator<char>(musicFile),
            std::istreambuf_iterator<char>()
        };

        if(fileData.empty()) return false;
        musicBinary = std::move(fileData);
    }
    else{
        std::string tempBinary;
        if(!db.KVGet(KeyOrPath, tempBinary)) return false;

        musicBinary = std::vector<uint8_t>(tempBinary.begin(), tempBinary.end());    
    }
    return ma_decoder_init_memory(musicBinary.data(), musicBinary.size(), &dconf, &dec) == MA_SUCCESS;
}

bool
Decoder::changePos(FRAME_POS Pos)
{
    return ma_decoder_seek_to_pcm_frame(&dec, Pos) == MA_SUCCESS;
}

bool
Decoder::getPos(FRAME_POS& pos)
{
    return ma_decoder_get_cursor_in_pcm_frames(&dec, &pos) == MA_SUCCESS;
}

bool
Decoder::getRange(FRAME_POS numFrames, std::vector<float>& buffer)
{
    FRAME_POS BufferSize = numFrames * CHANNEL;
    if(buffer.size() < BufferSize){
        buffer.resize(BufferSize);
    }
    if(ma_decoder_read_pcm_frames(&dec, buffer.data(), numFrames, NULL) != MA_SUCCESS){
        return false;
    }
    return true;
}

Decoder::~Decoder()
{
    ma_decoder_uninit(&dec);
}
