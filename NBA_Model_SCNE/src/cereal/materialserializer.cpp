#include <cereal/materialserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/meshjson.h>

#include <material/effect.h>
#include <material/material.h>
#include <common.h>


CMaterialSerializer::CMaterialSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
	this->getMaterialPresets();
}

void CMaterialSerializer::getMaterialPresets()
{
	// find preset directory path
	std::string presetDir = common::get_module_directory() + "/Preset-Mats";

	// load default materials
	printf("\nScanning for materials in: %s\n", presetDir.c_str());
	m_defaultMats = CNSMaterial::getMaterialLib(presetDir.c_str());
}

std::shared_ptr<JSON> 
CMaterialSerializer::serialize()
{
	auto scene = m_parent->scene();
	m_json->clear();

	// collect model childs
	for (auto& model : scene->models())
	{
		this->processModel(model);
	}

	return m_json;
}

void CMaterialSerializer::processModel(const std::shared_ptr<CNBAModel>& model)
{
	if (m_defaultMats.empty())
		return;

	// collect mesh childs
	for (auto& mesh : model->getMeshes()) 
	{
		this->getMeshMatJson(mesh);
	}
}

void CMaterialSerializer::getMeshMatJson(const std::shared_ptr<Mesh>& mesh)
{
	JSON  mtlJson;
	auto& material     = m_defaultMats.front();
	auto  mtlId        = mesh->material.name();

	// find material by id
	for (auto& mat : m_defaultMats)
		if (mat->name() == mtlId)
		{
			material = mat;
			break;
		}

	// update material id
	material->setName( mtlId.c_str() );
	material->setTextures(mesh->material.textures());
	material->updateTexLinks(mesh->name);

	// populate material json data
	mtlJson["Effect"]    = material->effect()->id();
	mtlJson["Parameter"] = material->data()["Parameter"];
	mtlJson["Resource"]  = material->data()["Resource"];
	mtlJson["Technique"] = material->data()["Technique"];

	(*m_json)[material->name()] = mtlJson;
}


