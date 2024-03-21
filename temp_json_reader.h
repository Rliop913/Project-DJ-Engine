#pragma once
#include <unordered_map>
#include <string>
#include "GlobalStructs.h"
#include <vector>
extern function_pointers GFP;
typedef std::vector<std::unordered_map<std::string, std::string>> TJR_RETURN;



namespace TJR {
	TJR_RETURN temp_json_reader(const std::string& json_path);
}








TJR_RETURN
TJR::temp_json_reader(const std::string& json_path)
{
	std::vector<JSON_OUT> fin_val;
	std::string temp = "asdf";
	void* fp = GFP.init_fileitr(json_path);
	std::string one_line;
	while (GFP.line_getter(fp, one_line)) {
		JSON_OUT tags = GFP.JSON_parser(one_line);
		fin_val.push_back(tags);
	}
	return fin_val;
}