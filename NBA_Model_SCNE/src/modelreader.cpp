#include <modelreader.h>
#include <scenefile.h>
#include <common.h>
#include <armature/bone_reader.h>

CModelReader::CModelReader(const char* id, JSON& data)
	:
	CNBAModel(id),
	m_json(data),
	m_parent(NULL)
{
}

CModelReader::~CModelReader()
{
}

void CModelReader::parse()
{
	for (JSON::iterator it = m_json.begin(); it != m_json.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
		case enModelData::MORPH:
			readMorphs(it.value());
			break;
		case enModelData::WEIGHTBITS:
			m_weightBits = it.value();
			break;
		case enModelData::TRANSFORM:
			readTfms(it.value());
			break;
		case enModelData::PRIM:
			readPrim(it.value());
			break;
		case enModelData::INDEXBUFFER:
			readIndexBuffer(it.value());
			break;
		case enModelData::MATRIXWEIGHTBUFFER:
			readMtxWeightBuffer(it.value());
			break;
		case enModelData::VERTEXFORMAT:
			readVertexFmt(it.value());
			break;
		case enModelData::VERTEXSTREAM:
			readVertexStream(it.value());
			break;
		case enPrimTag::PM_DUV_0:
			g_uvDeriv.push_back(it.value());
			break;
		case enPrimTag::PM_DUV_1:
			g_uvDeriv.push_back(it.value());
			break;
		case enPrimTag::PM_DUV_2:
			g_uvDeriv.push_back(it.value());
			break;
		default:
			break;
		};
	}

	this->loadMeshData();
}

inline static void trisFromMeshGroup(std::shared_ptr<Mesh>& fullMesh, std::shared_ptr<Mesh>& splitMsh, const FaceGroup& group)
{
	splitMsh->triangles.clear();

	// update index list
	const int numTris = fullMesh->triangles.size();
	size_t indexBegin = (group.begin) / 3;
	size_t indexEnd   = (group.begin + group.count) / 3;

	for (size_t i = indexBegin; i < indexEnd; i++)
		if (i <= numTris)
		{
			Triangle& tri = fullMesh->triangles[i];
			splitMsh->triangles.push_back(tri);
		}
};

void CModelReader::splitMeshGroups()
{
	if (m_meshes.empty())
		return;

	// split mesh using groups ...
	auto fullMesh = m_meshes.front();
	for (auto& group : fullMesh->groups)
	{
		auto splitMsh = std::make_shared<Mesh>(*fullMesh);
		::trisFromMeshGroup(fullMesh, splitMsh, group);

		splitMsh->name = group.name;
		m_meshes.push_back(splitMsh);
	}

	m_meshes.erase(m_meshes.begin());
}

void CModelReader::loadMeshData()
{
	if (m_dataBfs.empty() || m_vtxBfs.empty() || m_primitives.empty())
		return;

	this->loadMesh();
	//this->splitMeshGroups();

	m_primitives.clear();
}

void CModelReader::loadMesh()
{
	// build full model mesh
	uintptr_t dataOffset = NULL;
	int  beginIdx        = 0;
	auto mesh            = std::make_shared<Mesh>();

	for (auto& prim : m_primitives)
	{
		FaceGroup group;

		dataOffset  = (prim.data_begin < 0) ? dataOffset : prim.data_begin;
		group.name  = prim.name;
		group.begin = beginIdx;
		group.count = prim.count;
		group.material.setName(prim.material_name.c_str());
		mesh->groups.push_back(group);

		loadIndices(*mesh, prim.count, dataOffset);
		beginIdx += prim.count;
	}

	loadVertices(*mesh);
	loadWeights(*mesh);
	m_meshes.push_back(mesh);
}

void CModelReader::loadVertices(Mesh& mesh)
{
	auto posBf = findDataBuffer("POSITION0");
	auto tanBf = findDataBuffer("TANGENTFRAME0");
	auto texBf = findDataBuffer("TEXCOORD0");

	if (!posBf || !tanBf || !texBf)
		return;

	// Build mesh geometry - unpack data
	mesh.name = (mesh.name.empty()) ? m_name : mesh.name;
	GeomDef::setMeshVtxs(posBf, mesh);
	GeomDef::calculateVtxNormals(tanBf, mesh);

	if (!texBf->data.empty()) GeomDef::addMeshUVMap(texBf, mesh);

	// Update mesh refs
	mesh.vertex_ref   = posBf;
	mesh.normals_ref  = tanBf;
	mesh.texcoord_ref = texBf;

	// System logs ...
	if (USE_DEBUG_LOGS)
		printf
		(	"\n[CModelReader] Built 3D Mesh: \"%s\" | Points: %d | Tris: %d",
			mesh.name.c_str(),
			mesh.vertices.size() / 3,
			mesh.triangles.size()
		);
}

void CModelReader::loadIndices(Mesh& mesh, const int count, uintptr_t& offset)
{
	auto triBf = findDataBuffer("IndexBuffer");
	int end    = count + offset;

	if (!triBf || end > triBf->data.size() || count % 3 != 0)
		return;

	for (int i = offset; i < end; i += 3)
	{
		Triangle face
		{
			triBf->data[i],
			triBf->data[i + 1],
			triBf->data[i + 2]
		};

		mesh.triangles.push_back(face);
	}

	offset += count;
}

void CModelReader::readVertexFmt(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			CDataBuffer data;
			data.id = it.key();
			data.parse(it.value());
			m_vtxBfs.push_back(data);
		}
	}
}

CDataBuffer* CModelReader::getVtxBuffer(int index)
{
	if (index > m_vtxBfs.size())
		return nullptr;


	for (auto& vtxBf : m_vtxBfs)
	{
		if (vtxBf.getStreamIdx() == index)
			return &vtxBf;
	}

	return nullptr;
}

CDataBuffer* CModelReader::findDataBuffer(const char* target)
{
	for (auto& dataBf : m_dataBfs)
	{
		if (dataBf.id == target)
			return &dataBf;
	}

	for (auto& vtxBf : m_vtxBfs)
	{
		if (vtxBf.id == target)
			return &vtxBf;
	}

	return nullptr;
}

void CModelReader::readVertexStream(JSON& obj)
{
	int index = 0;

	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			for (auto& vtxBf : m_vtxBfs)
			{
				// Vertex buffers can sometimes share a stream binary
				if (vtxBf.getStreamIdx() == index)
				{
					vtxBf.parse(it.value());
					vtxBf.loadBinary();
				}
			}

			index++;
		}
	}
}

void CModelReader::readMtxWeightBuffer(JSON& obj)
{
	// find weight data stream
	CDataBuffer data;
	data.parse(obj);
	data.loadBinary();
	data.id = "MatrixWeightBuffer";

	m_dataBfs.push_back(data);
};


void CModelReader::readIndexBuffer(JSON& obj)
{
	CDataBuffer data;
	data.parse(obj);
	data.loadBinary();
	data.id = "IndexBuffer";

	m_dataBfs.push_back(data);
}

void CModelReader::readPrim(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			StGeoPrim grp{ m_name };
			grp.load(it.value());

			// default to global transform if non specified
			grp.uv_deriv = (grp.uv_deriv.empty()) ? g_uvDeriv : grp.uv_deriv;

			// push lods
			GeomDef::pushPrimLods(grp, m_primitives);
		}
	}
}


void CModelReader::readTfms(JSON& obj)
{
	// Load bone data
	CBoneReader::fromJSON(obj, m_skeleton);
}


inline static float unpackWeight(uint16_t packedWeight) {
    return static_cast<float>(packedWeight) / 65535.0f;
}

inline static void loadPackedWeights(const uint32_t& index, const std::vector<float>& mtxData, const int num_weights, BlendVertex& skinVtx)
{
	uint32_t encodedValue, blendIdx, skinVal;
	skinVtx.weights.resize(num_weights);
	skinVtx.indices.resize(num_weights);

	for (int i = 0; i < num_weights; i++)
	{
		encodedValue = mtxData[index + i];
		blendIdx = encodedValue >> 0x10;
		skinVal  = encodedValue & 0xFFFF;

		skinVtx.indices[i] = blendIdx;
		skinVtx.weights[i] = ::unpackWeight(skinVal);
	};
}

inline static void loadMatrixBufferWeights(
	Mesh& mesh, const size_t& numVerts, const size_t& numElems, const CDataBuffer* weightBf, const CDataBuffer* matrixBf)
{
	// Load vertex weights + indices
	uint32_t packedVtxSkin, numWeights, index;
	mesh.skin.blendverts.resize(numVerts);

	for (size_t i = 0; i < numVerts; i++)
	{
		auto& skinVtx = mesh.skin.blendverts[i];
		packedVtxSkin = weightBf->data[i];
		numWeights    = packedVtxSkin & 0xFF;
		index         = packedVtxSkin >> 0x8;

		if (numWeights == 0)
		{   // load default weight
			skinVtx.weights.push_back(1.0f);
			skinVtx.indices.push_back(index);
		}
		else { // unpack matrix buffer weights
			::loadPackedWeights(index, matrixBf->data, numWeights+1, skinVtx);
		}
	};
}

void CModelReader::loadWeights(Mesh& mesh)
{
	// Retrieve weight buffers
	auto weightBf = findDataBuffer("WEIGHTDATA0");
	auto matrixBf = findDataBuffer("MatrixWeightBuffer");
	if (!weightBf || !matrixBf)
		return;	

	// Get num total elements
	size_t numBfElems = matrixBf->data.size();
	size_t numVerts   = mesh.vertices.size() / 3;
	if (numVerts > weightBf->data.size())
		return;

	// load vertex skin
	::loadMatrixBufferWeights(mesh, numVerts, numBfElems, weightBf, matrixBf);
	mesh.skin.updateIndices(&m_skeleton);
}



void CModelReader::readMorphs(JSON& obj)
{
	// debug morph id's - collects and prints all debug morphs
	return;
	
	std::vector<std::string> debugMorphs;
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		debugMorphs.push_back(it.key());
	}

	printf("\ndebugMorphs = {");
	for (auto& morph : debugMorphs)
	{
		printf("\n\"%s\",", morph.c_str());
	}

	printf("};");
}
