#include <meshstructs.h>
#pragma once

struct NSJoint 
{
	int index = -1;
	std::string name;
	Matrix3 transform;
	Vec3 translate;

	std::vector<std::shared_ptr<NSJoint>> children;
	std::shared_ptr<NSJoint> parent;
};

struct NSSkeleton 
{
	std::vector<std::shared_ptr<NSJoint>> joints;
};


