// Stores scene attributes and model children. Extrapolates CNBAModel objects from given JSON.

#include <vector>
#include <memory>
#include <json.hpp>
#pragma once

struct Material;
class CNBAModel;
using JSON = nlohmann::json;

enum enSceneData {
	EFFECT = 2957141266,
	TEXTURE = 976334198,
	MATERIAL = 3427514740,
	MODEL = 229569494,
	OBJECT = 3343895260
};

enum enMaterialTag {
	COLOR_MAP  = 0,
	NORMAL_MAP = 1,
	ROUGH_MAP  = 2
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
	std::shared_ptr<CNBAModel>& model(const int index);
	std::vector<std::shared_ptr<CNBAModel>>& models();

public:
	void parse();

private:
	void readModels(JSON& json);
	void readMaterials(JSON& json);

private:
	JSON m_json;
	std::string m_name;
	std::vector<std::shared_ptr<CNBAModel>> m_models;
	std::vector<Material> m_materials;
};

