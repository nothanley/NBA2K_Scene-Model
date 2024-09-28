#include <armature/armature.h>
#include <json.hpp>
#pragma once

using JSON = nlohmann::ordered_json;

enum enBoneTag {
	PARENT    = 3382132591,
	CHILD     = 217464393,
	SIBLING   = 4109984141,
	TRANSLATE = 2483604563
};

class CBoneReader
{
public:
	CBoneReader(JSON& data, NSSkeleton& skeleton);

public:
	void parse();
	static void fromJSON(JSON& data, NSSkeleton& skeleton);

private:
	void loadTransformDef(const int index, const std::string& key, JSON& obj);

private:
	JSON* m_json;
	NSSkeleton* m_skeleton;
};

