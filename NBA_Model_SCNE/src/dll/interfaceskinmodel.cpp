#include "interfaceskinmodel.h"
#include <nbascene>
#include <vector>

void* loadModelFile(const char* filePath, void** filePtr, bool use_lods, bool split_groups)
{
    /* Initialize CModelFile Address */
    INCLUDE_LODS = use_lods;
    MERGE_MESH_PRIMS = !split_groups;
    *filePtr = nullptr;

    /* Load file and scene-contents */
    try 
    {
        CSceneFile* file = new CSceneFile(filePath);
        file->load();

        if (file->scene()->empty())
            return nullptr;

        auto& scene = file->scene();
        *filePtr = file;

        printf("\n[CNBAInterface] Found total models: %d" , file->scene()->getNumModels() );
        return scene.get();
    }
    catch (...) {}

    printf("[CNBAScene] Failed to read user scenefile.\n");
    return nullptr;
}

void release_model_file(void* filePtr)
{
    CSceneFile* file = static_cast<CSceneFile*>(filePtr);
    if (!file) return;

    delete file;
}

void release_model(void* pScene)
{
    CNBAScene* scene = static_cast<CNBAScene*>(pScene);
    if (!scene) return;

    delete scene;
}

int getModelTotal(void* pNbaScene)
{
    CNBAScene* scene = static_cast<CNBAScene*>(pNbaScene);
    
    if (!scene)
        return 0;

    return scene->getNumModels();
}

int getMeshTotal(void* pNbaModel) 
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model)
    {
        printf("\n[Interface] Failed to load CNBAModel object.");
        return 0;
    }

    /* accumulate mesh total */
    return model->getNumMeshes();
}

const float* getVertexData(void* pNbaModel, const int index)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model || index > model->getNumMeshes())
        return nullptr;

    /* Load mesh */
    auto mesh = model->getMesh(index);

    return mesh->vertices.data();
}

int getNumVerts(void* pModel, const int index) {
    
    CNBAModel* model= static_cast<CNBAModel*>(pModel);
    if (!model || index > model->getNumMeshes())
        return 0;

    /* Load mesh */
    auto mesh = model->getMesh(index);
    return mesh->vertices.size() / 3;
}

int getNumUvChannels(void* pNbaModel, const int index)
{
 
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model || index > model->getNumMeshes())
        return 0;

    /* Load mesh */
    auto mesh = model->getMesh(index);
    return mesh->uvs.size();
}

const float* getMeshUvChannel(void* pNbaModel, const int meshIndex, const int channelIndex)
{
    
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model || meshIndex > model->getNumMeshes())
        return 0;

    /* Load mesh */
    auto mesh = model->getMesh(meshIndex);
    auto& uvs  = mesh->uvs;

    if (channelIndex > uvs.size())
        return nullptr;

    auto& channel = uvs.at(channelIndex).map;
    return channel.data(); 
}

void* getSceneModel(void* pNbaScene, const int index)
{
    
    CNBAScene* scene = static_cast<CNBAScene*>(pNbaScene);
    if (!scene || index > scene->getNumModels())
        return 0;

    auto model = scene->model(index);
    return model.get();
}

const char* getMeshName(void* pNbaModel, int mesh_index) 
{
    // Convert void pointer back to CNbaModel pointer
    CNBAModel* model= static_cast<CNBAModel*>(pNbaModel);
    if (!model || mesh_index > model->getNumMeshes())
        return "";
    
    return model->getMesh(mesh_index)->name.c_str();
}

int getNumTriangles(void* pNbaModel, const int index) 
{
    // Convert void pointer back to CNBAModel pointer
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model || index > model->getNumMeshes())
        return 0;

    /* Load mesh */
    auto mesh = model->getMesh(index);

    return mesh->triangles.size();
}

const uint32_t* getMeshTriangleList(void* pNbaModel, const int index) {
    // Convert void pointer back to CNBAModel pointer
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model || index > model->getNumMeshes())
        return nullptr;

    /* Load mesh */
    auto mesh = model->getMesh(index);
     
    /* Cast triangle vector to index list */
    auto& tris = mesh->triangles;
    return (uint32_t*)tris.data();
}


const float* getMeshNormals(void* pNbaModel, const int index) {
    // Convert void pointer back to CNBAModel pointer
    CNBAModel* model= static_cast<CNBAModel*>(pNbaModel);
    if (!model || index > model->getNumMeshes())
        return nullptr;

    /* Load mesh */
    auto mesh = model->getMesh(index);

    return mesh->normals.data();
}

void freeMemory_intArr(int* data)
{
    if (!data) return;
    delete[] data;
}

void freeMemory_float32(float* set) 
{
    if (!set) return;
    delete[] set;
}

void freeMemory_charArrPtr(const char** set)
{
    if (!set) return;
    delete[] set;
}

void freeMemory_skinData(void* pSkinData)
{
    
    std::vector<BlendWeight>* skin = static_cast<std::vector<BlendWeight>*>(pSkinData);
    if (!skin)
        return;

    delete skin;
}
