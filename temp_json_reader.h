#pragma once
#include "json.h"
#include <unordered_map>
#include <string>
#include "GlobalStructs.h"
#include <vector>

typedef std::vector<std::unordered_map<std::string, std::string>> TJR_RETURN;



namespace TJR {
	TJR_RETURN temp_json_reader(const std::string& json_path);
}








TJR_RETURN
TJR::temp_json_reader(const std::string& json_path)
{
	std::vector<std::unordered_map<std::string, std::string>> fin_val;
	Json::Value jroot;
	Json::Reader parser;

	void* fp = GFP.init_fileitr(json_path);
	std::string one_line;
	while (GFP.line_getter(fp, one_line)) {
		std::unordered_map<std::string, std::string> tags;
		parser.parse(one_line, jroot);
		for (auto it = jroot.begin(); it != jroot.end(); ++it) {
			tags[it.key().asString()] = (*it).asString();
		}
		fin_val.push_back(tags);
	}
	return fin_val;
}