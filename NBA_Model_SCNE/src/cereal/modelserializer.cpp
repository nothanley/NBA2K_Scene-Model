#include <cereal/modelserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/skeletonserializer.h>
#include <cereal/mesh_json.h>
#include <cereal/skin_json.h>
#include <common.h>
#include <armature/armature.h>
#include <morphs/debug_morphs.h>

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
CModelSerializer::setRigJson(const std::shared_ptr<CNBAModel>& model, std::shared_ptr<JSON>& root)
{
	CSkeletonSerializer worker(m_parent);
	auto tfmJson  = worker.serialize();
	int numJoints = model->getSkeleton().joints.size();

	(*root)["WeightBits"]      = 16;
	(*root)["BlendIndexRange"] = std::vector<int>{ 0, numJoints };
	(*root)["Transform"]       = *tfmJson.get();
}

void
CModelSerializer::addModelJson(const std::shared_ptr<CNBAModel>& model)
{
	// collect mesh childs
	for (auto& mesh : model->getMeshes()) 
	{
		// format mesh data
		Mesh::createWAxis(mesh->vertices);
		Mesh::createWAxis(mesh->normals);

		// push mesh data to json
		auto obj = getMeshJson(mesh);

		// load skeleton data
		if (model->hasSkeleton())
		{
			setRigJson(model, obj);
			serializeMeshSkin(obj, mesh, model->getSkeleton());
			setMeshMorphs(obj, mesh);
		}

		(*m_json)[mesh->name.c_str()] = *obj.get();
	}
}

std::shared_ptr<JSON>
CModelSerializer::getMeshJson(const std::shared_ptr<Mesh>& mesh)
{
	auto json  = std::make_shared<JSON>();
	auto prims = std::make_shared<JSON>();

 	// collect mesh data
	//MeshJSON::AABBsToJson(mesh, json);
	//MeshJSON::dUVsToJson(mesh, json);
	MeshJSON::tfmToJson(mesh, json);
	MeshJSON::primsToJson(mesh, prims);
	(*json)["Prim"] = std::vector<JSON>{ *prims.get() };

	// push vertex/face data buffers
	std::string saveDir = common::get_parent_directory(m_parent->path());
	std::string subDir  = "meshbuffers/";
	
	// generate mesh buffers
	if (subDir.empty() || !common::create_folder(saveDir + "/" + subDir))
	{
		throw std::runtime_error("Failed to create mesh buffers directory"); 
	}

	MeshJSON::vertexDataToJson ( saveDir.c_str(), subDir.c_str(), mesh, json );
	MeshJSON::indexDataToJson  ( saveDir.c_str(), subDir.c_str(), mesh, json );

	return json;
}

void
CModelSerializer::serializeMeshSkin(std::shared_ptr<JSON>& json, const std::shared_ptr<Mesh>& mesh, NSSkeleton& armature)
{
	CSkinJsonEncoder skinJson(json, *mesh, armature);
	auto saveDir = common::get_parent_directory(m_parent->path());

	skinJson.encode(saveDir.c_str(), "meshbuffers/");
}

void
CModelSerializer::setMeshMorphs(std::shared_ptr<JSON>& json, const std::shared_ptr<Mesh>& mesh)
{
	JSON morphs;

	for (auto& morph : debugMorphs)
	{
		morphs[morph] = nullptr;
	}

	(*json)["Morph"] = morphs;
}


