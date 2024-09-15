#include "nbamodel.h"
#include <scenefile.h>
#include <common.h>

static int gPrimOffset = 0;
bool INCLUDE_LODS = /* default LOD setting */ false;

CNBAModel::CNBAModel(const char* id)
	:
	m_name(id),
	m_parent(NULL)
	//g_mOffset(0.0f),
	//g_mScale(1.0f)
{
}


CNBAModel::CNBAModel(const char* id, JSON& data)
	:
	m_name(id),
	m_json(data),
	m_parent(NULL)
	//g_mOffset(0.0f),
	//g_mScale(1.0f),
{
}


CNBAModel::~CNBAModel()
{
}

int CNBAModel::getNumMeshes()
{
	return m_meshes.size();
}

Mesh* CNBAModel::getMesh(int index)
{
	if (m_meshes.empty() || index > m_meshes.size())
		return nullptr;

	return &m_meshes[index];
}

void CNBAModel::parse()
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

void CNBAModel::loadMeshData()
{
	if (m_dataBfs.empty() || m_vtxBfs.empty())
		return;

	for (auto& group : m_groups)
	{
		gPrimOffset = (group.begin > 0) ? group.begin : gPrimOffset;
		this->loadMesh(group);
	}

	gPrimOffset = NULL;
}

void CNBAModel::loadMesh(StGeoPrim& prim)
{
	Mesh mesh;
	int numTris = prim.count;

	mesh.definition = m_name;
	mesh.name       = prim.name;
	loadIndices(mesh, numTris);
	loadVertices(mesh);

	m_meshes.push_back(mesh);
	prim.mesh = &m_meshes.back();
	prim.applyUVDisp();
}

void CNBAModel::loadVertices(Mesh& mesh)
{
	auto posBf = findDataBuffer("POSITION0");
	auto tanBf = findDataBuffer("TANGENTFRAME0");
	auto texBf = findDataBuffer("TEXCOORD0");

	if (!posBf || !tanBf || !texBf)
		return;

	mesh.name     = (mesh.name.empty()) ? m_name : mesh.name;
	mesh.normals  = tanBf->data;
	GeomDef::setMeshVtxs(posBf, mesh);

	if (!texBf->data.empty())
		GeomDef::addMeshUVMap(texBf, mesh);

	// debug log ...
	printf("\n[CNBAModel] Built 3D Mesh: \"%s\" | Points: %d | Tris: %d",
		mesh.name.c_str(),
		mesh.vertices.size() / 3,
		mesh.triangles.size() 
	);
}

void CNBAModel::loadIndices(Mesh& mesh, const int count)
{
	auto triBf = findDataBuffer("IndexBuffer");
	int end    = count + gPrimOffset;

	if (!triBf || end > triBf->data.size() || count % 3 != 0)
		return;

	for (int i = gPrimOffset; i < end; i+=3)
	{
		Triangle face
		{
			triBf->data[i],
			triBf->data[i+1],
			triBf->data[i+2]
		};

		mesh.triangles.push_back(face);
	}

	gPrimOffset += count;
}

void CNBAModel::readVertexFmt(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			DataBuffer data;
			data.id = it.key();
			data.parse( it.value() );
			m_vtxBfs.push_back(data);
		}
	}
}

DataBuffer* CNBAModel::getVtxBuffer(int index)
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

DataBuffer* CNBAModel::findDataBuffer(const char* target)
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

void CNBAModel::readVertexStream(JSON& obj)
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

void CNBAModel::readIndexBuffer(JSON& obj)
{
	DataBuffer data;
	data.parse(obj);
	data.loadBinary();

	m_dataBfs.push_back(data);
}

void StGeoLOD::parse(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
		case enPrimTag::PM_START:
			start = it.value();
			break;
		case enPrimTag::PM_COUNT:
			count = it.value();
			break;
		default:
			break;
		};
	}
}
 
void CNBAModel::readPrim(JSON& obj)
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

void StGeoPrim::applyUVDisp()
{
	if (!mesh || mesh->uvs.empty())
		return;

	for (auto& channel : mesh->uvs)
	{
		for (int i = 0; i < channel.map.size(); i++)
		{
			auto& texcoord = channel.map[i];
			texcoord = (texcoord * .5) + .5;
			texcoord = (i % 2 == 0) ? (-texcoord) + 1.0f : texcoord;
		}
	}
}

void StGeoPrim::load(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
		case enPrimTag::PM_MATERIAL:
			material_name = it.value();
			break;		
		case enPrimTag::PM_MESH:
			name = it.value();
			break;		
		case enPrimTag::PM_TYPE:
			type = it.value();
			break;		
		case enPrimTag::PM_START:
			begin = it.value();
			break;		
		case enPrimTag::PM_COUNT:
			count = it.value(); // (it.value() * UINT16_MAX ) / 3
			break;		
		case enPrimTag::PM_DUV_0:
			uv_deriv.push_back( it.value() );
			break;		
		case enPrimTag::PM_DUV_1:
			uv_deriv.push_back(it.value());
			break;		
		case enPrimTag::PM_DUV_2:
			uv_deriv.push_back(it.value());
			break;
		case enPrimTag::PM_LODLIST:
			for (auto& child : it.value())
			{	// Get prim LOD data from JSON array
				StGeoLOD geo;
				geo.parse(child);
				this->lods.push_back(geo);
			}
			break;
		default:
			break;
		};
	}
}

void GeomDef::pushPrimLods(StGeoPrim& prim, std::vector<StGeoPrim>& prim_vec)
{
	if (prim.lods.empty())
	{
		// No lods - only push main prim geo
		prim_vec.push_back(prim);
		return;
	}

	int num_lods = (INCLUDE_LODS) ? prim.lods.size() : 1;

	for (int i = 0; i < num_lods; i++)
	{
		auto& lod = prim.lods[i];

		// Inherit source geometry from primitive but alter triangle list
		StGeoPrim newPrim = prim;
		newPrim.begin = lod.start;
		newPrim.count = lod.count;

		// Format and push to scene.
		newPrim.name += "_LOD" + std::to_string(i);
		prim_vec.push_back(newPrim);
	}
}

void GeomDef::setMeshVtxs(DataBuffer* posBf, Mesh& mesh)
{
	/* Format vertex coord mesh data - ignore every W position coord */
	for (int i = 0; i < posBf->data.size(); i++)
	{
		// Apply Scale and Offset transforms -
		auto& vtx = posBf->data[i];
		float tfm = vtx;

		if (!common::containsSubstring(posBf->getFormat(), "R16G16B16A16"))
		{
			mesh.vertices.push_back(tfm);
		}
		else if ((i + 1) % 4 != 0)
		{
			mesh.vertices.push_back(tfm);
		}
	}
}

void GeomDef::addMeshUVMap(DataBuffer* texBf, Mesh& mesh)
{
	/* Format uv coord mesh data - ignore every W position coord */
	UVMap channel{ texBf->id };

	for (int i = 0; i < texBf->data.size(); i++)
	{
		// Apply Scale and Offset transforms -
		auto& coord = texBf->data[i];
		float tfm = coord;

		channel.map.push_back(tfm);
	}

	mesh.uvs.push_back(channel);
}

