#include <dll/interface_mesh.h>
#include <nbascene>
#include <vector>

void* loadModelFile(const char* filePath, void** filePtr, bool use_lods, bool split_groups)
{
    /* Initialize CModelFile Address */
    INCLUDE_LODS     = use_lods;
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

        printf("\n[CNBAInterface] Found total models: %d\n" , file->scene()->getNumModels() );
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

void release_model(void* pModel)
{
    CNBAModel* model = static_cast<CNBAModel*>(pModel);
    if (!model) return;

    delete model;
}

void release_scene(void* pScene)
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

    // Format Alignment
    mesh->alignPosition();
   
    return mesh->vertices.data();
}

int getNumVerts(void* pModel, const int index) 
{    
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

    mesh->alignNormals();

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
    // ...
}

int getNumBones(void* pNbaModel)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model) return 0;

    auto skel = model->getSkeleton();
    return skel.joints.size();
}

int getBoneParentIndex(void* pNbaModel, int joint_index)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
	if (!model)
        return -1;

	auto& skel = model->getSkeleton();
    if (joint_index > skel.joints.size())
		return -1;

    auto& joint  = skel.joints[joint_index];
    auto& parent = joint->parent;
    return (parent) ? parent->index : -1;
}

float* getBoneMatrix(void* pNbaModel, int joint_index)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model) 
        return nullptr;

    auto& skel = model->getSkeleton();
    if (joint_index > skel.joints.size())
        return nullptr;

    auto& joint = skel.joints[joint_index];
    float* matrix = new float[16];
 
    matrix[0] = joint->translate.x;
    matrix[1] = joint->translate.y;
    matrix[2] = joint->translate.z;
    return matrix; 
}

const char* getBoneName(void* pNbaModel, int joint_index)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model)
        return "";

    auto& skel = model->getSkeleton();
    if (joint_index > skel.joints.size())
        return "";;

    auto& joint = skel.joints[joint_index];
    return joint->name.c_str();
}


void* getSkinData(void* pNbaModel, int mesh_index)
{
    CNBAModel* model = static_cast<CNBAModel*>(pNbaModel);
    if (!model)
        return nullptr;

    if (mesh_index > model->getNumMeshes())
		return nullptr;

    auto mesh = model->getMesh(mesh_index);
    return &mesh->skin;
}

inline bool hasGroup(const std::vector<std::string*>& vec, const std::string* target)
{
    for (auto& string : vec)
        if (*string == *target)
            return true;

    return false;
}

const char** getAllSkinGroups(void* pSkin, int* num_groups)
{
    Skin* skin = static_cast<Skin*>(pSkin);
    if (!pSkin)
        return nullptr;

    // Get all bones from skin
    std::vector<std::string*> groups;
    for (auto& vertex : skin->blendverts) {
        for (auto& bone : vertex.bones)
        {
            if (!hasGroup(groups, &bone))
                groups.push_back(&bone);
        }
    }

    // Convert std::vector<std::string> to array of char pointers
    *num_groups = static_cast<int>(groups.size());
    const char** arr = new const char* [*num_groups];
    for (size_t i = 0; i < *num_groups; ++i) {
        arr[i] = groups[i]->c_str();
    }

    return arr;
}

float* getAllJointWeights(void* pSkin, const char* joint_name, int* size)
{
    Skin* skin = static_cast<Skin*>(pSkin);
    if (!pSkin)
        return nullptr;

    int numVerts      = skin->blendverts.size();
    float* vtxWeights = new float[numVerts];

    // Iterate through skin data and get a weight list for all verts of specified bone..
    for (int i = 0; i < numVerts; i++)
    {
        auto& skinVtx   = skin->blendverts.at(i);
        int numVtxBones = skinVtx.bones.size();
        vtxWeights[i]   = 0.0f;

        for (int j = 0; j < numVtxBones; j++)
            if (skinVtx.bones.at(j) == joint_name)
            {
                vtxWeights[i] = skinVtx.weights.at(j);
                break;
            }
    }

    *size = numVerts;
    return vtxWeights;
}


