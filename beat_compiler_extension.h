#pragma once
#ifndef BEATEX
#define BEATEX
#include <mutex>
#include <thread>
#include <fstream>
#include <unordered_map>
#include "GlobalStructs.h"
#include "dj_BSL.h"
#include "music_label_BSL.h"
#include "paper_BSL.h"
#define CONST_BPM 60
class beat_compiler_extension
{

public://structs & data arrays
	beat_compiler_extension(const dj_init_group& init);
	~beat_compiler_extension();
	std::unordered_map<int, std::vector<engine_order>> reservation_storage;
	std::unordered_map<int, std::string> album_specific_data;
	std::unordered_map<int, stored_data> album_data_for_real_time;
	std::unordered_map<int, std::vector<DJBSL>> raw_reserve;//temp data array for beatcompiler
	
	//API functions
	double calc_in_real_time(const double& approx_loc, const int& albumID);
	void calc_now_bpm(const unsigned long long& now_frame, const int& albumID, double& bpmP);
	inline void init_in_real_time(const int& albumID, double& bpmP, double& first_beat_P);
private://inner datas
	sfit<DJBSL>* bindj;
	DJBSL* origin_base;
	//std::string MAIN_DATA;
	void * voidp;
	std::mutex real_time_mutex;
	std::mutex data_locker;//about accessing reservation_storage

	//inner functions & thread safes
	void calculate_raw_data(const int& albumID);
	inline double to_appr(const standard_tag_table& table);
	void sort_storage(std::vector<ch_bpm_data_table>& inside);
	void calc_bpm_storage(std::vector<ch_bpm_data_table>& inside,const double& start_bpm,const double& first_beat);

	inline double calc_time_T_S(const standard_tag_table& std_dat,const double& bpm,const double& first_beat);
	inline double calc_time_T_S(const double& approx_loc,const double& bpm,const double& first_beat);
	inline double calc_time_between_T_S(const ch_bpm_data_table& front_dat,const standard_tag_table& back_dat);
	inline double calc_time_between_T_S(const ch_bpm_data_table& front_dat,const double& back_approx_loc);

	//inner functions
	void main_reader_start();
	void reservation(std::vector<ch_bpm_data_table>& inside,const int& albumID,const double& start_bpm,const double& first_beat);
	void sort_reservation();
	
	int check_bpms(const std::vector<ch_bpm_data_table>& bpmS, const double& aprx);
};

#endif // !BEATEX