// File object and container class for all .scene data. Parses and extrapolates CNBAScene from given JSON file path.

#include <fstream>
#include <json.hpp>
#pragma once

using JSON = nlohmann::json;
class CNBAScene;

class CSceneFile
{
public:
	CSceneFile(const char* path);
	~CSceneFile();

public:
	virtual void load();
	std::shared_ptr<CNBAScene>& scene();

protected:
	void parse();
	bool validate();
	static std::string formatInputJson(const std::string& path);

protected:
	JSON m_json;
	std::string m_path;
	std::shared_ptr<CNBAScene> m_scene;
};

