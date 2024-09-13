#include "interfaceskinmodel.h"
#include <nbascene>
#include <vector>

void* loadModelFile(const char* filePath, void** filePtr)
{
    /* Initialize CModelFile Address */
    *filePtr = nullptr;

    /* Load file and scenecontents */
    try {
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

void release_model(void* skinmodel)
{
    CNBAScene* scene = static_cast<CNBAScene*>(skinmodel);
    if (!scene) return;

    delete scene;
}

int getMeshTotal(void* pSkinModel) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene = static_cast<CNBAScene*>(pSkinModel);
    if (!scene)
        return 0;

    /* accumulate mesh total */
    return scene->getNumModels();
}

const float* getVertexData(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene = static_cast<CNBAScene*>(pSkinModel);
    if (!scene || index > scene->getNumModels())
        return nullptr;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();
    return mesh->vertices.data();
}

int getNumVerts(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return 0;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();

    printf("\n[CNBAInterface] Loading Mesh ID: %s | Index: %d", mesh->name.c_str(), index);
    return mesh->vertices.size() / 3;
}

int getNumUvChannels(void* pSkinModel, const int index) 
{
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return 0;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();
    return mesh->uvs.size();
}

const float* getMeshUvChannel(void* pSkinModel, const int meshIndex, const int channelIndex)
{
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| meshIndex > scene->getNumModels())
        return 0;

    /* Load mesh */
    auto  mesh = scene->models().at(meshIndex)->getMesh();
    auto& uvs  = mesh->uvs;

    if (channelIndex > uvs.size())
        return nullptr;

    /* Extrapolate uv channel */
    //mesh->translateUVs(channelIndex);

    auto& channel = uvs.at(channelIndex).map;
    return channel.data(); 
}

const char* getMeshName(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return "";

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();
    return mesh->name.c_str();
}

int getNumTriangles(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return 0;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();

    return mesh->triangles.size();
}

const uint32_t* getMeshTriangleList(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return nullptr;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();
     
    /* Cast triangle vector to index list */
    auto& tris = mesh->triangles;
    return (uint32_t*)tris.data();
}


const float* getMeshNormals(void* pSkinModel, const int index) {
    // Convert void pointer back to CNBAScene pointer
    CNBAScene* scene= static_cast<CNBAScene*>(pSkinModel);
    if (!scene|| index > scene->getNumModels())
        return nullptr;

    /* Load mesh */
    auto mesh = scene->models().at(index)->getMesh();
    //mesh->convertSplitNorms();

    /* Rearrange normal data */
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
    // Convert void pointer back to CNBAScene pointer
    std::vector<BlendWeight>* skin = static_cast<std::vector<BlendWeight>*>(pSkinData);
    if (!skin)
        return;

    delete skin;
}
