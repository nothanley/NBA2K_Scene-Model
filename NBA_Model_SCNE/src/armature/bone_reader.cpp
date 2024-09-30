#include <armature/bone_reader.h>
#include <common.h>

CBoneReader::CBoneReader(JSON& data, NSSkeleton& skeleton)
	:
	m_json(&data),
	m_skeleton(&skeleton)
{
}

void CBoneReader::fromJSON(JSON& obj, NSSkeleton& skeleton)
{
	CBoneReader reader(obj, skeleton);
	reader.parse();
}

inline static size_t getNumJsonChilds(JSON& obj)
{
	size_t size = 0;
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
		if (it.value().is_object())
			size++;
	return size;
}

void CBoneReader::parse()
{
	// validate JSON and skeleton
	if (!m_json || !m_skeleton)
		return;

	// get size of skeleton
	size_t size = ::getNumJsonChilds(*m_json);
	m_skeleton->joints.resize(size);

	// create joints
	for (auto& joint : m_skeleton->joints)
		joint = std::make_shared<NSJoint>();

	// Load bone data from JSON iterator
	int index = 0;
	for (JSON::iterator it = m_json->begin(); it != m_json->end(); ++it)
		if (it.value().is_object())
		{
			loadTransformDef(index, it.key(), it.value());
			index++;
		}
}

inline static void loadParent(JSON::iterator& it, std::shared_ptr<NSJoint>& joint, const std::vector<std::shared_ptr<NSJoint>>& joints)
{
	int parent_index = it.value();
	if (parent_index > joints.size())
		return;

	joint->parent = joints[parent_index];
	joint->parent->children.push_back(joint);
}

inline static void loadChild(JSON::iterator& it, std::shared_ptr<NSJoint>& joint, const std::vector<std::shared_ptr<NSJoint>>& joints)
{
	int child_index = it.value();
	if (child_index > joints.size())
		return;

	joints[child_index]->parent = joint;
}

inline static void loadTranslate(JSON::iterator& it, std::shared_ptr<NSJoint>& joint)
{
	std::vector<float> translate = it.value();
	joint->translate = { translate[0], translate[1], translate[2] };
}

void CBoneReader::loadTransformDef(const int index, const std::string& key, JSON & obj)
{
	// create joint
	auto& joint = m_skeleton->joints[index];
	joint->name  = key;
	joint->index = index;

	// collect bone data
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
			case enBoneTag::PARENT:
				::loadParent(it, joint, m_skeleton->joints);
				break;
			case enBoneTag::CHILD:
				::loadChild(it, joint, m_skeleton->joints);
				break;
			case enBoneTag::SIBLING:
				break;
			case enBoneTag::TRANSLATE:
				::loadTranslate(it, joint);
				break;
			default:
				break;
		};
	}
}

 