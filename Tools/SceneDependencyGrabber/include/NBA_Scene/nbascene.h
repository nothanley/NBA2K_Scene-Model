// Stores scene attributes and model children. Extrapolates CNBAModel objects from given JSON.

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
	int getNumModels();
	const char* getName();
	bool empty();
	void setName(const char* id);
	std::vector<std::shared_ptr<CNBAModel>> models();

public:
	void parse();

private:
	void readModels(JSON& json);

private:
	JSON m_json;
	std::string m_name;
	std::vector<std::shared_ptr<CNBAModel>> m_models;
};

