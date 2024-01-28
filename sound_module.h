#pragma once
#include "GlobalStructs.h"
#include "include/SoundTouch.h"
#include "faust_v42.hpp"


class Processor;
class ALBUM;
class CURSOR;
class sound_module
{
private://hashs
	std::unordered_map<std::string, int> tog_hash;
	std::unordered_map<std::string, int> inter_hash;
	void tog_hash_filler();
	void inter_hash_filler();
public:
	sound_module(Processor* first, ALBUM* second, Faust_engine* third);
	~sound_module() = default;

private://-----------------------------------------------pointers----------------------------------------------------------//
	//Engine_Main* pengine;
	Processor* processor;
	ALBUM* palbum;
	Faust_engine* pfaust;
public://----------------------------------------------tog/interpolate API--------------------------------------------------------------//
	void toggle(const tagables& tag);
	void interpolate(const tagables& tag, const ma_uint64& start_frame_, const ma_uint64& end_frame_);

private://----------------------------------------------toggle functions---------------------------------------------------------//
	void eq_tog(const tagables& tag);
	void distortion_tog(const tagables& tag);
	void filter_tog(const tagables& tag);
	void control_tog(const tagables& tag);
	void vol_tog(const tagables& tag);
	void load_tog(const tagables& tag);
	void unload_tog(const tagables& tag);
	void dj_comp_tog(const tagables& tag);
	void beat_match_tog(const tagables& tag);
	void sola_tog(const tagables& tag);
	void echo_tog(const tagables& tag);
	void LFS_tog(const tagables& tag);
	void flanger_tog(const tagables& tag);
	void phaser_tog(const tagables& tag);
	void trance_tog(const tagables& tag);
	void panner_tog(const tagables& tag);
	void battle_tog(const tagables& tag);
	void roll_tog(const tagables& tag);


private://---------------------------------------------interpolate functions----------------------------------------------------------//
	void eq_inter(const tagables& tag, inter_body body);
	void distortion_inter(const tagables& tag, inter_body body);
	void filter_inter(const tagables& tag, inter_body body);
	void vol_inter(const tagables& tag, inter_body body);
	void sola_inter(const tagables& tag, inter_body body);
	void battle_inter(const tagables& tag, inter_body body);
};
