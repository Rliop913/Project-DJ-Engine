#pragma once
#ifndef engine_main
#define engine_main

#include "miniaudio.h"
#include "faust_v_3_2.h"
#include <functional>
#include <string>
//#include "include/SoundTouch.h"
#include <unordered_map>
#include <vector>
#include "beat_compiler_extension.h"

#include "Processor.h"
//#define SOUNDTOUCH_FLOAT_SAMPLES

#include "GlobalStructs.h"
class Engine_Main
{
public:
	Engine_Main();
	~Engine_Main();
	Engine_Main(const function_pointers& ppp);
public://----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------data structs, data arrays
	
	/*struct custom_data {
		int channel=2;
	}custom;
	*/
public://----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Engine APIs
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
	ma_uint64 processed_time_getter_return();
	std::unordered_map<int,mix_data_set> get_mixing_status_updater();//get mixing data inside deck
	//THREADS

private:
	Processor* PDJE_processor;
};

#endif // engine_main

//
//public://----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------pointers
//	//beat_compiler_extension* pcompiler; bool comp_alive = false;//for real time calculation, should be deleted after use
//	ma_device_info* pPlayback_info;
//	//--------------------------------------------//
//	
//protected://----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------inside use datas
//	ma_device_config idle_conf;//idle configure
//	ma_device_config dj_conf;//dj configure
//	ma_device_config daw_conf;//daw configure
//
//	ma_context_config con_conf;//context conf, dummy
	//access with album ID. 
	//The ID 0 means start album's reservation	
	//The reservation will be check every Engine_Callback