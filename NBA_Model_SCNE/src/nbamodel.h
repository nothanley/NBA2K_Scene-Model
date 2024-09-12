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

typedef std::array<float, 2> Array2D;

enum enModelData {
	UDIM_SCALE     = 2089048676,
	UDIM_TRANSLATE = 2089048677,
	PRIM		 = 2089476285,
	VERTEXFORMAT = 4088860140,
	VERTEXSTREAM = 308573839,
	INDEXBUFFER  = 498649527,
};

class CNBAModel
{
public:
	CNBAModel(const char* id);
	CNBAModel(const char* id, JSON& data);
	~CNBAModel();

public:
	Mesh* getMesh();
	void parse();

private:
	void loadMeshData();
	void loadVertices();
	void loadIndices();
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

private:
	float g_mOffset, g_mScale;
	Array2D g_dUVscale, g_dUVoffset;

	Mesh m_mesh;
	std::vector<DataBuffer> m_vtxBfs;
	std::vector<DataBuffer> m_dataBfs;
};



