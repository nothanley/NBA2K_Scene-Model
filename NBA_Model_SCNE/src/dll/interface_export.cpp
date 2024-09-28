#include <dll/interface_export.h>
#include <cereal/sceneserializer.h>
#include <material/material.h>

void freeMesh(void* pMesh)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	delete mesh;
}

void* getNewSkinMesh(const char* name)
{
	return new Mesh{ name };
}

void* getNewSkinModel(const char* name)
{
	return new CNBAModel(name);
}

void* getNewSceneObj(const char* name)
{
	return new CNBAScene(name);
}

void setMeshNameInfo(void* pMesh, const char* meshName, const char* mtlName)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	mesh->name = meshName;
	mesh->material.setName(mtlName);
	return;
}

void setMeshData(void* pMesh, float* position, int* indexList, int numVerts, int numFaces)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	/* Populate position coords - (+X-Z+Y)*/
	mesh->vertices.resize(numVerts);
	for (int i = 0; i < numVerts; i += 3) {
		mesh->vertices[i]     = position[i];
		mesh->vertices[i + 1] = position[i + 1];
		mesh->vertices[i + 2] = position[i + 2];
	}

	/* Set triangle indices */
	numFaces /= 3;
	mesh->triangles.resize(numFaces);
	for (int i = 0; i < numFaces; i++)
	{
		Triangle& face = mesh->triangles.at(i);
		size_t index = (i * 3);

		face[0] = indexList[index];
		face[1] = indexList[index + 1];
		face[2] = indexList[index + 2];
	}

	/* Update counts */
	mesh->alignPosition(true);
	mesh->generateAABBs();
}

void setMeshNormals(void* pMesh, float* normals, int size)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	if ((mesh->vertices.size() / 3) != (size / 3)) {
		printf("[CSkinModel] Vertex normal count mismatch.");
		throw std::runtime_error("");
	}

	/* Populate vertex normals -- seems slow */
	mesh->normals.resize(size);
	for (int i = 0; i < size; i++)
		mesh->normals[i] = normals[i];

	// Pack tangent frames
	mesh->alignNormals(true);
	MeshCalc::calculateTangentsBinormals(*mesh);
	MeshCalc::buildTangentFrameVec(*mesh, mesh->tangent_frames);
}

void addUvMap(void* pMesh, float* texcoords, int size)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	/* Create a new uv channel */
	UVMap channel;
	channel.map.resize(size);
	for (int i = 0; i < size; i += 2) {
		channel.map.at(i) = texcoords[i];
		channel.map.at(i + 1) = -(-1.0 + texcoords[i + 1]);
	}

	mesh->uvs.push_back(channel);
}

void setMeshSkinData(void* pMesh, int* indices, float* weights, int size, int numWeightsPerVtx)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh)
		return;

	//printf("\n[CSkinModel] Populating Skin with limit: %d\n", numWeightsPerVtx);
	
}

void saveModelToFile(void* pModel, const char* savePath)
{
	// todo: add cereal implementation ...
	auto model = static_cast<CNBAModel*>(pModel);
	if (!model || (model->getNumMeshes() == 0)) return;
	
	printf("\n[CSkinModel] Saving model to file: %s\n", savePath);
	auto scene_id = model->getMesh()->name;
	auto scene = std::make_shared<CNBAScene>(scene_id.c_str());
	scene->pushModel(*model);

	CSceneSerializer serializer(scene);
	serializer.save(savePath);
}

void setNewModelBone(
	void* pModel,
	const char* name,
	float* matrices,
	const int index,
	const char* parent,
	bool reorder_matrix)
{
	//todo: add bone implemetation ...
};

void linkMeshToModel(void* pModel, void* pMesh)
{
	// Convert void pointer back to CNBAModel pointer
	CNBAModel* model = static_cast<CNBAModel*>(pModel);
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	model->pushMesh(*mesh);

	// release mesh
	::freeMesh(mesh);
	return;
}

void setMeshMaterial(void* pMesh, const char* name)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	mesh->material.setName(name);
}
 
void setMaterialTexture(void* pMesh, const char* name, const char* type, const int width, const int height, const int size, float* pixmap)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	// Create a new texture map
	auto& material = mesh->material;
	auto  texture  = std::make_shared<CNSTexture>(name, width, height);

	// Set texture pixmap
	texture->setPixmap(pixmap, size);
	texture->setType(type);
	material.addTexture(texture);
}



