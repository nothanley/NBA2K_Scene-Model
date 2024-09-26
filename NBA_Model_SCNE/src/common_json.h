#include <json.hpp>
#include <string>
#include <vector>
#pragma once


using JSON = nlohmann::ordered_json;

namespace commonjson
{
	std::shared_ptr<JSON> loadFileJson(const char* path);
	void findAllMatchingJsonValues(JSON& obj, const char* target_key, std::vector<std::string>& list);
}