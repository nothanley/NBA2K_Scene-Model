#include <cereal/meshjson.h>
#include <meshstructs.h>
#include <bin_codec.h>
#include <datastream.h>
#include <common.h>

void
MeshJSON::AABBsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	mesh->generateAABBs();
	auto& aabb  = mesh->bounds;
	auto center = mesh->get_center();
	auto radius = mesh->get_radius();

	(*json)["Radius"] = radius;
	(*json)["Center"] = { center.x, center.y, center.z };
	(*json)["Min"]    = { aabb.minX,  aabb.minY,  aabb.minZ };
	(*json)["Max"]    = { aabb.maxX,  aabb.maxY,  aabb.maxZ };
}

void
MeshJSON::dUVsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	auto dUV0 = { 0.0f, 0.0f };
	auto dUV1 = { 0.0f, 0.0f };

	(*json)["Duv0"] = dUV0;
	(*json)["Duv1"] = dUV1;
}

void
MeshJSON::tfmToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	JSON tfmObj;

	// use unaltered transform
	tfmObj[mesh->name.c_str()] = nullptr;
	(*json)["Transform"] = tfmObj;
}

void 
MeshJSON::skinDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	// todo: implement blend index range ...
	(*json)["BlendIndexRange"] = { 0, 0 };
}

void
MeshJSON::primsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& prims)
{
	(*prims)["Material"] = nullptr;
	(*prims)["Mesh"]     = mesh->name.c_str();
	(*prims)["Type"]     = "TRIANGLE_LIST";
	(*prims)["Count"]    = mesh->triangles.size();
	MeshJSON::skinDataToJson(mesh, prims);
	MeshJSON::AABBsToJson(mesh, prims);
	MeshJSON::dUVsToJson(mesh, prims);
}

void 
MeshJSON::writeVertexBuffer(const std::vector<float>* data, const char* format, const char* type, const char* path)
{
	// create binary codec
	if (!data) return;
	BinaryCodec codec(format, type);
	size_t size;
	
	// encode data
	char* buffer = codec.encode(*data, size);
	CDataStream::writeDataToFile(path, buffer, size);
	
	// cleanup
	delete[] buffer;
}

static inline const std::vector<float>* getMeshData(const std::shared_ptr<Mesh>& mesh, const char* stream_id)
{
	std::vector<float>* data = nullptr;

	data = (stream_id == "POSITION0") ?     &mesh->vertices : data;
	data = (stream_id == "TANGENTFRAME0") ? &mesh->tangent_frames : data;
	data = (stream_id == "TEXCOORD0") ?     &mesh->uvs.front().map : data;

	return data;
}

void
MeshJSON::createVertexBuffer(
	const char* dirPath, const std::shared_ptr<Mesh>& mesh,
	std::shared_ptr<JSON>& fmtObj, std::shared_ptr<JSON>& srmObj,
	const int index, const char* format,
	const char* type, const char* stream_id,
	const Vec4 offset = { 0.0f, 0.0f, 0.0f, 0.0f }, const Vec4 scale  = { 1.0f, 1.0f, 1.0f, 1.0f })
{
	// get mesh data ptr
	auto data = ::getMeshData(mesh, stream_id);
	if (!data) return;

	// Define vertex buffer format
	JSON fmtInfo, stream;
	fmtInfo["Format"] = std::string(format) + "_" + std::string(type);
	fmtInfo["Stream"] = index;
	fmtInfo["Offset"] = { offset.x, offset.y, offset.z, offset.w };
	fmtInfo["Scale"]  = { scale.x, scale.y, scale.z, scale.w };

	// Define vertex buffer stream
	stream["Size"]       = data->size();
	stream["Binary"]     = mesh->name + "_" + std::string(stream_id) + ".bin";
	(*fmtObj)[stream_id] = fmtInfo;
	(*srmObj)["VertexBuffer_" + std::to_string(index)] = stream;

	// write vertex buffer to disk
	std::string savePath = std::string(dirPath) + "/" + mesh->name + "_" + std::string(stream_id) + ".bin";
	MeshJSON::writeVertexBuffer(data, format, type, savePath.c_str());
}

void
MeshJSON::vertexDataToJson(const char* save_directory, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	auto streamObj = std::make_shared<JSON>();
	auto formatObj = std::make_shared<JSON>();

	MeshJSON::createVertexBuffer(save_directory, mesh, formatObj, streamObj, 0, "R32G32B32A32", "FLOAT", "POSITION0");
	MeshJSON::createVertexBuffer(save_directory, mesh, formatObj, streamObj, 1, "R10G10B10A2", "UINT", "TANGENTFRAME0");
	MeshJSON::createVertexBuffer(save_directory, mesh, formatObj, streamObj, 2, "R16G16", "SNORM", "TEXCOORD0");

	(*json)["VertexFormat"] = *formatObj;
	(*json)["VertexStream"] = *streamObj;
}

void
MeshJSON::indexDataToJson(const char* dirPath, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	JSON indexBfObj;

	size_t numTris  = mesh->triangles.size();
	size_t numVerts = mesh->vertices.size() / 3;
	size_t size     = (numTris * 3);
	bool   is32Bit  = (numVerts > _UI16_MAX);

	// use unaltered transform
	indexBfObj["Format"] = (is32Bit) ? "R32_UINT" : "R16_UINT";
	indexBfObj["Size"]   = size * (is32Bit ? sizeof(uint32_t) : sizeof(uint16_t));
	indexBfObj["Binary"] = mesh->name + "_IndexBuffer.bin";

	(*json)["IndexBuffer"] = indexBfObj;

	// write vertex buffer to disk
	std::string savePath = std::string(dirPath) + "/" + mesh->name + "_IndexBuffer.bin";
	MeshJSON::writeIndexBuffer( &mesh->triangles , (is32Bit ? "R32" : "R16") , "UINT" , savePath.c_str() );
}

void
MeshJSON::writeIndexBuffer(const void* data, const char* format, const char* type, const char* path)
{
	// get triangle data
	std::vector<float> indexList;
	std::vector<Triangle>* tris = (std::vector<Triangle>*)data;

	for (auto& tri : *tris)
	{
		indexList.push_back(tri[0]);
		indexList.push_back(tri[1]);
		indexList.push_back(tri[2]);
	}

	// create binary codec
	BinaryCodec codec(format, type);
	size_t size;
	
	// encode data
	char* buffer = codec.encode(indexList, size);
	CDataStream::writeDataToFile(path, buffer, size);
	
	// cleanup
	delete[] buffer;
}

