#pragma once
#include <string>
#include <fstream>
#include "MiniAudioWrapper.h"
void
default_decoder_init(const std::string& song_path, ma_decoder& dec, ma_decoder_config& conf) {
	ma_decoder_init_file(song_path.c_str(), &conf, &dec);
}

void
default_decoder_uninit(ma_decoder& dec)
{
	ma_decoder_uninit(&dec);
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

void
default_get_whole_file(char*& bin_whole, long long& bin_size, const std::string& file_path)
{
	std::ifstream def_stream = std::ifstream(file_path, std::ios::binary);
	if (def_stream.is_open()) {
		def_stream.seekg(std::ios::end);
		std::streamsize ssize = def_stream.tellg();
		def_stream.seekg(std::ios::beg);
		bin_whole = new char[ssize];
		def_stream.read(bin_whole, ssize);
		def_stream.close();
	}
}

void
default_delete_binary_buffer(char*& binptr) {
	delete[] binptr;
}

//
//JSON_OUT
//default_json_parser(const std::string& json_text) {
//	Json::Reader reader;
//	Json::Value root;
//	reader.parse(json_text,root);
//	Json::Value::Members mem = root.getMemberNames();
//	JSON_OUT out;
//	for (int i = 0; i < mem.size(); ++i) {
//		out[mem.back()] = (root[mem.back()].asString());
//	}
//	return out;
//}


