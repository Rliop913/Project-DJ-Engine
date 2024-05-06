#pragma once
#include <algorithm>

#include "miniaudio.h"
#include "SoundTouch.h"
#include <immintrin.h>
//#include "Processor.h"




using namespace soundtouch;
class Processor;
namespace MAW {
	void init_device(ma_device& dev, const ma_device_type& device_mode, const int& audio_buffer_size, const ma_device_data_proc& callback_mode, void* me);
	void silence_memory(void* point,const ma_uint32& frameCount);
	void touch_sound(soundtouch::SoundTouch& ST,const ma_uint32& frameCount,float* buffer,ma_decoder& dec, float* sola_buf, double& ffixer);
	void touch_sound_back(soundtouch::SoundTouch& ST, const ma_uint32& frameCount, float* buffer, ma_decoder& dec, float* sola_buf, double& ffixer);
	void init_decoder(const std::string& song_path,ma_decoder& dec, const Processor& pproc);
	void uninit_decoder(ma_decoder& dec, Processor& pproc);
	void uninit_device(ma_device& dev);
	void decoder_seek_frame(ma_decoder& dec, const ma_uint64& pos);
	void decoder_get_cursor(ma_decoder& dec, ma_uint64& now);
	void stop_device(ma_device& dev);
	void start_device(ma_device& dev);
	inline void reverse_data(float* buffer, const ma_uint32& frames);
	void back_read(ma_decoder& dec,float* wbuffer, const ma_uint32& frames);

};


