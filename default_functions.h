#pragma once
#include <string>
#include <fstream>
#include "MiniAudioWrapper.h"
void
default_decoder_init(const std::string& song_path, ma_decoder& dec, ma_decoder_config& conf) {
	ma_decoder_init_file(song_path.c_str(), &conf, &dec);
}


void*
default_file_init(const std::string& path) {
	std::ifstream* def_stream = new std::ifstream(path);
	return def_stream;
}

bool
default_line_getter(void* itr, std::string& outstr) {
	std::ifstream* isp = (std::ifstream*)itr;
	if (std::getline(*isp, outstr)) {
		return true;
	}
	else {
		return false;
	}
}

void
default_file_uninit(void* p) {
	delete(p);
	return;
}