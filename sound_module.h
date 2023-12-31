#pragma once
#include "GlobalStructs.h"
#include "Processor.h"
#include "ALBUM.h"
#include "faust_v_3_2.h"
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
	void toggle(tagables tag);
	void interpolate(tagables tag, ma_uint64 start_frame_, ma_uint64 end_frame_);

private://----------------------------------------------toggle functions---------------------------------------------------------//
	void eq_tog(tagables tag);
	void distortion_tog(tagables tag);
	void filter_tog(tagables tag);
	void control_tog(tagables tag);
	void vol_tog(tagables tag);
	void load_tog(tagables tag);
	void unload_tog(tagables tag);
	void dj_comp_tog(tagables tag);
	void beat_match_tog(tagables tag);
	void sola_tog(tagables tag);
	void echo_tog(tagables tag);
	void LFS_tog(tagables tag);
	void flanger_tog(tagables tag);
	void phaser_tog(tagables tag);
	void trance_tog(tagables tag);
	void panner_tog(tagables tag);
	void battle_tog(tagables tag);
	void roll_tog(tagables tag);


private://---------------------------------------------interpolate functions----------------------------------------------------------//
	void eq_inter(tagables tag, inter_body body);
	void distortion_inter(tagables tag, inter_body body);
	void filter_inter(tagables tag, inter_body body);
	void vol_inter(tagables tag, inter_body body);
	void sola_inter(tagables tag, inter_body body);
	void battle_inter(tagables tag, inter_body body);
};
