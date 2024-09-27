#include <texture/texture.h>
#pragma once

// namespace types
using JSON = nlohmann::ordered_json;

// forward declarations
class CNSEffect;

// game material class
class CNSMaterial
{
public:
	CNSMaterial();

public:
	bool empty();
	JSON data() const;
	std::string name();
	const std::shared_ptr<CNSEffect> effect() const;
	bool setPath(const char* path);
	void setName(const char* name);

public:
	void updateTexLinks(const std::string base_id);
	void setTextures(const std::vector<std::shared_ptr<CNSTexture>>& textures);
	void addTexture(const std::shared_ptr<CNSTexture> texture);
	const std::vector<std::shared_ptr<CNSTexture>>& textures() const;
	std::shared_ptr<CNSTexture> colorMap();
	std::shared_ptr<CNSTexture> normalMap();
	std::shared_ptr<CNSTexture> rdmoMap();

public:
	static std::vector<std::shared_ptr<CNSMaterial>> getMaterialLib(const char* directory);
	static std::shared_ptr<CNSMaterial>              fromDisk(const char* material_directory);

private:
	void loadMtl();
	void loadFX();

private:
	std::string m_path;
	std::string m_name;
	std::vector<std::shared_ptr<CNSTexture>> m_textures;
	std::shared_ptr<JSON> m_data;
	std::shared_ptr<CNSEffect> m_effect;
};

