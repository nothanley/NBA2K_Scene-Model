// ...
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
	void load();
	void parseJson();
	bool validate();

private:
	static std::string formatInputJson(const std::string& path);

private:
	JSON m_json;
	std::string m_path;
	std::shared_ptr<CNBAScene> m_scene;
};

