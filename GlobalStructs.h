#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "MiniAudioWrapper.h"

#define VOID_BUFFER_SIZE(framecount, channel_size) framecount* 4 * channel_size


struct engine_order {//data struct for beatcompiler
	double frame_in;
	double frame_out;
	std::unordered_map<std::string, std::string> tag;//tag stored
};//the reserved order block define

struct work {//data struct for worker threads
	int ID = 0;
	ma_uint32 frameCount;
	void* buf;//main buffer pointer
};

struct mix_data_set {//dataset for update game GUI
	std::string path;
	float now_bpm;

	float trim;
	float fader;
	float eq_mid_v;
	float eq_low_v;
	float eq_hi_v;
	bool lpf_sw;
	bool hpf_sw;
	bool echo_sw_;
	bool l_f_s_sw_;
	bool phaser_sw_;
	bool flanger_sw_;
	bool trance_sw_;
	bool panner_sw_;
	bool distortion_sw_;

	bool scratch_sw_;
	bool rev_sw_;

};

enum manual_effect_control_set {
	E_EQ_hi,
	E_EQ_mid,
	E_EQ_low,
	E_lpf,
	E_hpf,
	E_echo,
	E_l_f_s,
	E_phaser,
	E_flanger,
	E_trance,
	E_panner,
	E_distortion
};

struct daw_inner_data {
	std::string editing_data_path;
	bool is_soloing = false;
	bool data_written = false;
	int target_ID;
	float master_sola = 1.0f;
	std::unordered_map<int, std::vector<engine_order>> reserve_buffer; //a storage for lesser compile
};

struct tagables
{
	std::string type;
	int where_;
	std::string what_;
	double first;
	double second;
	std::string str_first;
	std::string str_second;
	int for_who;
	ma_uint64 frame_in;
	ma_uint64 frame_out;
};


struct song_data {//song_datas
	bool playback_ordered = false;
	double bpm = 0;
	double start_time = 0;
	int ID;
	bool got_frames = false;
	std::string data_path;
};

struct inter_body {//data struct for interpolation
	ma_uint64 start_frame;
	ma_uint64 end_frame;
	double start_value;
	double end_value;
	ALBUM* album_pointer;
	ALBUM* for_who_album_p;
	std::function<void(float)> setter_method;
	std::function<void(bool)> sw_method;
	std::function<void()> death_method;
};



struct daw_status_frame {
	int ID;
	std::string song_path;

	unsigned long long cursor;
	bool playback_ordered = true;


	float trim = 1.0f;
	float fader = 1.0f;
	//----------switches----------//
	int lo_fi_sw = 1;
	int hi_fi_sw = 1;
	int eq_mid_sw = 1;
	int eq_low_sw = 1;
	int eq_high_sw = 1;
	int echo_sw = 1;
	int l_f_s_sw = 1;
	int phaser_sw = 1;
	int flanger_sw = 1;
	int trance_sw = 1;
	int panner_sw = 1;
	int distortion_sw = 1;

	//--------values------------//

	int low_freq_value = 500;
	int high_freq_value = 500;
	float eq_mid_v = 0;
	float eq_low_v = 0;
	float eq_hi_v = 0;
	float echo_dur = 0.5f;
	float echo_feedback_var = 0.1f;
	float l_f_s_bps = 1.0f;
	float flanger_bps = 1.0f;
	float flanger_gain = 1.0f;
	float phaser_bps = 1.0f;
	float phaser_gain = 1.0f;
	float trance_bps = 1.0f;
	float panner_bps = 1.0f;
	float distortion_gain = 1.0f;
	float trance_gain = 1.0f;
	float l_f_s_gain_min_freq = 1.0f;
	float panner_gain = 1.0f;
	std::vector<inter_body> interpolate_queue;
};
struct function_pointers {
	void* (*init_fileitr)(const std::string&) = nullptr;//------------------------file itr init
	bool (*line_getter)(void*, std::string&) = nullptr;//-------------------------file line getter
	void (*uninit_fileitr)(void*) = nullptr;//-----------------------------uninit file itr
	void (*loader_function)(const std::string&, ma_decoder&, ma_decoder_config&);
}GFP;
struct standard_tag_table
	{
		int line_number = 0;
		int separate = 0;
		int y = 0;
	};
struct ch_bpm_data_table
	{
		standard_tag_table std_table;
		double time_to_here = 0.0;
		double approx_loc = 0.0;
		double bpm = 0;
		int frame_to_here = 0;
	};
struct dj_init_group {
		//void* (*init_fileitr)(std::string) = nullptr;//------------------------file itr init
		//bool (*line_getter)(void*, std::string&) = nullptr;//-------------------------file line getter
		//void (*uninit_fileitr)(void*) = nullptr;//-----------------------------uninit file itr
		std::string dj_data_path;
		void* process_pointer;
	};
struct stored_data {
		double start_bpm;
		double start_time;
		std::vector<ch_bpm_data_table> bpm_storage;
	};
struct raw_data {//-------store raws----------------//
		standard_tag_table loc_table;
		standard_tag_table long_end_table;
		std::unordered_map<std::string, std::string> other_tags;
	};

enum tog_module {
	EQ,
	DISTORTION,
	FILTER,
	CONTROL,
	VOL,
	LOAD,
	UNLOAD,
	DJ_COMP,
	BEAT_MATCH,
	SOLA,
	ECHO,
	LFS,
	FLANGER,
	PHASER,
	TRANCE,
	PANNER,
	BATTLE_DJ,
	ROLL,
	ALIVE
};

enum inter_module {
	EQ,
	FILTER,
	DISTORTION,
	VOL,
	SOLA,
	BATTLE_DJ,
};

#define Global_album_Sample_Rate 48000