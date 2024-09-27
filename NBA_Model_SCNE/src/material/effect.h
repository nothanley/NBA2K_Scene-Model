#include <string>
#include <vector>
#include <json.hpp>
#pragma once

using JSON = nlohmann::ordered_json;

struct NSShader
{
	std::string name;
	std::string path;
};

class CNSEffect
{

public:
	CNSEffect();

public:
	void setName(const char* name);
	void setPath(const char* path);
	void setData(std::shared_ptr<JSON> data);
	void updateShaderPaths(const char* base_path);

public:
	const std::vector<NSShader>& shaders() const;
	std::string id();
	std::string path();
	std::shared_ptr<JSON> data();

private:
	void loadShaderResources();

private:
	std::shared_ptr<JSON> m_data;
	std::string m_path;
	std::string m_name;
	std::vector<NSShader> m_shaders;
};

