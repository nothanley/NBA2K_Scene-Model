#include <cereal/bin_json.h>
#include <datastream.h>
#include <bin_codec.h>
#include <common.h>

std::string BinJSON::generateBufferID(const Mesh& mesh, const char* stream_id, const int enum_type)
{
	std::string id = mesh.name + stream_id;
	std::string type;

	switch(enum_type)
	{
		case 0: type = "VertexBuffer"; break;
		case 1: type = "IndexBuffer"; break;
		case 2: type = "MatrixWeightsBuffer"; break;
	}

	return type + "." + common::get_u64_hash_str(id) + ".bin";
}

void
BinJSON::writeVertexBuffer(
	VertexStream& vs)
{
	// create binary codec
	if (!vs.data) return;
	BinaryCodec codec(vs.format, vs.type);

	// encode data
	vs.stride = codec.size(1);
	char* buffer = codec.encode(*vs.data, vs.size);
	CDataStream::writeDataToFile(vs.file_path, buffer, vs.size);

	// cleanup
	delete[] buffer;
}

void
BinJSON::createVertexBuffer
(
	VertexStream& vs,
	const char* dirPath,
	const char* subdir,
	const Mesh& mesh,
	JSON& fmtObj, 
	JSON& srmObj,
	const int index,
	const Vec4 offset, 
	const Vec4 scale
)
{
	// validate data
	if (!vs.data) return;

	// write vertex buffer to disk
	vs.file_name = generateBufferID(mesh, vs.name.c_str(), 0);
	vs.file_path = std::string(dirPath) + "/" + std::string(subdir) + vs.file_name;
	BinJSON::writeVertexBuffer(vs);

	// Define vertex buffer format
	JSON fmtInfo, stream;
	fmtInfo["Format"] = vs.format + "_" + vs.type;
	fmtInfo["Stream"] = index;
	fmtInfo["Offset"] = { offset.x, offset.y, offset.z, offset.w };
	fmtInfo["Scale"]  = { scale.x, scale.y, scale.z, scale.w };

	// Define vertex buffer stream
	stream["Stride"] = vs.stride;
	stream["Size"]   = vs.size;
	stream["Binary"] = std::string(subdir) + vs.file_name;
	fmtObj[vs.name]  = fmtInfo;
	srmObj["VertexBuffer_" + std::to_string(index)] = stream;
}

void
BinJSON::writeIndexBuffer(VertexStream& vs)
{
	// get triangle data
	std::vector<float> indexList;
	std::vector<Triangle>* tris = (std::vector<Triangle>*)vs.index_data;

	for (auto& tri : *tris)
	{
		indexList.push_back(tri[0]);
		indexList.push_back(tri[1]);
		indexList.push_back(tri[2]);
	}

	// encode data
	BinaryCodec codec(vs.format, vs.type);
	char* buffer = codec.encode(indexList, vs.size);
	CDataStream::writeDataToFile(vs.file_path, buffer, vs.size);

	// cleanup
	delete[] buffer;
}

