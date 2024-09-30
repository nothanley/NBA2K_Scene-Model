#include <meshstructs.h>
#pragma once


// Skin Matrix Encoder structs
struct MatrixGroup
{
	int boneIndex;
	float weight;

	bool operator==(const MatrixGroup& other) const 
	{
		return (this->boneIndex == other.boneIndex) && (this->weight == other.weight);
	}

	bool operator!=(const MatrixGroup& other) const
	{
		return (this->boneIndex != other.boneIndex) || (this->weight != other.weight);
	}
};

struct MatrixPalette
{
	std::vector<MatrixGroup> groups;
	uint32_t offset;
};

struct VertexWeightLink
{
	int pair_index = -1;
	uint32_t pack(const std::vector<MatrixPalette>& table, const BlendVertex& vertex) const;
};



// Binary encoder for skin data
class CSkinJsonEncoder
{
public:
	CSkinJsonEncoder(std::shared_ptr<JSON>& parent, Mesh& mesh, NSSkeleton& skeleton);

public:
	void encode(const char* path, const char* subpath_title);

private:
	void createMatrixBuffer();
	void writeWeightData();
	void writeMatrixData();
	void packMatrices(std::vector<float>& data);
	int getVertexStreamIndex();

private:
	std::vector<MatrixPalette> m_matrixTable;
	std::vector<VertexWeightLink> m_vertexLinks;

	// members
	Mesh* m_mesh;
	std::string m_subPath;
	std::string m_savePath;
	std::shared_ptr<JSON> m_json;
	NSSkeleton* m_skeleton;
	int m_streamIndex;
};
