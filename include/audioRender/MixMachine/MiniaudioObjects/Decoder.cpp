#include "Decoder.hpp"
#include <filesystem>
#include <fstream>

Decoder::Decoder()
{
    ;
}

bool 
Decoder::init(const fs::path& song_path, const fs::path& root_path)
{
    ma_decoder_config dconf = ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
    
    fs::path fullpath = root_path.parent_path() / song_path;
    fullpath = fullpath.lexically_normal();
    
    std::ifstream musicFile(fullpath, std::ios::binary);
    std::vector<uint8_t> fileData {
        std::istreambuf_iterator<char>(musicFile),
        std::istreambuf_iterator<char>()
    };

    if(fileData.empty()) return false;

    musicBinary = std::move(fileData);
    return ma_decoder_init_memory(musicBinary.data(), musicBinary.size(), &dconf, &dec) == MA_SUCCESS;

    // return ma_decoder_init_file(reinterpret_cast<const char*>(fullpath.u8string().c_str()), &dconf, &dec) == MA_SUCCESS;
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
