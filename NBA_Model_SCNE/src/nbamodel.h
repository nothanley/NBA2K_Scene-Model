
#include <meshprimitive.h>
#include <meshbuffer.h>
#pragma once

struct RigBone;
struct Material;
struct BoundingBox;

class CNBAModel
{
public:
	CNBAModel(const char* id);
	~CNBAModel();

public:
	int getNumMeshes();
	Mesh* getMesh(int index = 0);

protected:
	std::string m_name;
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<StGeoPrim> m_groups;
	std::vector<Array2D> g_uvDeriv;
};




