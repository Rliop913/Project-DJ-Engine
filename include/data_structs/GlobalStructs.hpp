#pragma once
#ifndef GlobalStructs
#define GlobalStructs
#include <unordered_map>
#include <string>
#include <functional>
#include "MiniAudioWrapper.hpp"
#include "dj_BSL.hpp"
#define Global_album_Sample_Rate 48000
#define VOID_BUFFER_SIZE(framecount, channel_size) framecount* 4 * channel_size
#define SF(N) std::stof(N)
#define SI(N) std::stoi(N)
#define SD(N) std::stod(N)
using K_V = std::unordered_map<std::string, std::string>;
class ALBUM;

struct engine_order {//data struct for beatcompiler
	double frame_in=0.0;
	double frame_out=0.0;
	K_V dj_tags;//tag stored
};//the reserved order block define

struct work {//data struct for worker threads
	bool is_null = true;
	int ID = 0;
	//ma_uint32 frameCount;
	void* buf;//main buffer pointer
};

struct mix_data_set {//dataset for update game GUI
	std::string path="";
	float now_bpm=0.0f;

	float trim=0.0f;
	float fader=0.0f;
	float eq_mid_v = 0.0f;
	float eq_low_v = 0.0f;
	float eq_hi_v = 0.0f;
	bool lpf_sw=false;
	bool hpf_sw = false;
	bool echo_sw_ = false;
	bool l_f_s_sw_ = false;
	bool phaser_sw_ = false;
	bool flanger_sw_ = false;
	bool trance_sw_ = false;
	bool panner_sw_ = false;
	bool distortion_sw_ = false;

	bool scratch_sw_ = false;
	bool rev_sw_ = false;

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
	E_distortion,
	E_roll
};

struct daw_inner_data {
	std::string editing_data_path;
	bool is_soloing = false;
	bool data_written = false;
	int target_ID;
	float master_sola = 1.0f;
	std::unordered_map<int, std::vector<engine_order>> reserve_buffer; //a storage for lesser compile
};
struct DDTG {
	ma_uint64 frame_in;
	ma_uint64 frame_out;
	K_V tags;
};
// struct DDTG :DTAG {
// 	DDTG(DTAG dtg) {
// 		is_interpolate = dtg.is_interpolate;
// 		type = dtg.type;
// 		from = dtg.from;
// 		what = dtg.what;
// 		first_value = dtg.first_value;
// 		second_value = dtg.second_value;
// 		third_value = dtg.third_value;
// 		target = dtg.target;
// 		first_str = dtg.first_str;
// 		second_str = dtg.second_str;
// 	}
// };
//
//struct tagables
//{
//	std::string type;
//	int where_;
//	std::string what_;
//	double first;
//	double second;
//	double third;
//	std::string str_first;
//	std::string str_second;
//	int for_who;
//	ma_uint64 frame_in;
//	ma_uint64 frame_out;
//};


struct song_data {//song_datas
	bool playback_ordered = false;
	double bpm = 0;
	double first_beat = 0;
	int ID;
	bool got_frames = false;
	std::string data_path;
};

struct inter_body {//data struct for interpolation
	ma_uint64 start_frame=0;
	ma_uint64 end_frame=0;
	double start_value=0.0;
	double end_value=0.0;
	ALBUM* album_pointer=nullptr;
	ALBUM* for_who_album_p=nullptr;
	std::function<void(double)> setter_method;
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


typedef std::unordered_map<std::string, std::string> JSON_OUT;

struct function_pointers {
	void* (*init_fileitr)(const std::string&) = nullptr;//------------------------file itr init
	bool (*line_getter)(void*, std::string&) = nullptr;//-------------------------file line getter
	void (*uninit_fileitr)(void*) = nullptr;//-----------------------------uninit file itr
	void (*loader_function)(const std::string&, ma_decoder&, ma_decoder_config&);
	void (*unloader_function)(ma_decoder&);
	void (*read_whole_file)(char*& bin_whole, long long& bin_size, const std::string& file_path);
	void (*delete_bin_buf)(char*& binptr);
};

struct standard_tag_table
	{
		int bar = 0;
		int separate = 0;
		int beat = 0;
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
		std::string dj_data;
		void* process_pointer;
	};
struct stored_data {
		double start_bpm=0.0;
		double first_beat=0.0;
		std::vector<ch_bpm_data_table> bpm_storage;
	};
//struct raw_data {//-------store raws----------------//
//		standard_tag_table loc_table;
//		standard_tag_table long_end_table;
//		DJBSL tag_origin;
//	};

struct note_data {
	double beatpos = 0;
	double hold_end = -1.0;
};
struct mouse_data {
	double beatpos = 0;
	double direction = 0;
	double power = 0;
};


//function_pointers GFP;


#endif