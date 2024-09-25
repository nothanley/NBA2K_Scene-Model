#include <dll/interface_export.h>

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
	mesh->material.name = mtlName;
	return;
}

void setMeshData(void* pMesh, float* position, int* indexList, int numVerts, int numFaces)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	/* Populate position coords - (+X-Z+Y)*/
	mesh->vertices.resize(numVerts);
	for (int i = 0; i < numVerts; i += 3) {
		mesh->vertices[i] = position[i];
		mesh->vertices[i + 1] = -position[i + 2];
		mesh->vertices[i + 2] = position[i + 1];
	}

	/* Set triangle indices */
	numFaces /= 3;
	mesh->triangles.resize(numFaces);
	for (int i = 0; i < numFaces; i++)
	{
		Triangle& face = mesh->triangles.at(i);
		size_t index = (i * 3);

		face[0] = indexList[index + 1];
		face[1] = indexList[index];
		face[2] = indexList[index + 2];
	}

	/* Update counts */
	mesh->generateAABBs();

	/* Setup mesh default mtl */
	FaceGroup faceMat{ mesh->material, 0, numFaces };
	mesh->groups.push_back(faceMat);
}

void setMeshNormals(void* pMesh, float* normals, int size)
{
	Mesh* mesh = static_cast<Mesh*>(pMesh);
	if (!mesh) return;

	int numVerts = mesh->vertices.size() / 3;
	int numNorms = size / 3;

	if (numNorms != numVerts) {
		printf("[CSkinModel] Vertex normal count mismatch.");
		throw std::runtime_error("");
	}

	/* Populate vertex normals -- seems slow */
	for (int i = 0; i < size; i += 3) {
		mesh->normals.push_back(normals[i]);
		mesh->normals.push_back(-normals[i + 2]);
		mesh->normals.push_back(normals[i + 1]);
		mesh->normals.push_back(0.0f);
	}

	//mesh->generateTangentsBinormals(); // Update TBN buffers
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
	auto& blendindices = mesh->skin.indices;
	auto& blendweights = mesh->skin.weights;
	mesh->skin.numWeights = numWeightsPerVtx;

	blendindices.resize(size);
	for (int i = 0; i < size; i++) {
		blendindices.at(i) = indices[i];
	}

	blendweights.resize(size);
	for (int i = 0; i < size; i++) {
		blendweights.at(i) = weights[i];
	}
}

void saveModelToFile(void* pModel, const char* savePath)
{
	// todo: add cereal implementation ...
	auto model = static_cast<CNBAModel*>(pModel);
	if (!model) return;
	
	


	//printf("\n[CSkinModel] Saving model to file: %s\n", savePath);
	//
	//for (auto& mesh : model->getMeshes()) {
	//	printf("\n[CSkinModel] Saving mesh: %s\n", mesh->name.c_str());
	//	printf("\n[CSkinModel] Mesh has %d vertices and %d faces\n\n", mesh->vertices.size() / 3, mesh->triangles.size() / 3);
	//};
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

	// todo: doing a mesh copy is inefficient - need to optimize

	// release mesh
	::freeMesh(mesh);
	return;
}


