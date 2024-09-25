#include <cereal/meshjson.h>
#include <meshstructs.h>

void
MeshJSON::AABBsToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	auto& aabb = mesh->bounds;
	auto center = mesh->get_center();
	auto radius = mesh->get_radius();

	(*json)["Radius"] = radius;
	(*json)["Center"] = { center.x, center.y, center.z };
	(*json)["Min"] = { aabb.minX,  aabb.minY,  aabb.minZ };
	(*json)["Max"] = { aabb.maxX,  aabb.maxY,  aabb.maxZ };
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
	(*prims)["Material"] = mesh->material.name.c_str();
	(*prims)["Mesh"] = mesh->name.c_str();
	(*prims)["Type"] = "TRIANGLE_LIST";
	(*prims)["BlendIndexRange"] = { 0, 0 }; // todo: implement blend index range ...
	(*prims)["Count"] = mesh->triangles.size();
	MeshJSON::AABBsToJson(mesh, prims);
	MeshJSON::dUVsToJson(mesh, prims);
}

void
MeshJSON::createVertexBuffer(
	const std::shared_ptr<Mesh>& mesh,
	std::shared_ptr<JSON>& fmtObj,
	std::shared_ptr<JSON>& srmObj,
	const int index,
	const char* format,
	const char* stream_id,
	const Vec4 offset = { 0.0f, 0.0f, 0.0f, 0.0f },
	const Vec4 scale  = { 1.0f, 1.0f, 1.0f, 1.0f })
{
	JSON fmtInfo, stream;
	fmtInfo["Format"] = format;
	fmtInfo["Stream"] = index;
	fmtInfo["Offset"] = { offset.x, offset.y, offset.z, offset.w };
	fmtInfo["Scale"]  = { scale.x, scale.y, scale.z, scale.w };

	stream["Size"] = mesh->vertices.size();
	stream["Binary"] = mesh->name + "_" + std::string(stream_id) + ".bin";
	(*fmtObj)[stream_id] = fmtInfo;
	(*srmObj)["VertexBuffer_" + std::to_string(index)] = stream;
}

void
MeshJSON::vertexDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	auto streamObj = std::make_shared<JSON>();
	auto formatObj = std::make_shared<JSON>();

	MeshJSON::createVertexBuffer(mesh, formatObj, streamObj, 0, "R32G32B32A32_FLOAT", "POSITION0");
	MeshJSON::createVertexBuffer(mesh, formatObj, streamObj, 1, "R10G10B10A2_UINT", "TANGENTFRAME0");
	MeshJSON::createVertexBuffer(mesh, formatObj, streamObj, 2, "R16G16_SNORM", "TEXCOORD0");

	(*json)["VertexFormat"] = *formatObj;
	(*json)["VertexStream"] = *streamObj;
}

void
MeshJSON::indexDataToJson(const std::shared_ptr<Mesh>& mesh, std::shared_ptr<JSON>& json)
{
	JSON indexBfObj;

	size_t numTris = mesh->triangles.size();
	size_t numVerts = mesh->vertices.size() / 3;
	size_t size = (numTris * 3);

	// use unaltered transform
	indexBfObj["Format"] = (numVerts > _UI16_MAX) ? "R32_UINT" : "R16_UINT";
	indexBfObj["Size"] = size * (numVerts > _UI16_MAX ? sizeof(uint32_t) : sizeof(uint16_t));
	indexBfObj["Binary"] = mesh->name + "_IndexBuffer.bin";

	(*json)["IndexBuffer"] = indexBfObj;
}