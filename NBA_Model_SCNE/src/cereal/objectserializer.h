#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CSceneSerializer;
class CNBAModel;
struct Mesh;

// Model serializer
class CObjectSerializer : public CSerializer
{
public:
	CObjectSerializer(CSceneSerializer* parent);

	virtual std::shared_ptr<JSON> serialize() override;

private:
	void processModel(const std::shared_ptr<CNBAModel>& model);
	void processMesh(const std::shared_ptr<Mesh>& mesh);

private:
	CSceneSerializer* m_parent;
};

