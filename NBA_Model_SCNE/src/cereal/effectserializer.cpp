#include <cereal/effectserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/mesh_json.h>

#include <material/effect.h>
#include <material/material.h>
#include <common.h>


CEffectSerializer::CEffectSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
	this->getMaterialPresets();
}

void CEffectSerializer::getMaterialPresets()
{
	// find preset directory path
	std::string presetDir = common::get_module_directory() + "/Preset-Mats";

	// load default materials
	m_defaultMats = CNSMaterial::getMaterialLib(presetDir.c_str());
}

std::shared_ptr<JSON> 
CEffectSerializer::serialize()
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

void CEffectSerializer::processModel(const std::shared_ptr<CNBAModel>& model)
{
	if (m_defaultMats.empty())
		return;

	// collect mesh childs
	for (auto& mesh : model->getMeshes()) 
	{
		this->getMeshFXJson(mesh);
	}
}

void CEffectSerializer::getMeshFXJson(const std::shared_ptr<Mesh>& mesh)
{
	auto& material     = m_defaultMats.front();
	auto  mtlId        = mesh->material.name();

	// find material by id
	for (auto& mat : m_defaultMats)
		if (mat->name() == mtlId)
		{
			material = mat;
			break;
		}

	// update effect id
	auto effect = material->effect();
	effect->setName(mtlId.c_str());
	this->saveDiskShaders(effect);

	// populate effect json data
	(*m_json)[effect->id()] = *(effect->data());
}

void CEffectSerializer::saveDiskShaders(const std::shared_ptr<CNSEffect>& effect)
{
	// get target shader directory
	std::string subdir    = "shaders"; // save shaders in unique sub folder
	std::string shaderDir = common::get_parent_directory( m_parent->path() ) + "/" + subdir;
	auto shaders          = effect->shaders();

	if (!common::create_folder(shaderDir))
		throw std::runtime_error("Failed to create mesh buffers directory");

	// copy all shaders to disk
	effect->updateShaderPaths(subdir.c_str());
	for (auto& shader : shaders)
	{
		auto dstPath = shaderDir + "/" + shader.name;

		try 
		{
			std::filesystem::copy(shader.path, dstPath, std::filesystem::copy_options::overwrite_existing);
		}
		catch (...){}
	}
}

