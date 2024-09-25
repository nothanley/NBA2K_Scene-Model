#include <cereal/modelserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/meshjson.h>

CModelSerializer::CModelSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
}

std::shared_ptr<JSON> 
CModelSerializer::serialize()
{
	auto scene = m_parent->scene();
	m_json = std::make_shared<JSON>();

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
	MeshJSON::primsToJson(mesh, prims);
	(*m_json)["Prim"] = *prims.get();
	
	// push vertex/face data buffers
	MeshJSON::vertexDataToJson(mesh, json);
	MeshJSON::indexDataToJson(mesh, json);

	return json;
}



