#include <cereal/skeletonserializer.h>
#include <cereal/sceneserializer.h>

CSkeletonSerializer::CSkeletonSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
}

std::shared_ptr<JSON> 
CSkeletonSerializer::serialize()
{
	auto scene = m_parent->scene();
	m_json->clear();
	
	// Validate scene models
	if (scene->models().empty())
		return m_json;
	
	// Get root skeleton
	auto skeleton = scene->models().front()->getSkeleton();
	this->generateTransforms(m_json, skeleton);

	return m_json;
}

inline static int findFirstChild(
	const std::string& target,
	const std::vector<std::shared_ptr<NSJoint>>& joints,
	const int ignore_index = -1)
{
	for (int i = 0; i < joints.size(); i++)
	{
		auto& joint = joints[i];

		if (joint->parent && joint->parent->name == target && i != ignore_index)
			return i;
	}

	return -1;
}

void
CSkeletonSerializer::generateTransforms(std::shared_ptr<JSON> json, NSSkeleton& skeleton)
{
	const int numJoints = skeleton.joints.size();
	int parent, child, sibling;

	for (int i = 0; i < numJoints; i++)
	{
		JSON tfmJson;
		auto& joint = skeleton.joints[i];

		// Populate joint data
		child   = ::findFirstChild(joint->name, skeleton.joints);
		parent  = (joint->parent) ? joint->parent->index : -1;
		sibling = (joint->parent) ? ::findFirstChild(joint->parent->name, skeleton.joints, i) : -1;

		if (parent != -1)
			tfmJson["Parent"] = parent;

		if (child != -1)
			tfmJson["Child"] = child;

		if (sibling != -1)
			tfmJson["Sibling"] = sibling;

		tfmJson["Translate"]   = std::vector<float>{ joint->translate.x, joint->translate.y, joint->translate.z };
		(*m_json)[joint->name] = tfmJson;
	}
}

