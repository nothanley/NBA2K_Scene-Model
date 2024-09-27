/* DLL Exports for usage and interface with Blender and python api */
#include <cstring>
#include <NBAScene>
#pragma once

#define DLLEX extern "C" __declspec(dllexport)

// Class/Struct Initializer Funcs
DLLEX void* getNewSkinMesh(const char* name);
DLLEX void* getNewSkinModel(const char* name);
DLLEX void* getNewSceneObj(const char* name);
DLLEX void  freeMesh(void* pMesh);

// Mesh Data Population Funcs
DLLEX void setMeshNameInfo(void* pMesh, const char* meshName, const char* mtlName);
DLLEX void setMeshData(void* pMesh, float* position, int* indexList, int numVerts, int numFaces);
DLLEX void setMeshNormals(void* pMesh, float* normals, int size);
DLLEX void addUvMap(void* pMesh, float* texcoords, int size);
DLLEX void setNewModelBone(
	void* pModel,
	const char* name,
	float* matrices,
	const int index,
	const char* parent,
	bool reorder_matrix = true);

// Model Serializer Funcs
DLLEX void saveModelToFile(void* pModel, const char* savePath);
DLLEX void linkMeshToModel(void* pModel, void* pMesh);

// Material Serializer Funcs
DLLEX void setMeshMaterial      ( void* pMesh, const char* name );
DLLEX void setMaterialTexture   ( 
	void* pMesh,
	const char* name,
	const char* type,
	const int width,
	const int height,
	const int size, 
	float* pixmap);


