#pragma once
#include "MiniAudioWrapper.h"
#include "GlobalStructs.h"
class Processor;
class ALBUM;
using namespace soundtouch;
class CURSOR
{
public:
	CURSOR(const std::string& song_path, Processor& pproc, ALBUM* aalbum);
	~CURSOR();
	void mvcur(float* wbuf, const ma_uint32& frameCount);
	void set_dir(const bool& is_front);
	void set_speed(const double& pitch, const double& tempo);
	void set_point(const ma_uint64& position);
	void get_now_frame(ma_uint64& now);
	double get_IOSR();
	void temp_mv(const bool& direction, const double& pitch, const double& tempo, const ma_uint64& play_frame);
	
private:
	double ffixer=0.0;
	ALBUM* album;
	bool go_front = true;
	bool temp_mv_enabled = false;
	ma_uint64 remained_frames = 0;
	bool origin_mv_dir = true;
	double origin_pitch = 0.0;
	double origin_tempo = 0.0;
	ma_decoder dec;
	SoundTouch* ST;
	void back_to_origin();
};

