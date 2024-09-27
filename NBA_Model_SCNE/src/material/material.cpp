#include <material/material.h>
#include <material/effect.h>
#include <common.h>
#include <common_json.h>

CNSMaterial::CNSMaterial()
{
}

bool
CNSMaterial::setPath(const char* path)
{
	this->m_path = path;
	return !m_path.empty();
}

bool
CNSMaterial::empty()
{
	return !m_effect || m_path.empty();
}

std::string
CNSMaterial::name()
{
	return m_name;
}

void
CNSMaterial::setName(const char* name)
{
	m_name = name;

	if (m_effect)
		m_effect->setName(name);
}

std::shared_ptr<CNSMaterial>
CNSMaterial::fromDisk(const char* path)
{
	auto material = std::make_shared<CNSMaterial>();
	if (!material->setPath(path))
		return material;

	// load material data ...
	material->loadMtl();
	material->loadFX();

	// load id
	auto name = common::get_abs_dir_name(path);
	material->setName(name.c_str());
	return material;
}

void
CNSMaterial::loadMtl()
{
	// find .material file path
	auto mtlPath = common::findTargetFileExt(m_path.c_str(), ".material");
	if (mtlPath.empty())
		return;

	// load mtl json
	auto mtlJson = commonjson::loadFileJson(mtlPath.c_str());
	if (!mtlJson)
		return;

	// update mtl data
	this->m_data = mtlJson;
}

void
CNSMaterial::loadFX()
{
	// find .fx file path
	auto fxPath = common::findTargetFileExt(m_path.c_str(), ".fx");
	if (fxPath.empty())
		return;

	// load effect json
	auto fxJson = commonjson::loadFileJson(fxPath.c_str());
	if (!fxJson)
		return;

	// push effect to mtl
	m_effect = std::make_shared<CNSEffect>();
	m_effect->setPath(fxPath.c_str());
	m_effect->setData(fxJson);
}

JSON CNSMaterial::data() const
{
	return *m_data;
}

const std::shared_ptr<CNSEffect> CNSMaterial::effect() const
{
	return m_effect;
}

std::vector< std::shared_ptr< CNSMaterial > >
CNSMaterial::getMaterialLib(const char* template_dir)
{
	std::vector< std::shared_ptr< CNSMaterial > >  mats;
	std::vector< std::string > subdirs;
	common::get_all_subdirectories(template_dir, subdirs);

	for (auto& dir : subdirs)
	{
		auto mat = CNSMaterial::fromDisk(dir.c_str());
		if (!mat->empty())
			mats.push_back(mat);
	}

	return mats;
}


void
CNSMaterial::updateTexLinks(const std::string base_id)
{
	if (!m_data) return;

	try 
	{
		auto& json = *m_data.get();
		json["Resource"]["ColorTexture"]["Pixelmap"]                 = colorMap()->name();
		json["Resource"]["NormalTexture"]["Pixelmap"]                = normalMap()->name();
		json["Resource"]["RoughnessDensityMetalnessMap"]["Pixelmap"] = rdmoMap()->name();
	}
	catch (...)
	{
		printf("Error: CNSMaterial::updateTexLinks\n");
	}

}

void
CNSMaterial::addTexture(const std::shared_ptr<CNSTexture> texture)
{
	printf("\nLinking texture: %s - INFO: Type: %s Height: %d Width: %d\n", texture->name().c_str(), texture->type().c_str(), texture->height(), texture->width());
	m_textures.push_back(texture);
}

void
CNSMaterial::setTextures(const std::vector<std::shared_ptr<CNSTexture>>& textures)
{
	m_textures = textures;
}

std::shared_ptr<CNSTexture>
CNSMaterial::colorMap()
{
	for (auto& tex : m_textures)
		if (tex->type() == "ColorTexture")
			return tex;

	auto newTex = std::make_shared<CNSTexture>();
	m_textures.push_back(newTex);
	return newTex;
}

std::shared_ptr<CNSTexture>
CNSMaterial::normalMap()
{
	for (auto& tex : m_textures)
		if (tex->type() == "NormalTexture")
			return tex;

	auto newTex = std::make_shared<CNSTexture>();
	m_textures.push_back(newTex);
	return newTex;
}

std::shared_ptr<CNSTexture>
CNSMaterial::rdmoMap()
{
	for (auto& tex : m_textures)
		if (tex->type() == "RoughnessDensityMetalnessMap")
			return tex;

	auto newTex = std::make_shared<CNSTexture>();
	m_textures.push_back(newTex);
	return newTex;
}

const std::vector<std::shared_ptr<CNSTexture>>&
CNSMaterial::textures() const
{
	return m_textures;
}

