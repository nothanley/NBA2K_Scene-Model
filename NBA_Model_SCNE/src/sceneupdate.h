// File object and container class for all .scene data. Parses and extrapolates CNBAScene from given JSON file path.

#include <scenefile.h>
#pragma once

struct Mesh;
struct StUpdatePkg
{
    std::string id;
    float* position;
    float* texcoords;
    float* normals;
    int numVerts;
    int numFaces;
    int search_method;
};

class CSceneUpdate : public CSceneFile
{
public:
	CSceneUpdate(const char* path, const bool fix_mesh=false);
	~CSceneUpdate();

public:
	void update(StUpdatePkg* data);
    void clear();

private:
    void updateTarget();
    void findTarget();
    void buildVertexMap();
   
private:
    void getUpdatedVertices();
    void getUpdatedNormals();
    void updateVertexBuffer();
    void updateTangentBuffer();

private:
    int m_numVtxComponents;
    bool doMeshFix;
    StUpdatePkg* m_data;
    std::shared_ptr<Mesh> m_updateMesh;
    std::shared_ptr<Mesh> m_targetMesh;
    std::vector<int> m_vertexMap; // holds vertex index id
};

