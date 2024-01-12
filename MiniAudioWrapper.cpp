#include "MiniAudioWrapper.h"
#include "Processor.h"
#define MAW_SAMPLERATE 48000
ma_device
MAW::init_device(const ma_device_type& device_mode, const bool& enable_low_buffer_mode, const ma_device_data_proc& callback_mode,void* me)
{
	ma_device_config deconf = ma_device_config_init(device_mode);
	deconf.playback.format = ma_format_f32;
	deconf.playback.channels = 2;
	deconf.sampleRate = MAW_SAMPLERATE;
	deconf.dataCallback = callback_mode;
	{
		enable_low_buffer_mode ?
			deconf.performanceProfile = ma_performance_profile_low_latency
			:
			false;
	}
	deconf.pUserData = me;
	ma_device dev;
	ma_device_init(NULL, &deconf, &dev);
	return dev;
}
void
MAW::silence_memory(void* point, const ma_uint32& frameCount)
{
	ma_silence_pcm_frames(point, frameCount, ma_format_f32, 2);
}

void 
MAW::touch_sound(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec)
{
	ma_uint32 final_frame = ceil(double(frameCount) / ST.getInputOutputSampleRatio());
	float* sola_temp = new float[final_frame * 2];
	silence_memory(buffer, frameCount);
	ma_decoder_read_pcm_frames(&dec, sola_temp, final_frame, NULL);
	ST.putSamples(sola_temp, final_frame);
	ST.receiveSamples(buffer, frameCount);
	delete[] sola_temp;
}


ma_decoder 
MAW::init_decoder(const std::string& song_path,ma_decoder& dec, const Processor& pproc)
{
	ma_decoder_config decconf = ma_decoder_config_init(ma_format_f32, 2, 48000);
}

void 
MAW::init_soundtouch(soundtouch::SoundTouch* ST)
{
	ST = new soundtouch::SoundTouch();
	ST->setChannels(2);
	ST->setSampleRate(44100);
	ST->setSetting(0, 1);
	ST->setSetting(3, 1);
	ST->setSetting(2, 0);
	ST->setTempo(1.0);
}

void
MAW::touch_sound_back(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec)
{
	ma_uint32 final_frame = ceil(double(frameCount) / ST.getInputOutputSampleRatio());
	float* sola_temp = new float[final_frame * 2];
	silence_memory(buffer, frameCount);
	back_read(dec, sola_temp, final_frame);
	reverse_data(sola_temp, final_frame);
	ST.putSamples(sola_temp, final_frame);
	ST.receiveSamples(buffer, frameCount);
	delete[] sola_temp;
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