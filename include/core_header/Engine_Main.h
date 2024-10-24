#pragma once
#ifndef engine_main
#define engine_main

#include "faust_v42_SIMD.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "beat_compiler_extension.hpp"
#include <chrono>
#include "Processor.h"
#include "GlobalStructs.hpp"
class Engine_Main
{
public:
	Engine_Main(const int& init_audio_buffer_size=480);
	~Engine_Main();
	Engine_Main(const function_pointers& ppp, const int& init_audio_buffer_size=480);
	int init_audio_buffer_size = 480;



	void sw_to_manual(const std::string& song_meta_path);//switch to manual mode
	void sw_to_dj(const std::string& dj_data_path);//switch to dj mode
	//void sw_to_daw(std::string planning_data_path);//switch to daw mode
	void manual_music_change(const std::string& song_meta_path);//load album or change album. 
	void manual_playback(const int& albumID=-1);//manually playback
	void manual_stop(const int& albumID);//manually stop
	void manual_effect(const int& effect_type,const float& first, const float& second, const float& third);//manual effect API
	void manual_control();
	void clear_deck();
	void dj_start();
	double processed_time_return_ms();
	//THREADS
	//ma_uint64 processed_time_getter_return();
	//std::unordered_map<int,mix_data_set> get_mixing_status_updater();//get mixing data inside deck
	//THREADS

private:
	function_pointers GFP;
	Processor* PDJE_processor;
};

#endif // engine_main
