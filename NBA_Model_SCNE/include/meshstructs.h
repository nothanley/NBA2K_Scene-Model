/* Generic 3D structs and handlers */

#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <json.hpp>
#include <material/material.h>
#pragma once

using JSON = nlohmann::ordered_json;

typedef std::array<uint32_t, 3> Triangle;
typedef std::array<float, 2> Array2D;

#undef max
#undef min

struct Vec3 {
	float x, y, z;

	void pack_values(float ceiling) {
		x = (x > ceiling) ? ceiling : x;
		y = (x > ceiling) ? ceiling : y;
		z = (x > ceiling) ? ceiling : z;

		float floor = -ceiling;
		x = (x < floor) ? floor : x;
		y = (x < floor) ? floor : y;
		z = (x < floor) ? floor : z;
	}

	void operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;
	}

	void operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;
	}

	void operator+=(const Vec3& value) {
		x += value.x;
		y += value.y;
		z += value.z;
	}

	Vec3 operator*(float value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vec3 operator*(float s) const
	{
		Vec3 v = *this;
		return v * s;
	}

	Vec3 operator+(const Vec3& other) const {
		return Vec3{ x + other.x, y + other.y, z + other.z };
	}

	Vec3 operator-(const Vec3& other) const {
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	bool operator==(const Vec3& other) const {
		return (x == other.x) && (y == other.y) && (z == other.z);
	}

	bool operator!=(const Vec3& other) const {
		return (x != other.x) || (y != other.y) || (z != other.z);
	}

	static void min(Vec3& result, const Vec3& a, const Vec3& b)
	{
		result.x = std::isnan(a.x) || std::isnan(b.x) ? result.x : std::min(a.x, b.x);
		result.y = std::isnan(a.y) || std::isnan(b.y) ? result.y : std::min(a.y, b.y);
		result.z = std::isnan(a.z) || std::isnan(b.z) ? result.z : std::min(a.z, b.z);
	};

	static void max(Vec3& result, const Vec3& a, const Vec3& b)
	{
		result.x = std::isnan(a.x) || std::isnan(b.x) ? result.x : std::max(a.x, b.x);
		result.y = std::isnan(a.y) || std::isnan(b.y) ? result.y : std::max(a.y, b.y);
		result.z = std::isnan(a.z) || std::isnan(b.z) ? result.z : std::max(a.z, b.z);
	};

	void handleNaN(float value = 0.00001f) {
		if (std::isnan(x)) x = value;
		if (std::isnan(y)) y = value;
		if (std::isnan(z)) z = value;
	}

	void normalize() {
		float length = std::sqrt(x * x + y * y + z * z);
		if (length != 0.0f) {
			x /= length;
			y /= length;
			z /= length;
		}
	}

	bool null() const {
		return x == 0 && y == 0 && z == 0;
	}

	static Vec3 cross(const Vec3& a, const Vec3& b){
		return  Vec3{ a.y * b.z - a.z * b.y, 
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x };
	}

	static float dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};

struct Vec2 {
	float x, y;

	Vec2 operator-(const Vec2& other) const {
		return Vec2{ x - other.x, y - other.y};
	}

	operator Vec3() const
	{
		return Vec3{ x, y, 0.0f };
	}
};

struct Vec4 {
	float x, y, z, w;
};

struct Matrix3 {
	Vec3 x, y, z;
};

struct Matrix4x3 {
	Vec3 x, y, z, w;
};

struct Matrix4 {
	Vec4 x, y, z, w;
};

struct Material
{
	std::string name;
	std::string color_map;
	std::string normal_map;
	std::string roughness_map;
};

struct BoundingBox
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float radius;
};

struct BlendVertex 
{
	int num_weights = 0;
	std::vector<int> indices;
	std::vector<std::string> bones;
	std::vector<float> weights;
};

struct BlendShape {
	std::string name;
	std::vector<float> vertices;
	std::vector<int>   vtxMorphs;

	Vec3 vertex(const int index) const;
};

struct NSSkeleton;

struct Skin 
{
	std::vector<BlendVertex> blendverts;
	void updateIndices(const NSSkeleton* skeleton);
};

struct UVMap {
	std::string name;
	std::vector<float> map;

	Vec2 texcoord(const int index) const;
	float baseU, baseV;
};

struct FaceGroup 
{
	std::string name;
	CNSMaterial material;
	int begin;
	int count;
};

// Game mesh struct - todo: make this a full class
struct Mesh
{
	std::string name;
	std::string definition;
	BoundingBox bounds;

	Skin skin;
	CNSMaterial material;
	std::vector<float> vertices, tangent_frames;
	std::vector<float> normals, binormals, tangents;
	std::vector<Triangle> triangles;
	std::vector<UVMap> uvs;
	std::vector<FaceGroup> groups;

	// NBA Specific attributes
	void* vertex_ref   = NULL;
	void* normals_ref  = NULL;
	void* texcoord_ref = NULL;
	 
	static void createWAxis(std::vector<float>& data); // used for 2k meshes
	static void removeWAxis(std::vector<float>& data); // used for 2k meshes

	void alignPosition(const bool align_inverse = false, const int num_components = 3); /* Flips all mesh vertices to (x,-z,y) basis */
	void alignNormals (const bool align_inverse = false, const int num_components = 3);
	void flipNormals(); /* Flips all mesh triangle faces inside out. */
	void convertSplitNorms(); /* Re-arrange normals for blender import/interface */
	void translateUVs(const int& index); /* Translates and aligns UV map to Blender/MAX 3D space*/
	void generateAABBs(); /* Generates mesh's axis aligned boundary box */
	Vec3 vertex(const int index) const; /* Extracts position vector of coord array */
	Vec3 normal(const int index) const; /* Extracts normal vector of coord array */
	Vec3 get_center() const;
	float get_radius() const;
};


// Forward declarations
class CDataBuffer;

namespace MeshCalc
{
	void buildTangentFrameVec(const Mesh& mesh, std::vector<float>& dst);
	void updateTangentFrameVec(const std::vector<float>& tanFrames, const std::vector<float>& normals, std::vector<float>& dst);
	void transformVertices(const CDataBuffer* vertexBuffer, std::vector<float>& verts, const int numChannels);
	void EncodeOctahedralNormal(Vec4* Encoded, const Vec3& Normal);
	Vec3 decodeOctahedralNormal(const Vec3& encoded);
	void setFlatTangentBinormals(Mesh& mesh);
	void calculateTangentsBinormals(Mesh& mesh, const bool use_tangents=true);
}

