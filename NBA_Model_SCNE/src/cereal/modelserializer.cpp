#include <cereal/modelserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/meshjson.h>
#include <common.h>

CModelSerializer::CModelSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
}

std::shared_ptr<JSON> 
CModelSerializer::serialize()
{
	auto scene = m_parent->scene();
	m_json->clear();

	// collect model childs
	for (auto& model : scene->models())
		addModelJson(model);

	return m_json;
}

void
CModelSerializer::addModelJson(const std::shared_ptr<CNBAModel>& model)
{
	// collect mesh childs
	for (auto& mesh : model->getMeshes()) 
	{
		auto obj = getMeshJson(mesh);
		(*m_json)[mesh->name.c_str()] = *obj.get();
	}
}

std::shared_ptr<JSON>
CModelSerializer::getMeshJson(const std::shared_ptr<Mesh>& mesh)
{
	auto json  = std::make_shared<JSON>();
	auto prims = std::make_shared<JSON>();

 	// collect mesh data
	MeshJSON::AABBsToJson(mesh, json);
	MeshJSON::dUVsToJson(mesh, json);
	MeshJSON::tfmToJson(mesh, json);
	MeshJSON::skinDataToJson(mesh, json);
	MeshJSON::primsToJson(mesh, prims);
	(*json)["Prim"] = *prims.get();
	
	// get (mesh buffer) save directory
	auto saveDir = common::get_parent_directory(m_parent->path());

	// push vertex/face data buffers
	MeshJSON::vertexDataToJson(saveDir.c_str(), mesh, json);
	MeshJSON::indexDataToJson( saveDir.c_str(), mesh, json);

	return json;
}



