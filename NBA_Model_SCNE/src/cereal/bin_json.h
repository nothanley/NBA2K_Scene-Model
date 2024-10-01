// Common binary encoder methods for vertex data streams

#include <json.hpp>
#include <memory>
#include <meshstructs.h>
#pragma once

using JSON = nlohmann::ordered_json;

// forward declaration
struct Mesh;
struct Vec4;

struct VertexStream
{
	std::string name;
	std::string format;
	std::string type;
	std::string file_name;
	std::string file_path;
	size_t size = 0;
	size_t stride = 0;

	std::vector<float>* data;
	void* index_data;

	void fromMesh(const std::shared_ptr<Mesh>& mesh);
};

// namespace
namespace BinJSON
{
	std::string generateBufferID(
		const Mesh& mesh,
		const char* stream_id,
		const int enum_type);

	void writeIndexBuffer(
		VertexStream& vs);
	
	void writeVertexBuffer(
		VertexStream& vs);
	
	void createPackedVertexSkinBuffer(
		VertexStream& vertexBuffer,
		const char* dirPath,
		const char* subdir,
		const Mesh& mesh,
		JSON& fmtObj,
		JSON& srmObj,
		const int index);

	void createVertexBuffer(
		VertexStream& vertexBuffer,
		const char* dirPath,
		const char* subdir,
		const Mesh& mesh,
		JSON& fmtObj,
		JSON& srmObj,
		const int index);
}