#include <nbascene.h>
#include <json.hpp>
#pragma once

using JSON = nlohmann::json;

enum enSceneData {
	EFFECT = 2957141266,
	TEXTURE = 976334198,
	MATERIAL = 3427514740,
	MODEL = 229569494,
	OBJECT = 3343895260
};

enum enMaterialTag {
	COLOR_MAP = 0,
	NORMAL_MAP = 1,
	ROUGH_MAP = 2
};

class CSceneReader : public CNBAScene
{
public:
	CSceneReader(const char* name, JSON& json);

public:
	void parse();

private:
	void readModels(JSON& json);
	void readMaterials(JSON& json);
	void loadMaterial(JSON& obj);

private:
	JSON m_json;
};

