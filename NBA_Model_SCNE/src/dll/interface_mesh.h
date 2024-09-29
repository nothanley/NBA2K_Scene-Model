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
DLLEX void*           loadModelFile(const char* path, void** file);
DLLEX void*           getSceneModel(void* pNbaScene, const int index);
DLLEX int             getModelTotal(void* pNbaScene);
DLLEX int             getMeshTotal(void* pNbaModel);
DLLEX const char*     getMeshName(void* pNbaScene, const int index);
DLLEX const float*    getVertexData(void* pNbaScene, const int index);
DLLEX const float*    getMeshNormals(void* pNbaScene, const int index);
DLLEX int             getNumVerts(void* pNbaScene, const int index);
DLLEX int             getNumUvChannels(void* pNbaScene, const int index);
DLLEX const uint32_t* getMeshTriangleList(void* pNbaScene, const int index);
DLLEX int             getNumTriangles(void* pNbaScene, const int index);
DLLEX const float*    getMeshUvChannel(void* pNbaScene, const int meshIndex, const int channelIndex);

/* Interface methods for updating 'CSkinModel' rig data */
DLLEX int          getNumBones(void* pNbaModel);
DLLEX int          getBoneParentIndex(void* pNbaModel, int joint_index);
DLLEX float*       getBoneMatrix(void* pNbaModel, int joint_index);
DLLEX const char*  getBoneName(void* pNbaModel, int joint_index);
				   
/* Interface methods for updating 'CSkinModel' skin data */
DLLEX void*        getSkinData(void* pNbaModel, int mesh_index);
DLLEX const char** getAllSkinGroups(void* pSkin, int* num_groups);
DLLEX float*       getAllJointWeights(void* pSkin, const char* joint_name, int* size);

/* Interface methods for retrieving 'CSkinModel' material groups */
DLLEX const char** getAllFaceGroups(void* pSkinModel, const int meshIndex, int* size);
DLLEX void         getMaterialFaceGroup(void* pSkinModel, const int meshIndex, const int groupIndex, int* faceBegin, int* faceSize);


