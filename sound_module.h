#pragma once
#include "GlobalStructs.h"
#include "include/SoundTouch.h"
#include "faust_v42_SIMD.hpp"


class Processor;
class ALBUM;
class CURSOR;
class sound_module
{
public:
	sound_module(Processor* first, ALBUM* second, Faust_engine* third);
	~sound_module() = default;

private://-----------------------------------------------pointers----------------------------------------------------------//
	//Engine_Main* pengine;
	Processor* processor;
	ALBUM* palbum;
	Faust_engine* pfaust;
public://----------------------------------------------tog/interpolate API--------------------------------------------------------------//
	void toggle(const DDTG& tag);
	void interpolate(const DDTG& tag, const ma_uint64& start_frame_, const ma_uint64& end_frame_);

private://----------------------------------------------toggle functions---------------------------------------------------------//
	void eq_tog(const DDTG& tag);
	void distortion_tog(const DDTG& tag);
	void filter_tog(const DDTG& tag);
	void control_tog(const DDTG& tag);
	void vol_tog(const DDTG& tag);
	void load_tog(const DDTG& tag);
	void unload_tog(const DDTG& tag);
	void dj_comp_tog(const DDTG& tag);
	void beat_match_tog(const DDTG& tag);
	void sola_tog(const DDTG& tag);
	void echo_tog(const DDTG& tag);
	void LFS_tog(const DDTG& tag);
	void flanger_tog(const DDTG& tag);
	void phaser_tog(const DDTG& tag);
	void trance_tog(const DDTG& tag);
	void panner_tog(const DDTG& tag);
	void battle_tog(const DDTG& tag);
	void roll_tog(const DDTG& tag);


private://---------------------------------------------interpolate functions----------------------------------------------------------//
	void eq_inter(const DDTG& tag, inter_body body);
	void distortion_inter(const DDTG& tag, inter_body body);
	void filter_inter(const DDTG& tag, inter_body body);
	void vol_inter(const DDTG& tag, inter_body body);
	void sola_inter(const DDTG& tag, inter_body body);
	void battle_inter(const DDTG& tag, inter_body body);
};
