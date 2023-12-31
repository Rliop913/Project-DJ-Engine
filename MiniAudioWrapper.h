#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "Processor.h"
#include "miniaudio.h"
#include "include/SoundTouch.h"
#include <algorithm>
namespace MAW {
	ma_device init_device(const ma_device_type& device_mode, const bool& enable_low_buffer_mode, const ma_device_data_proc& callback_mode, void* me);
	inline void silence_memory(void* point,const ma_uint32& frameCount);
	void touch_sound(soundtouch::SoundTouch& ST,const ma_uint32& frameCount,float* buffer,ma_decoder& dec);
	void touch_sound_back(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec);
	inline ma_decoder init_decoder(const std::string& song_path,ma_decoder& dec, const Processor& pproc);
	void init_soundtouch(soundtouch::SoundTouch* ST);
	inline void reverse_data(float* buffer, const ma_uint32& frames);
	void back_read(ma_decoder& dec,float* wbuffer, const ma_uint32& frames);
};


