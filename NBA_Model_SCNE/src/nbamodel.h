/* Stores special model attributes and child Meshes for model object. 
   Extrapolates mesh data from given JSON container. */

#include <fstream>
#include <istream>
#include <json.hpp>
#include <meshstructs.h>
#include <meshbuffer.h>
#pragma once

struct RigBone;
struct Material;
struct BoundingBox;

class CSceneFile;
using JSON = nlohmann::json;

namespace GeomDef
{
	void pushPrimLods(StGeoPrim& prim, std::vector<StGeoPrim>& prim_vec);
	void setMeshVtxs(DataBuffer* posBf, Mesh& mesh);
	void calculateVtxNormals(DataBuffer* tanBf, Mesh& mesh);
	void addMeshUVMap(DataBuffer* texBf, Mesh& mesh);
};

extern bool INCLUDE_LODS; // toggles lower level meshes

class CNBAModel
{
public:
	CNBAModel(const char* id);
	CNBAModel(const char* id, JSON& data);
	~CNBAModel();

public:
	int getNumMeshes();
	Mesh* getMesh(int index = 0);
	void parse();

private:
	void loadMeshData();
	void loadVertices(Mesh& mesh);
	void loadIndices(Mesh& mesh, const int count);
	void loadMesh(StGeoPrim& prim);

private:
	void readPrim(JSON& obj);
	void readVertexFmt(JSON& obj);
	void readVertexStream(JSON& obj);
	void readIndexBuffer(JSON& obj);

private:
	DataBuffer* findDataBuffer(const char* id);
	DataBuffer* getVtxBuffer(int index);

private:
	std::string m_name;
	JSON m_json;
	CSceneFile* m_parent;

	std::vector<Mesh> m_meshes;
	std::vector<StGeoPrim>  m_groups;
	std::vector<DataBuffer> m_vtxBfs;
	std::vector<DataBuffer> m_dataBfs;
	std::vector<Array2D> g_uvDeriv;
};



