#include "nbamodel.h"
#include <scenefile.h>
#include <common.h>

CNBAModel::CNBAModel(const char* id)
	:
	m_name(id),
	m_parent(NULL),
	g_mOffset(0.0f),
	g_mScale(1.0f),
	g_dUVscale(Array2D{ 1,1 }),
	g_dUVoffset(Array2D{ 0,0 })
{
}


CNBAModel::CNBAModel(const char* id, JSON& data)
	:
	m_name(id),
	m_json(data),
	m_parent(NULL),
	g_mOffset(0.0f),
	g_mScale(1.0f),
	g_dUVscale(Array2D{ 1,1 }),
	g_dUVoffset(Array2D{ 0,0 })
{
}


CNBAModel::~CNBAModel()
{
}

Mesh* CNBAModel::getMesh()
{
	return &m_mesh;
}

void CNBAModel::parse()
{
	for (JSON::iterator it = m_json.begin(); it != m_json.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
			case enModelData::UDIM_SCALE:
				g_dUVscale = { it.value()[0], it.value()[1] };
				break;
			case enModelData::UDIM_TRANSLATE:
				g_dUVoffset = { it.value()[0], it.value()[1] };
				break;
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

	this->loadIndices();
	this->loadVertices();
}

static void setMeshVtxs(DataBuffer* posBf, Mesh& mesh)
{
	/* Format vertex coord mesh data - ignore every W position coord */
	for (int i = 0; i < posBf->data.size(); i++)
	{
		auto& vtx = posBf->data[i];

		// Apply Scale and Offset transforms -
		float tfm = vtx * posBf->scale[i % 4];
		tfm      += posBf->offset[i % 4];

		if ( !common::containsSubstring(posBf->getFormat(), "R16G16B16A16") )
		{
			mesh.vertices.push_back(tfm);
		}
		else if ((i + 1) % 4 != 0)
		{
			mesh.vertices.push_back(tfm);
		}
	}
}

static void addMeshUVMap(DataBuffer* texBf, Mesh& mesh, Array2D& scale, Array2D& offset)
{
	/* Format uv coord mesh data - ignore every W position coord */
	UVMap channel{ texBf->id };

	for (int i = 0; i < texBf->data.size(); i++)
	{
		auto& coord = texBf->data[i];

		// Apply Scale and Offset transforms -
		float tfm = coord * scale[i % 2] + offset[i % 2];

		channel.map.push_back(tfm);
	}

	mesh.uvs.push_back(channel); 
}

void CNBAModel::loadVertices()
{
	auto posBf = this->findDataBuffer("POSITION0");
	auto tanBf = this->findDataBuffer("TANGENTFRAME0");
	auto texBf = this->findDataBuffer("TEXCOORD0");

	if (!posBf || !tanBf || !texBf)
		return;

	// todo: handle when model has multiple pos,tan,tex etc. datasets ...
	m_mesh.name     = m_name;
	m_mesh.normals  = tanBf->data;
	::setMeshVtxs(posBf, m_mesh);

	if (!texBf->data.empty())
		::addMeshUVMap(texBf, m_mesh, g_dUVscale, g_dUVoffset);

	// debug log ...
	printf("\n[CNBAModel] Built 3D Mesh: \"%s\" | Points: %d | Tris: %d",
		m_mesh.name.c_str(),
		m_mesh.vertices.size() / 3,
		m_mesh.triangles.size() 
	);
}

void CNBAModel::loadIndices()
{
	auto triBf = this->findDataBuffer("IndexBuffer");

	if (!triBf || triBf->data.size() % 3 != 0) // check valid tri list count
		return;

	for (int i = 0; i < triBf->data.size(); i+=3)
	{
		Triangle face
		{
			triBf->data[i],
			triBf->data[i+1],
			triBf->data[i+2]
		};

		m_mesh.triangles.push_back(face);
	}
}

void CNBAModel::readPrim(JSON& obj)
{
	int index = 0;

	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			// todo: define material groups...
			index++;
		}
	}
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

