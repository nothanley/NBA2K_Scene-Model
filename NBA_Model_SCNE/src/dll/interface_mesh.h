/* DLL Exports for usage and interface with Blender and python api */

#include <cstring>
#include <NBAScene>
#pragma once

#define DLLEX extern "C" __declspec(dllexport)

/* Free memory allocated during interface */
DLLEX void release_scene(void* scene);
DLLEX void release_model(void* skinmodel);
DLLEX void release_model_file(void* model_file);

DLLEX void freeMemory_float32(float* data);
DLLEX void freeMemory_intArr(int* data);
DLLEX void freeMemory_charArrPtr(const char** data);
DLLEX void freeMemory_skinData(void* pSkinData);

/* Interface methods for accessing 'CSkinModel' object data */
DLLEX void* loadModelFile(const char* path, void** file, bool use_lods, bool split_groups);
DLLEX void* getSceneModel(void* pNbaScene, const int index);
DLLEX int getModelTotal(void* pNbaScene);
DLLEX int getMeshTotal(void* pNbaModel);
DLLEX const char* getMeshName(void* pNbaScene, const int index);
DLLEX const float* getVertexData(void* pNbaScene, const int index);
DLLEX const float* getMeshNormals(void* pNbaScene, const int index);
DLLEX int getNumVerts(void* pNbaScene, const int index);
DLLEX int getNumUvChannels(void* pNbaScene, const int index);
DLLEX const uint32_t* getMeshTriangleList(void* pNbaScene, const int index);
DLLEX int getNumTriangles(void* pNbaScene, const int index);
DLLEX const float* getMeshUvChannel(void* pNbaScene, const int meshIndex, const int channelIndex);


