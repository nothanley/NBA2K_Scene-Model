#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CSceneSerializer;
class CNBAModel;
struct Mesh;

// Model serializer
class CModelSerializer : public CSerializer
{
public:
	CModelSerializer(CSceneSerializer* parent);

	virtual std::shared_ptr<JSON> serialize() override;

private:
	std::shared_ptr<JSON> getMeshJson(const std::shared_ptr<Mesh>& mesh);
	void addModelJson(const std::shared_ptr<CNBAModel>& model);

	CSceneSerializer* m_parent;
};

