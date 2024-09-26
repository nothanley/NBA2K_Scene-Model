#include <string>
#include <memory>
#include <vector>
#include <json.hpp>
#pragma once

// namespace types
using JSON = nlohmann::ordered_json;

// forward declare
class CAssetTex;
class CNSEffect;

// game material class
class CNSMaterial
{
public:
	CNSMaterial();

public:
	bool setPath(const char* path);
	void loadFromDisk(const char* material_directory);

private:
	void loadFX();

private:
	std::string m_path;
	std::string m_name;
	std::vector<std::shared_ptr<CAssetTex>> m_textures;
	std::shared_ptr<CNSEffect> m_effect;
};


