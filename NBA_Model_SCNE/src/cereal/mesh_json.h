#include <cereal/bin_json.h>
#pragma once

// namespace
namespace MeshJSON
{
	void skinDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void AABBsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void dUVsToJson  (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void tfmToJson   (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
	void primsToJson (const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& prims);

	void vertexDataToJson (
		const char* save_directory, const char* subdirectory, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);

	void indexDataToJson  (
		const char* save_directory, const char* subdirectory, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json);
}

