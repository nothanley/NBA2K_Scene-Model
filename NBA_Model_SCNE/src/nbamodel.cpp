#include "nbamodel.h"
#include <scenefile.h>
#include <common.h>

static int gPrimOffset = 0;

CNBAModel::CNBAModel(const char* id)
	:
	m_name(id)
{
}

CNBAModel::~CNBAModel()
{
}

int CNBAModel::getNumMeshes()
{
	return m_meshes.size();
}

Mesh* CNBAModel::getMesh(int index)
{
	if (m_meshes.empty() || index > m_meshes.size())
		return nullptr;

	return m_meshes[index].get();
}

