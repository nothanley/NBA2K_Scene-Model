#include <modelreader.h>
#include <scenefile.h>
#include <common.h>

static int gPrimOffset = 0;

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
		case enModelData::PRIM:
			readPrim(it.value());
			break;
		case enModelData::INDEXBUFFER:
			readIndexBuffer(it.value());
			m_dataBfs.back().id = it.key();
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

void CModelReader::mergeMeshGroups()
{
	// Inherit base mesh from primitive - merge all data
	auto src = *m_groups.front().mesh.get();
	auto mesh = std::make_shared<Mesh>(src);

	// Update mesh info
	mesh->name = this->m_name;
	mesh->definition = m_name;
	mesh->triangles.clear();

	for (auto& group : m_groups)
	{
		// Merge Faces
		for (auto& triangle : group.mesh->triangles)
			mesh->triangles.push_back(triangle);
	}

	// update scene collection
	m_meshes.clear();
	m_meshes.push_back(mesh);
}

void CModelReader::createSkinMeshes()
{
	for (auto& group : m_groups)
	{
		// build split meshes using material group data
		gPrimOffset = (group.begin > 0) ? group.begin : gPrimOffset;
		loadMesh(group);
	}

	gPrimOffset = NULL;
}

void CModelReader::loadMeshData()
{
	if (m_dataBfs.empty() || m_vtxBfs.empty() || m_groups.empty())
		return;

	createSkinMeshes();

	if (MERGE_MESH_PRIMS)
		this->mergeMeshGroups();

	m_groups.clear();
}

void CModelReader::loadMesh(StGeoPrim& prim)
{
	prim.mesh = std::make_shared<Mesh>();
	auto& mesh = *prim.mesh;

	int index = m_meshes.size();
	int numTris = prim.count;
	mesh.definition = m_name;
	mesh.name = prim.name;

	loadIndices(mesh, numTris);
	loadVertices(mesh);
	m_meshes.push_back(prim.mesh);
}

void CModelReader::loadVertices(Mesh& mesh)
{
	auto posBf = findDataBuffer("POSITION0");
	auto tanBf = findDataBuffer("TANGENTFRAME0");
	auto texBf = findDataBuffer("TEXCOORD0");

	if (!posBf || !tanBf || !texBf)
		return;

	mesh.name = (mesh.name.empty()) ? m_name : mesh.name;
	GeomDef::setMeshVtxs(posBf, mesh);
	GeomDef::calculateVtxNormals(tanBf, mesh);
	if (!texBf->data.empty())
		GeomDef::addMeshUVMap(texBf, mesh);

	// debug log ...
	printf("\n[CModelReader] Built 3D Mesh: \"%s\" | Points: %d | Tris: %d",
		mesh.name.c_str(),
		mesh.vertices.size() / 3,
		mesh.triangles.size()
	);
}

void CModelReader::loadIndices(Mesh& mesh, const int count)
{
	auto triBf = findDataBuffer("IndexBuffer");
	int end = count + gPrimOffset;

	if (!triBf || end > triBf->data.size() || count % 3 != 0)
		return;

	for (int i = gPrimOffset; i < end; i += 3)
	{
		Triangle face
		{
			triBf->data[i],
			triBf->data[i + 1],
			triBf->data[i + 2]
		};

		mesh.triangles.push_back(face);
	}

	gPrimOffset += count;
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

void CModelReader::readIndexBuffer(JSON& obj)
{
	CDataBuffer data;
	data.parse(obj);
	data.loadBinary();

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
			GeomDef::pushPrimLods(grp, m_groups);
		}
	}
}


