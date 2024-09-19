/* Generic 3D structs and handlers */

#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <json.hpp>
#pragma once

using JSON = nlohmann::json;

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

struct StGotoPoint
{
	int16_t no_0;
	int16_t no_1;
	int16_t no_2;
	int16_t no_3;
};


struct Material
{
	std::string name;
	std::string color_map;
	std::string normal_map;
	std::string roughness_map;
};

struct VertexColorSet
{
	std::string name;
	std::vector<float> map;
};

struct BoundingBox
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float radius;
};

struct BlendWeight {
	std::vector<std::string> bones;
	std::vector<float> weights;
};

struct StBlendShape {
	std::string name;
	std::vector<float> vertices;
	std::vector<int>   vtxMorphs;

	Vec3 vertex(const int index) const;
};

struct Skin {
	int numWeights = 0;
	std::vector<float> weights, indices;

	/* Unpacks all weights and indices into a bw struct vector */
	std::vector<BlendWeight>* unpack(
		const std::vector<std::string>& stringTable);
};

struct UVMap {
	std::string name;
	std::vector<float> map;

	Vec2 texcoord(const int index) const;
	float baseU, baseV;
};

struct FaceGroup
{
	Material material;
	int faceBegin;
	int numTriangles;
};

struct Mesh
{
	std::string name;
	std::string definition;
	Material material;
	BoundingBox bounds;

	Skin skin;
	std::vector<float> vertices, normals, tangent_frames;
	std::vector<float> binormals, tangents;
	std::vector<VertexColorSet> colors;
	std::vector<Triangle> triangles;
	std::vector<UVMap> uvs;
	std::vector<StBlendShape> blendshapes;
	std::vector<FaceGroup> groups;

	// NBA Specific attributes
	void* vertex_ref   = NULL;
	void* normals_ref  = NULL;
	void* texcoord_ref = NULL;
	 
	void alignPosition(const bool align_inverse = false, const int num_components = 3); /* Flips all mesh vertices to (x,-z,y) basis */
	void alignNormals (const bool align_inverse = false, const int num_components = 3);

	void flipNormals(); /* Flips all mesh triangle faces inside out. */
	void convertSplitNorms(); /* Re-arrange normals for blender import/interface */
	void translateUVs(const int& index); /* Translates and aligns UV map to Blender/MAX 3D space*/
	void generateAABBs(); /* Generates mesh's axis aligned boundary box */
	Vec3 vertex(const int index) const; /* Extracts position vector of coord array */
	Vec3 normal(const int index) const; /* Extracts normal vector of coord array */
};



