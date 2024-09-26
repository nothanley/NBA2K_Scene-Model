#include <json.hpp>
#include <memory>
#pragma once

using JSON = nlohmann::ordered_json;

// forward declaration
struct Mesh;
struct Vec4;

// namespace
namespace MeshJSON
{
	void skinDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void AABBsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void dUVsToJson  (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void tfmToJson   (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void primsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& prims);

	void vertexDataToJson (const char* save_directory, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void indexDataToJson  (const char* save_directory, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);

	void writeIndexBuffer(const void* data, const char* format, const char* type, const char* path, size_t& size);
	void writeVertexBuffer(const std::vector<float>* data, const char* format, const char* type, const char* path, size_t& size, size_t& stride);
	void createVertexBuffer(
		const char* dirPath,
		const std::shared_ptr<Mesh>& mesh,
		std::shared_ptr<JSON>& fmtObj,
		std::shared_ptr<JSON>& srmObj,
		const int index,
		const char* format,
		const char* type,
		const char* stream_id,
		const Vec4 offset,
		const Vec4 scale);
}

