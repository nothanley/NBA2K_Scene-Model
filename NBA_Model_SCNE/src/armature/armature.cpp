#include <armature/armature.h>


std::shared_ptr<NSJoint> NSSkeleton::findJoint(const char* name) const
{
	for (auto& joint : joints)
	{
		if (joint->name == name)
			return joint;
	}

	return nullptr;
}

void NSSkeleton::addJoint(const std::shared_ptr<NSJoint>& joint)
{
	joints.push_back(joint);
}

