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

enum enModelData {
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
	Mesh m_mesh;
	std::vector<UVMap> m_uvs;
	std::vector<DataBuffer> m_vtxBfs;
	std::vector<DataBuffer> m_dataBfs;
};



