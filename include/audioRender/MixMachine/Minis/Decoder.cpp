#include "Decoder.hpp"


Decoder::Decoder()
{
    ;
}

bool 
Decoder::init(const std::string& song_path)
{
    ma_decoder_config dconf = ma_decoder_config_init(ma_format_f32, CHANNEL, SAMPLERATE);
    return ma_decoder_init_file(song_path.c_str(), &dconf, &dec) == MA_SUCCESS;
}   

bool
Decoder::changePos(unsigned long Pos)
{
    return ma_decoder_seek_to_pcm_frame(&dec, Pos) == MA_SUCCESS;
}

bool
Decoder::getPos(unsigned long long& pos)
{
    return ma_decoder_get_cursor_in_pcm_frames(&dec, &pos) == MA_SUCCESS;
}

bool
Decoder::getRange(unsigned long numFrames, std::vector<float>& buffer)
{
    unsigned long BufferSize = numFrames * CHANNEL;
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
