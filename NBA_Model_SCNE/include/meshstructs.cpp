#include "meshstructs.h"
#include <databuffer.h>

void Mesh::generateAABBs()
{
	/* Validate coordinate vector */
	const auto& coords = this->vertices;
	if (coords.size() % 3 == 0) return;

	// Create default bounding box
	BoundingBox& box = this->bounds;
	box = BoundingBox{ /* Max Coords */ coords[0], coords[1], coords[2],
		/* Min Coords */ coords[0], coords[1], coords[2] };

	// Iterate and find highest/lowest position coord
	int numCoords = coords.size();
	for (int i = 0; i < numCoords; i += 3)
	{
		Vec3 vertex{ coords[i], coords[i + 1], coords[i + 2] };

		box.maxX = (vertex.x > box.maxX) ? vertex.x : box.maxX;
		box.maxY = (vertex.y > box.maxY) ? vertex.y : box.maxY;
		box.maxZ = (vertex.z > box.maxZ) ? vertex.z : box.maxZ;

		box.minX = (vertex.x < box.minX) ? vertex.x : box.minX;
		box.minY = (vertex.y < box.minY) ? vertex.y : box.minY;
		box.minZ = (vertex.z < box.minZ) ? vertex.z : box.minZ;
	}
}


void Mesh::flipNormals()
{
	for (auto& tri : triangles)
		tri = Triangle{ tri[1], tri[0], tri[2] };
}

void Mesh::convertSplitNorms()
{
	int numVerts = vertices.size() / 3; // Assumes 'R32G32B32' format 
	int array_size = (normals.size() == vertices.size()) ? 3 : 4;
	std::vector<float> data;

	for (int i = 0; i < numVerts; i++)
	{
		int index = (i * array_size);
		data.push_back(normals.at(index + 0));
		data.push_back(normals.at(index + 1));
		data.push_back(normals.at(index + 2));
	}

	this->normals = data;
}

void Mesh::translateUVs(const int& index)
{
	if (index > uvs.size())
		return;

	auto& map = this->uvs.at(index).map;
	/* Flip Y axis and translate up by one unit */
	for (int i = 0; i < map.size(); i += 2) 
	{
		map[i + 1] = -(map[i + 1] - 1.0f);
	}
}

inline void
LoadVertexSkin(const Skin* skin, BlendWeight& skinVertex,
	const std::vector<std::string>& stringTable, int& begin, const int& numWeights)
{
	/* Iterate through all specified weights for current vertex */
	for (int j = 0; j < numWeights; j++)
	{
		int index = skin->indices[begin];
		float influence = skin->weights[begin];

		std::string boneName = stringTable.at(index);
		skinVertex.bones.push_back(boneName);
		skinVertex.weights.push_back(influence);

		begin++; // Update skin pointer to next index
	}
}

std::vector<BlendWeight>*
Skin::unpack(const std::vector<std::string>& stringTable)
{
	int numVerts = this->weights.size() / numWeights;
	std::vector<BlendWeight>* skinData = new std::vector<BlendWeight>;
	skinData->resize(numVerts);

	/* Iterate through all skin vertices */
	for (int i = 0; i < numVerts; i++)
	{
		BlendWeight& skinVtx = skinData->at(i);
		int skinPtr = (i * numWeights);
		LoadVertexSkin(this, skinVtx, stringTable, skinPtr, numWeights);
	}

	return skinData;
}

Vec2 UVMap::texcoord(const int index) const
{
	int offset = (index * 2);
	return Vec2{ map[offset],  map[offset + 1]};
}

Vec3 Mesh::vertex(const int index) const 
{
	int offset = (index * 3);
	return Vec3{ vertices[offset],  vertices[offset + 1],  vertices[offset + 2] };
}

Vec3 StBlendShape::vertex(const int index) const 
{
	int offset = (index * 3);
	return Vec3{ vertices[offset],  vertices[offset + 1],  vertices[offset + 2] };
}

Vec3 Mesh::normal(const int index) const
{
	int numVerts    = vertices.size() / 3; // Assumes 'R32G32B32' format 
	int array_width = normals.size() / numVerts;
	int offset = (index * array_width);
	return Vec3{ normals[offset],  normals[offset + 1],  normals[offset + 2] };
}

void Mesh::alignPosition(const bool use_blender_scale, const int num_components)
{
	auto size = vertices.size();

	for (int i = 0; i < size; i += num_components)
	{
		auto x = vertices[i];
		auto y = vertices[i + 1];
		auto z = vertices[i + 2];

		if (use_blender_scale)
		{
			vertices[i]     = x;
			vertices[i + 1] = z;
			vertices[i + 2] = -y;
		}
		else {
			vertices[i]     = x;
			vertices[i + 1] = -z;
			vertices[i + 2] = y;
		}
	}
}

void Mesh::alignNormals(const bool use_blender_scale, const int num_components)
{
	auto size = normals.size();

	for (int i = 0; i < size; i += num_components)
	{
		auto x = normals[i];
		auto y = normals[i + 1];
		auto z = normals[i + 2];

		if (use_blender_scale)
		{
			normals[i]     =   x;
			normals[i + 1] =   z;
			normals[i + 2] =   y;
		}
		else {
			normals[i]     = x;
			normals[i + 1] = -z;
			normals[i + 2] = y;
		}
	}
}

Vec3 Mesh::get_center() const
{
	auto& aabb = this->bounds;

	// Calculate center
	float centerX = (aabb.minX + aabb.maxX) / 2.0f;
	float centerY = (aabb.minY + aabb.maxY) / 2.0f;
	float centerZ = (aabb.minZ + aabb.maxZ) / 2.0f;

	return { centerX, centerY, centerZ };
}

float Mesh::get_radius() const
{
	auto& aabb = this->bounds;

	// Calculate radius (half-diagonal length)
	float radiusX = (aabb.maxX - aabb.minX) / 2.0f;
	float radiusY = (aabb.maxY - aabb.minY) / 2.0f;
	float radiusZ = (aabb.maxZ - aabb.minZ) / 2.0f;
	float radius = std::sqrt(radiusX * radiusX + radiusY * radiusY + radiusZ * radiusZ);

	return radius;
}

void Mesh::createWAxis(std::vector<float>& data)
{
	int numItems = data.size() / 3;

	// Create W-axis coordinates
	std::vector<float> new_data;
	new_data.resize(numItems * 4);

	// Copy existing data and add W-axis
	for (int i = 0; i < numItems; i++)
	{
		int index = i * 3;
		int coord = i * 4;

		new_data[coord]     = data[index];
		new_data[coord + 1] = data[index + 1];
		new_data[coord + 2] = data[index + 2];
		new_data[coord + 3] = 1.0f;
	}

	data = new_data;
};

void Mesh::removeWAxis(std::vector<float>& data)
{
	int numItems = data.size() / 4;

	// Create W-axis coordinates
	std::vector<float> new_data;
	new_data.resize(numItems * 3);

	// Copy existing data and remove W-axis
	for (int i = 0; i < numItems; i++)
	{
		int index = i * 3;

		new_data[index]     = data[index];
		new_data[index + 1] = data[index + 1];
		new_data[index + 2] = data[index + 2];
	}

	data = new_data;
}

void MeshCalc::updateTangentFrameVec(const std::vector<float>& tanFrames, const std::vector<float>& normals, std::vector<float>& dst)
{
	// Validate mesh vectors
	int numTans = tanFrames.size() / 4;
	int numNrms = normals.size() / 3;
	if (numTans != numNrms)
		throw std::runtime_error("Unsupported tangent frame encoding. Failed to update.");

	// Encode octahedral normals...
	for (int i = 0; i < numTans; i++)
	{
		Vec4 encoded{
			tanFrames[(i * 4)],
			tanFrames[(i * 4) + 1],
			tanFrames[(i * 4) + 2],
			tanFrames[(i * 4) + 3]
		};

		Vec3 normal{ normals[(i * 3)],
			normals[(i * 3) + 1],
			normals[(i * 3) + 2] };

		MeshCalc::EncodeOctahedralNormal(&encoded, normal);

		dst.push_back(encoded.x);
		dst.push_back(encoded.y);
		dst.push_back(encoded.z);
		dst.push_back(encoded.w);
	}
}

void MeshCalc::buildTangentFrameVec(const Mesh& mesh, std::vector<float>& dst)
{
	// Get mesh data
	auto& normals = mesh.normals;

	// Validate mesh vectors
	if (normals.empty()) return;
	int numNrms = normals.size() / 3;
	dst.resize(numNrms * 4);

	// Encode octahedral normals...
	for (int i = 0; i < numNrms; i++)
	{
		Vec4 encoded{ 0,0,0,0 };
		Vec3 normal{
			normals[(i * 3)],
			normals[(i * 3) + 1],
			normals[(i * 3) + 2] };

		MeshCalc::EncodeOctahedralNormal(&encoded, normal);

		dst[(i*4)+0] = (encoded.x);
		dst[(i*4)+1] = (encoded.y);
		dst[(i*4)+2] = (encoded.z);
		dst[(i*4)+3] = (encoded.w);
	}
}


void MeshCalc::transformVertices(const CDataBuffer* vertexBuffer, std::vector<float>& verts, const int numChannels)
{
	auto& tfmScale = vertexBuffer->scale;
	auto& tfmOffset = vertexBuffer->translate;

	// Perform object transforms
	for (int i = 0; i < verts.size(); i++)
	{
		auto& vtx = verts[i];

		if (!tfmOffset.empty() && !tfmScale.empty())
		{
			// Apply Scale and Offset
			auto& scale = tfmScale[i % numChannels];
			auto& offset = tfmOffset[i % numChannels];

			vtx = (vtx - offset) / scale;
		}
	}
}

void MeshCalc::EncodeOctahedralNormal(Vec4* Encoded, const Vec3& Normal)
{
	float s = 1.0f / (fabsf(Normal.x) + fabsf(Normal.y) + fabsf(Normal.z));
	float nx = Normal.x * s;
	float ny = Normal.y * s;
	float nz = Normal.z * s;
	if (nz >= 0.0) {
		float octWrapX = (1.0f - fabsf(ny)) * (nx >= 0.0f ? 1.0f : -1.0f);
		float octWrapY = (1.0f - fabsf(nx)) * (ny >= 0.0f ? 1.0f : -1.0f);
		nx = octWrapX;
		ny = octWrapY;
	}
	Encoded->x = int((nx * 0.5f + 0.5f) * 1023.0f);
	Encoded->y = int((ny * 0.5f + 0.5f) * 1023.0f);
}

Vec3 MeshCalc::decodeOctahedralNormal(const Vec3& encoded)
{
	Vec3 normal;

	normal.x = (encoded.x * (1.0f / 1023.0f)) * 2.0f - 1.0f;
	normal.y = (encoded.y * (1.0f / 1023.0f)) * 2.0f - 1.0f;
	normal.z = 1.0f - fabsf(normal.x) - fabsf(normal.y);

	float t = fmaxf(-normal.z, 0.0f);
	normal.x += (normal.x >= 0.0f) ? -t : t;
	normal.y += (normal.y >= 0.0f) ? -t : t;
	normal.normalize();

	return normal;
};




