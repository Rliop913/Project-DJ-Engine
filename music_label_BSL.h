#pragma once
#include <string>
#include "Binary_Save_Load_Main.h"
struct MLBSL {
	std::string music_title;
	std::string path;
	std::string composer_name;
	double bpm;
	double first_beat_point;
	std::string sof_lan_path;
};