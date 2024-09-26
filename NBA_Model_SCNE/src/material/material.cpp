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

void 
CNSMaterial::loadFromDisk(const char* path)
{
	if (!setPath(path))
		return;

	// load material data ...
	this->loadFX();
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
	m_effect       = std::make_shared<CNSEffect>();
	m_effect->path = fxPath;
	m_effect->data = fxJson;
	m_effect->loadShaderResources();
}




