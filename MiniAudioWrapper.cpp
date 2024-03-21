#include "MiniAudioWrapper.h"
#include "Processor.h"

#define MAW_SAMPLERATE 48000
extern function_pointers GFP;
void
MAW::init_device(ma_device& dev, const ma_device_type& device_mode, const int& audio_buffer_size, const ma_device_data_proc& callback_mode,void* me)
{
	ma_device_config deconf = ma_device_config_init(device_mode);
	deconf.playback.format = ma_format_f32;
	deconf.playback.channels = 2;
	deconf.sampleRate = MAW_SAMPLERATE;
	deconf.periodSizeInFrames = audio_buffer_size;
	deconf.dataCallback = callback_mode;
	deconf.performanceProfile = ma_performance_profile_low_latency;
	deconf.pUserData = me;
	ma_device_init(NULL, &deconf, &dev);
}
void
MAW::silence_memory(void* point, const ma_uint32& frameCount)
{
	ma_silence_pcm_frames(point, frameCount, ma_format_f32, 2);
}

void 
MAW::touch_sound(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec, float* sola_buf, double& ffixer)
{
	double frame = double(frameCount) / ST.getInputOutputSampleRatio();
	double frame_floor = floor(frame);
	ffixer += (frame - frame_floor);
	ma_uint32 final_frame = ma_uint32(frame_floor);
	if (ffixer >= 1.0) {
		final_frame += 1;
		ffixer -= 1.0;
	}
	silence_memory(sola_buf, final_frame);
	silence_memory(buffer, frameCount);
	ma_decoder_read_pcm_frames(&dec, sola_buf, final_frame, NULL);
	ST.putSamples(sola_buf, final_frame);
	ST.receiveSamples(buffer, frameCount);
}


void
MAW::init_decoder(const std::string& song_path,ma_decoder& dec, const Processor& pproc)
{
	ma_decoder_config decconf = ma_decoder_config_init(ma_format_f32, 2, 48000);
	GFP.loader_function(song_path, dec, decconf);
}


void
MAW::touch_sound_back(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec, float* sola_buf, double& ffixer)
{
	double frame = double(frameCount) / ST.getInputOutputSampleRatio();
	double frame_floor = floor(frame);
	ffixer += (frame - frame_floor);
	ma_uint32 final_frame = ma_uint32(frame_floor);
	if (ffixer >= 1.0) {
		final_frame += 1;
		ffixer -= 1.0;
	}


	//ma_uint32 final_frame = ma_uint32(ceil(double(frameCount) / ST.getInputOutputSampleRatio()));
	silence_memory(sola_buf, final_frame);
	silence_memory(buffer, frameCount);
	back_read(dec, sola_buf, final_frame);
	reverse_data(sola_buf, final_frame);
	ST.putSamples(sola_buf, final_frame);
	ST.receiveSamples(buffer, frameCount);
}

void
MAW::reverse_data(float* buffer, const ma_uint32& frames)
{
	std::reverse(buffer, buffer + (frames - 1));
}

void
MAW::back_read(ma_decoder& dec, float* wbuffer, const ma_uint32& frames)
{
	ma_uint64 position;
	ma_decoder_get_cursor_in_pcm_frames(&dec, &position);
	position -= (ma_uint64)frames;
	ma_decoder_seek_to_pcm_frame(&dec, position);
	ma_decoder_read_pcm_frames(&dec, wbuffer, frames, NULL);
	ma_decoder_seek_to_pcm_frame(&dec, position);
}

void
MAW::uninit_decoder(ma_decoder& dec)
{
	GFP.unloader_function(dec);
	
}


void
MAW::uninit_device(ma_device& dev)
{
	ma_device_uninit(&dev);
}


void
MAW::decoder_seek_frame(ma_decoder& dec, const ma_uint64& pos)
{
	ma_decoder_seek_to_pcm_frame(&dec, pos);
}

void
MAW::decoder_get_cursor(ma_decoder& dec, ma_uint64& now)
{
	ma_decoder_get_cursor_in_pcm_frames(&dec, &now);
}

void
MAW::stop_device(ma_device& dev)
{
	ma_device_stop(&dev);
}

void
MAW::start_device(ma_device& dev)
{
	ma_device_start(&dev);
}
