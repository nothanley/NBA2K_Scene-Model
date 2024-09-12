#include "nbamodel.h"
#include <scenefile.h>
#include <common.h>

CNBAModel::CNBAModel(const char* id)
	:
	m_name(id),
	m_parent(NULL)
{
}


CNBAModel::CNBAModel(const char* id, JSON& data)
	:
	m_name(id),
	m_json(data),
	m_parent(NULL)
{
}


CNBAModel::~CNBAModel()
{
}


void CNBAModel::parse()
{
	printf("\n[CNBAModel] Parsing data...");

	for (JSON::iterator it = m_json.begin(); it != m_json.end(); ++it){
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

	this->loadVertices();
}


void CNBAModel::loadVertices()
{
	auto posBf = this->findDataBuffer("POSITION0");
	auto tanBf = this->findDataBuffer("TANGENTFRAME0");
	auto texBf = this->findDataBuffer("TEXCOORD0");

	if (!posBf || !tanBf || !texBf)
		return;

	m_mesh.vertices = posBf->data;
	m_mesh.normals  = tanBf->data;
	//m_mesh.uvs      = posBf->data;
}

void CNBAModel::loadIndices()
{
	auto triBf = this->findDataBuffer("IndexBuffer");

	if (!triBf || triBf->data.size() % 3 != 0) // eval valid tri list count
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
			// define material groups...
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
			auto vtxBf = getVtxBuffer(index);
			
			if (vtxBf)
			{
				vtxBf->parse(it.value());
				vtxBf->loadBinary();
			}

			index++;
		}
	}
}

void CNBAModel::readIndexBuffer(JSON& obj)
{
	DataBuffer data;
	data.parse(obj);
	data.setStride(2);
	data.loadBinary();

	m_dataBfs.push_back(data);
}

