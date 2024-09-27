#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
struct Mesh;
class CNSMaterial;
class CNSEffect;
class CSceneSerializer;
class CNBAModel;

// Material serializer
class CEffectSerializer : public CSerializer
{
public:
	CEffectSerializer(CSceneSerializer* parent);
	virtual std::shared_ptr<JSON> serialize() override;

private:
	void getMaterialPresets();
	void processModel(const std::shared_ptr<CNBAModel>& model);
	void getMeshFXJson(const std::shared_ptr<Mesh>& mesh);
	void saveDiskShaders(const std::shared_ptr<CNSEffect>& effect);

private:
	std::vector<std::shared_ptr<CNSMaterial>> m_defaultMats;
	CSceneSerializer* m_parent;
};

