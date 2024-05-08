#pragma once
#include "GlobalStructs.hpp"
#include "SoundTouch.h"
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
	void toggle(DDTG& data);
	void interpolate(DDTG& data);

private://----------------------------------------------toggle functions---------------------------------------------------------//
	void eq_tog(DDTG& data);
	void distortion_tog(DDTG& data);
	void filter_tog(DDTG& data);
	void control_tog(DDTG& data);
	void vol_tog(DDTG& data);
	void load_tog(DDTG& data);
	void unload_tog(DDTG& data);
	void dj_comp_tog(DDTG& data);
	void beat_match_tog(DDTG& data);
	void sola_tog(DDTG& data);
	void echo_tog(DDTG& data);
	void LFS_tog(DDTG& data);
	void flanger_tog(DDTG& data);
	void phaser_tog(DDTG& data);
	void trance_tog(DDTG& data);
	void panner_tog(DDTG& data);
	void battle_tog(DDTG& data);
	void roll_tog(DDTG& data);


private://---------------------------------------------interpolate functions----------------------------------------------------------//
	void eq_inter(DDTG& data, inter_body body);
	void distortion_inter(DDTG& data, inter_body body);
	void filter_inter(DDTG& data, inter_body body);
	void vol_inter(DDTG& data, inter_body body);
	void sola_inter(DDTG& data, inter_body body);
	void battle_inter(DDTG& data, inter_body body);
};
