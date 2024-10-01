#include <cereal/skin_json.h>
#include <cereal/bin_json.h>
#include <meshprimitive.h>
#include <common.h>
#include <bin_codec.h>
#include <datastream.h>

CSkinJsonEncoder::CSkinJsonEncoder(std::shared_ptr<JSON>& parent,  Mesh& mesh,  NSSkeleton& skeleton)
	:
	m_json(parent),
	m_mesh(&mesh),
	m_skeleton(&skeleton),
	m_streamIndex(0)
{
}

void
CSkinJsonEncoder::encode(const char* path, const char* subpath_title)
{
	// Validate mesh and skeleton
	if (!m_mesh || !m_skeleton || m_mesh->skin.blendverts.empty())
		return;
		
	// Write skin vertex streams
	this->m_savePath    = path;
	this->m_subPath     = subpath_title;
	this->m_streamIndex = getVertexStreamIndex();

	if (m_savePath.empty() || !common::create_folder(m_savePath + "/" + m_subPath))
		return;

	// Encode and generate vertex data
	this->createMatrixBuffer();
	this->writeWeightData();
}

int
CSkinJsonEncoder::getVertexStreamIndex()
{
	int size = 0;
	JSON obj = (*m_json)["VertexStream"];

	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
			size++;
	}

	return size;
}

inline static bool hasMatchingSkin(const MatrixPalette& in, const MatrixPalette& other)
{
	if (in.groups.size() > other.groups.size())
		return false;

	for (int k = 0; k < in.groups.size(); k++)
		if (in.groups[k] != other.groups[k])
			return false;

	return true;
}

inline static int findMatrixIndex(std::vector<MatrixPalette>& table, const MatrixPalette& target)
{
	// Ignore single weighted vertices
	if (target.groups.size() <= 1)
		return -1;

	// Scan table for target skin index/weight pairs
	for (int i = 0; i < table.size(); i++)
	{
		// Check if matrix element fits our target skin group
		if (::hasMatchingSkin(target, table[i]))
			return i;
	}
	
	// skin group not found - push and return last index
	table.push_back(target);
	return (table.size() - 1);
}

inline static MatrixPalette getVertexPalette(const BlendVertex& vertex)
{
	MatrixPalette matrix;

	int num_weights = vertex.indices.size();
	matrix.groups.resize(num_weights);

	for (int j = 0; j < num_weights; j++)
		matrix.groups[j] = MatrixGroup{ vertex.indices[j], vertex.weights[j] };

	return matrix;
}

inline static void packVertexCoordsAndWeights(Mesh& mesh, VertexStream& position, std::vector<float>& weights)
{
	// Pack encoded weights into W component of position stream
	int numVerts = weights.size();
	for (int i = 0; i < numVerts; i++)
	{
		mesh.vertices[(i * 4) + 3] = weights[i];
	}

	position.data = &mesh.vertices;
}

inline static void encodeVertexWeights(std::vector<float>& data, 
	std::vector<VertexWeightLink>& links, std::vector <MatrixPalette>& table, Mesh& mesh)
{
	for (int i = 0; i < links.size(); i++)
	{
		auto& blend = mesh.skin.blendverts[i];
		auto encoded = links[i].pack(table, blend);
		data.push_back(*reinterpret_cast<float*>(&encoded));
	}
}

void
CSkinJsonEncoder::writeWeightData()
{
	// Define JSON entries
	JSON fmt = (*m_json)["VertexFormat"];
	JSON srm = (*m_json)["VertexStream"];
	VertexStream position{ "POSITION0","R32G32B32A32", "FLOAT" };

	// Get packed vertex weight stream ...
	std::vector<float> data;
	::encodeVertexWeights(data, m_vertexLinks, m_matrixTable, *m_mesh);

	// Pack position and weight data ...
	::packVertexCoordsAndWeights(*m_mesh, position, data);
	BinJSON::createPackedVertexSkinBuffer(position, m_savePath.c_str(), m_subPath.c_str(), *m_mesh, fmt, srm, 0);

	(*m_json)["VertexFormat"] = fmt;
	(*m_json)["VertexStream"] = srm;
}

inline static uint16_t packWeight(float value)
{
	// Convert to 16-bit unsigned integer range (0 to 65535)
	return static_cast<uint16_t>(value * 65535.0f);
};

void
CSkinJsonEncoder::packMatrices(std::vector<float>& data)
{
	int offset = 0;

	for (int i = 0; i < m_matrixTable.size(); i++)
	{
		auto& matrixEle = m_matrixTable[i];
		matrixEle.offset = offset;

		for (int k = 0; k < matrixEle.groups.size(); k++)
		{
			auto& pair = matrixEle.groups[k];

			uint16_t weight  = ::packWeight(pair.weight);
			uint32_t encoded = (pair.boneIndex << 0x10) | weight;
			data.push_back(*reinterpret_cast<float*>(&encoded)); 
		}

		offset += matrixEle.groups.size();
	}
}

void
CSkinJsonEncoder::writeMatrixData()
{
	std::vector<float> mtxData;

	VertexStream vs{ "MatrixWeightsBuffer", "R32", "UINT"};
	vs.file_name = BinJSON::generateBufferID(*m_mesh, vs.name.c_str(), 2);
	vs.file_path = m_savePath + "/" + m_subPath + vs.file_name;
	this->packMatrices(mtxData);

	BinaryCodec codec(vs.format, vs.type);
	char* buffer = codec.encode(mtxData, vs.size);
	CDataStream::writeDataToFile(vs.file_path, buffer, vs.size);

	// store json
	JSON obj;
	obj["Format"]    = vs.format + "_" + vs.type;
	obj["Dimension"] = "BYTEADDRESSBUFFER";
	obj["Size"]      = vs.size;
	obj["Binary"]    = m_subPath + vs.file_name;

	// cleanup
	delete[] buffer;
	(*m_json)["MatrixWeightsBuffer"] = obj;
}

void
CSkinJsonEncoder::createMatrixBuffer()
{
	// Resize vectors
	int numVerts = m_mesh->skin.blendverts.size();
	m_vertexLinks.resize(numVerts);

	// Iterate skin vertices
	for (int i = 0; i < numVerts; i++)
	{
		auto& blend = m_mesh->skin.blendverts[i];
		auto& link  = m_vertexLinks[i];

		MatrixPalette matrix = ::getVertexPalette(blend);
		link.pair_index      = ::findMatrixIndex(m_matrixTable, matrix);
	}

	this->writeMatrixData();
}

uint32_t
VertexWeightLink::pack(const std::vector<MatrixPalette>& table, const BlendVertex& vertex) const
{
	int numWeights = std::max(int(vertex.indices.size() - 1), 0);

	int index = (pair_index == -1)
		?
		/* Interpret index as bone index */ ((vertex.indices.empty()) ? 0 : vertex.indices.front())
		:
		/* Interpret index as buffer index */ pair_index;

	
	if (pair_index != -1)
	{
		index = table[index].offset;
	}

	return (index << 0x8) | numWeights; 
}


