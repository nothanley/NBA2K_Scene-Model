#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CSceneSerializer;
class CNBAModel;
struct NSSkeleton;
struct Mesh;

// Model serializer
class CModelSerializer : public CSerializer
{
public:
	CModelSerializer(CSceneSerializer* parent);

	virtual std::shared_ptr<JSON> serialize() override;

private:
	void serializeMeshSkin(std::shared_ptr<JSON>& json, const std::shared_ptr<Mesh>& mesh, NSSkeleton& armature);
	std::shared_ptr<JSON> getMeshJson(const std::shared_ptr<Mesh>& mesh);
	void addModelJson(const std::shared_ptr<CNBAModel>& model);
	void setRigJson(const std::shared_ptr<CNBAModel>& model, std::shared_ptr<JSON>& root);
	void setMeshMorphs(std::shared_ptr<JSON>& json, const std::shared_ptr<Mesh>& mesh);

	CSceneSerializer* m_parent;
};

