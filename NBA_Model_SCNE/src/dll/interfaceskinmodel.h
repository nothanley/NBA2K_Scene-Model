/* DLL Exports for usage and interface with Blender and python api */

#include <cstring>
#include <NBAScene>
#pragma once


/* Free memory allocated during interface */
extern "C" __declspec(dllexport) void freeMemory_float32(float* data);

extern "C" __declspec(dllexport) void freeMemory_intArr(int* data);

extern "C" __declspec(dllexport) void release_model(void* skinmodel);

extern "C" __declspec(dllexport) void release_model_file(void* pModelFile);

extern "C" __declspec(dllexport) void freeMemory_charArrPtr(const char** data);

extern "C" __declspec(dllexport) void freeMemory_skinData(void* pSkinData);

/* Interface methods for accessing 'CSkinModel' object data */
extern "C" __declspec(dllexport) void* loadModelFile(const char* path, void** file, bool use_lods);

extern "C" __declspec(dllexport) void* getSceneModel(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) int getModelTotal(void* pNbaScene);

extern "C" __declspec(dllexport) int getMeshTotal(void* pNbaModel);

extern "C" __declspec(dllexport) const char* getMeshName(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) const float* getVertexData(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) const float* getMeshNormals(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) int getNumVerts(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) int getNumUvChannels(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) const uint32_t* getMeshTriangleList(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) int getNumTriangles(void* pNbaScene, const int index);

extern "C" __declspec(dllexport) const float* getMeshUvChannel(void* pNbaScene, const int meshIndex, const int channelIndex);


