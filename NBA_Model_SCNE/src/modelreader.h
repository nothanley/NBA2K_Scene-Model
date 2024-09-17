/* Stores special model attributes and child Meshes for model object.
   Extrapolates mesh data from given JSON container. */

#include <nbamodel.h>
#include <fstream>
#include <istream>
#pragma once

class CSceneFile;
using JSON = nlohmann::json;

class CModelReader : public CNBAModel
{
public:
	CModelReader(const char* id, JSON& data);
	~CModelReader();

	void parse();

private:
	inline void mergeMeshGroups();
	inline void createSkinMeshes();

private:
	void loadMeshData();
	void loadVertices(Mesh& mesh);
	void loadIndices(Mesh& mesh, const int count);
	void loadMesh(StGeoPrim& prim);
	void readPrim(JSON& obj);
	void readVertexFmt(JSON& obj);
	void readVertexStream(JSON& obj);
	void readIndexBuffer(JSON& obj);

private:
	DataBuffer* findDataBuffer(const char* id);
	DataBuffer* getVtxBuffer(int index);

private:
	JSON m_json;
	std::vector<DataBuffer> m_vtxBfs;
	std::vector<DataBuffer> m_dataBfs;
	CSceneFile* m_parent;
};



