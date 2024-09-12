// ...
#include <vector>
#include <memory>
#include <json.hpp>
#pragma once

class CNBAModel;
using JSON = nlohmann::json;

enum enSceneData {
	EFFECT = 2957141266,
	TEXTURE = 976334198,
	MATERIAL = 3427514740,
	MODEL = 229569494,
	OBJECT = 3343895260
};

class CNBAScene
{
public:
	CNBAScene(const char* name);
	CNBAScene(const char* name, JSON& json);

public:
	void parse();
	const char* getName();
	void setName(const char* id);

private:
	void readModels(JSON& json);
	// void readMaterial();
	// void readEffect();

private:
	JSON m_json;
	std::string m_name;
	std::vector<std::shared_ptr<CNBAModel>> m_models;
};

