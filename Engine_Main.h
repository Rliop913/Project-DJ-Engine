#pragma once
#ifndef engine_main
#define engine_main

#include "faust_v_3_2.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "beat_compiler_extension.h"

#include "Processor.h"
#include "GlobalStructs.h"
class Engine_Main
{
public:
	Engine_Main(const int& init_audio_buffer_size=480);
	~Engine_Main();
	Engine_Main(const function_pointers& ppp, const int& init_audio_buffer_size=480);
	int init_audio_buffer_size = 480;



	void sw_to_manual(const std::string& song_path, const std::string& song_meta_path);//switch to manual mode
	void sw_to_dj(const std::string& dj_data_path);//switch to dj mode
	//void sw_to_daw(std::string planning_data_path);//switch to daw mode
	void manual_music_change(const std::string& song_path, const std::string& song_meta_path);//load album or change album. 
	void manual_playback(const int& albumID=-1);//manually playback
	void manual_stop(const int& albumID);//manually stop
	void manual_effect(const int& effect_type,const float& first, const float& second);//manual effect API
	void clear_deck();
	void dj_start();

	//void daw_test_play(std::vector<daw_status_frame> data_pointer);//daw test play
	//void daw_stop();
	//void daw_solo(int ID);//play only single album
	//void daw_unsolo();
	//void daw_set_master_sola(float value);
	//void daw_set_max_deck_use(int value);
	//void daw_update_reserve();//fills reserve data
	//void metronome_activate(std::string sound_path,float sound_volume=1.0f);//activates metronome
	//
	
	//void view_album(ma_uint8*& buffer, ma_uint64* len, int rev_scale, std::string song_path);
	//THREADS
	//ma_uint64 processed_time_getter_return();
	//std::unordered_map<int,mix_data_set> get_mixing_status_updater();//get mixing data inside deck
	//THREADS

private:

	Processor* PDJE_processor;
};

#endif // engine_main
