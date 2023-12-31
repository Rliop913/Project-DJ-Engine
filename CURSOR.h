#pragma once
#include "MiniAudioWrapper.h"
#include "GlobalStructs.h"
class CURSOR
{
public:
	CURSOR(const std::string& song_path, Processor& pproc);
	~CURSOR();
	void mvcur(float* wbuf, ma_uint32 frameCount);
	inline void set_dir(const bool& is_front);
	inline void set_speed(const double& pitch, const double& tempo);
	inline void set_point(const ma_uint64& position);
	inline void get_now_frame(ma_uint64& now);
	inline double get_IOSR();
	void temp_mv(const bool& direction, const double& pitch, const double& tempo, const ma_uint32& play_frame);
	
private:
	bool go_front = true;
	bool temp_mv_enabled = false;
	ma_uint32 remained_frames = 0;
	bool origin_mv_dir = true;
	double origin_pitch = 0.0;
	double origin_tempo = 0.0;
	ma_decoder dec;
	soundtouch::SoundTouch* ST;
	void back_to_origin();
};

