#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CNSTexture;
class CSceneSerializer;
class CNBAModel;
struct Mesh;

// Model serializer
class CTextureSerializer : public CSerializer
{
public:
	CTextureSerializer(CSceneSerializer* parent);

	virtual std::shared_ptr<JSON> serialize() override;

private:
	void processModel(const std::shared_ptr<CNBAModel>& model);
	void processMesh(const std::shared_ptr<Mesh>& mesh);

private:
	std::shared_ptr<JSON> getTextureJson(const std::shared_ptr<CNSTexture>& texture);

private:
	CSceneSerializer* m_parent;
};



