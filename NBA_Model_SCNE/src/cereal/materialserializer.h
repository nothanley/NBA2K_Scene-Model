#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CNSMaterial;
class CSceneSerializer;
class CNBAModel;
struct Mesh;

// Material serializer
class CMaterialSerializer : public CSerializer
{
public:
	CMaterialSerializer(CSceneSerializer* parent);
	virtual std::shared_ptr<JSON> serialize() override;

private:
	void getMaterialPresets();
	void processModel(const std::shared_ptr<CNBAModel>& model);
	void getMeshMatJson(const std::shared_ptr<Mesh>& mesh);

private:
	std::vector<std::shared_ptr<CNSMaterial>> m_defaultMats;
	CSceneSerializer* m_parent;
};

