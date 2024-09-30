#include <cereal/mesh_json.h>
#include <meshstructs.h>
#include <bin_codec.h>
#include <datastream.h>
#include <common.h>

void
MeshJSON::AABBsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
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
MeshJSON::primsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& prims)
{
	(*prims)["Material"] = mesh->material.name();
	(*prims)["Mesh"]     = mesh->name.c_str();
	(*prims)["Type"]     = "TRIANGLE_LIST";
	(*prims)["Count"]    = mesh->triangles.size() * 3;

	MeshJSON::AABBsToJson(mesh, prims);
	MeshJSON::dUVsToJson(mesh, prims);
	MeshJSON::skinDataToJson(mesh, prims);
}
 
void
MeshJSON::skinDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	JSON skinObj;
	int min = 0;
	int max = 0;
	mesh->skin.getMinMaxRange(min, max);

	// use unaltered transform
	skinObj[mesh->name.c_str()] = nullptr;
	(*json)["BlendIndexRange"]  = std::vector<int>{ min,max };
}

void VertexStream::fromMesh(const std::shared_ptr<Mesh>& mesh)
{
	this->data = nullptr;

	this->data = (name == "POSITION0")     ? &mesh->vertices : data;
	this->data = (name == "TANGENTFRAME0") ? &mesh->tangent_frames : data;
	this->data = (name == "TEXCOORD0")     ? &mesh->uvs.front().map : data;
}

void
MeshJSON::vertexDataToJson(const char* save_directory, const char* subdir, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	auto streamObj = std::make_shared<JSON>();
	auto formatObj = std::make_shared<JSON>();

	VertexStream position{ "POSITION0","R32G32B32A32", "FLOAT" };
	VertexStream tangent{  "TANGENTFRAME0","R10G10B10A2", "UINT" };
	VertexStream texcoord{ "TEXCOORD0","R16G16", "SNORM" };

	// Populate vertex stream reference
	position.fromMesh(mesh);
	tangent.fromMesh(mesh);
	texcoord.fromMesh(mesh);

	// serialize vertex data
	BinJSON::createVertexBuffer(position, save_directory, subdir, *mesh, *formatObj, *streamObj, 0);
	BinJSON::createVertexBuffer(tangent, save_directory, subdir, *mesh, *formatObj, *streamObj, 1);
	BinJSON::createVertexBuffer(texcoord, save_directory, subdir, *mesh, *formatObj, *streamObj, 2);

	(*json)["VertexFormat"] = *formatObj;
	(*json)["VertexStream"] = *streamObj;
}

void
MeshJSON::indexDataToJson(const char* dirPath, const char* subdir, const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	size_t numVerts = mesh->vertices.size() / 3;
	bool   is32Bit  = (numVerts > _UI16_MAX);

	VertexStream indices{ "INDEX", (is32Bit ? "R32" : "R16"), "UINT" };

	indices.index_data = &mesh->triangles;
	indices.file_name  = BinJSON::generateBufferID(*mesh, "INDEX", 1);
	indices.file_path  = std::string(dirPath) + "/" + std::string(subdir) + indices.file_name;
	BinJSON::writeIndexBuffer(indices);

	// use unaltered transform
	JSON indexBfObj;
	indexBfObj["Format"] = indices.format + "_" + indices.type;
	indexBfObj["Size"]   = indices.size;
	indexBfObj["Binary"] = std::string(subdir) + indices.file_name;

	(*json)["IndexBuffer"] = indexBfObj;
}

