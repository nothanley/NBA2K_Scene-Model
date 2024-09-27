#include <cereal/objectserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/meshjson.h>
#include <common.h>


CObjectSerializer::CObjectSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
}

std::shared_ptr<JSON> 
CObjectSerializer::serialize()
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

void CObjectSerializer::processModel(const std::shared_ptr<CNBAModel>& model)
{
	for (auto& mesh : model->getMeshes())
	{
		this->processMesh(mesh);
	}
}


void CObjectSerializer::processMesh(const std::shared_ptr<Mesh>& mesh)
{
	JSON objJson;
	objJson["Type"]   = "OBJECT";
	objJson["Target"] =  mesh->name;


	(*m_json)[mesh->name] = objJson;
}

