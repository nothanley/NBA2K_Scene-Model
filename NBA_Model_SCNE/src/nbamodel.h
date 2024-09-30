
#include <meshprimitive.h>
#include <armature/armature.h>
#include <databuffer.h>
#pragma once

class CNBAModel
{
public:
	CNBAModel(const char* id);
	~CNBAModel();

public:
	int getNumMeshes();
	Mesh* getMesh(int index = 0);
	std::vector<std::shared_ptr<Mesh>> getMeshes();
	std::string name();

public:
	bool hasSkeleton();
	void pushMesh(const Mesh& mesh);
	NSSkeleton& getSkeleton();

protected:
	std::string m_name;
	NSSkeleton m_skeleton;
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<StGeoPrim> m_primitives;
	std::vector<Array2D> g_uvDeriv;
	int m_weightBits;
};




