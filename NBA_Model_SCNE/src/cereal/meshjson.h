#include <json.hpp>
#include <memory>
#pragma once

using JSON = nlohmann::json;

// forward declaration
struct Mesh;
struct Vec4;

// namespace
namespace MeshJSON
{
	void AABBsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void dUVsToJson  (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void tfmToJson   (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void primsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& prims);

	void vertexDataToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void indexDataToJson  (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);

	void createVertexBuffer(
		const std::shared_ptr<Mesh>& mesh,
		std::shared_ptr<JSON>& fmtObj,
		std::shared_ptr<JSON>& srmObj,
		const int index,
		const char* format,
		const char* stream_id,
		const Vec4 offset,
		const Vec4 scale);
}

