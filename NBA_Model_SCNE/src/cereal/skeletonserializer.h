#include <cereal/genericserializer.h>
#pragma once

// Forward declaration
class CSceneSerializer;
class CNBAModel;
struct Mesh;
struct NSSkeleton;

// Model serializer
class CSkeletonSerializer : public CSerializer
{
public:
	CSkeletonSerializer(CSceneSerializer* parent);

	virtual std::shared_ptr<JSON> serialize() override;

private:
	void generateTransforms(std::shared_ptr<JSON> json, NSSkeleton& skeleton);

private:
	CSceneSerializer* m_parent;
};

