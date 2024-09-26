#include <string>
#include <vector>
#include <json.hpp>
#pragma once

using JSON = nlohmann::ordered_json;

class CNSEffect
{

public:
	CNSEffect();

public:
	void loadShaderResources();

public:
	std::string                 path;
	std::shared_ptr<JSON>       data;
	std::vector<std::string> shaders;
};

