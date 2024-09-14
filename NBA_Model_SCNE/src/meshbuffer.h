/* Stores and extrapolates abstract data from JSON and binary input */

#include <string>
#include <vector>
#include <json.hpp>
#pragma once 

using JSON = nlohmann::json;
class CNBAModel;

enum enPropertyTag {
	FORMAT = 3007389838,
	STREAM = 3522070833,
	OFFSET = 3348510508,
	SCALE  = 236250733,
	SIZE_  = 2089574848,
	BINARY = 2843576490,
	STRIDE = 3522075280,
};

class DataBuffer 
{
public:
	DataBuffer();

public:
	void parse(JSON& json);
	void loadBinary();
	int getStreamIdx();
	std::string getFormat() { return m_format; }

public:
	int getStride();
	void setStride(int val);
	std::string id;
	std::vector<float> data; // stores all data eg. pos, uvs, indices
	std::vector<float> offset;
	std::vector<float> scale ;

private:
	bool decompressGzFile(const std::string& filePath, std::string& targetPath);
	std::string findBinaryFile();

private:
	int m_index;
	std::string m_format;
	std::string m_path;
	int m_stride;
	int m_size;
};


enum {
	POSITION = 1656822844,
	NORMALS = 2865655510,
	BINORMALS = 2552595965,
	TANGENTS = 2555205862,
	BLENDWEIGHTS = 3362741239,
	BLENDINDICES = 2622613563,
	TEXCOORDS = 680036153,
	COLOR = 229197112,

	R16_G16_B16_A16 = 123583891,
	R10_G10_B10_A2  = 775460992,
	R32_G32_B32 = 1171538707,
	R8_G8_B8_A8 = 3503152076,
	R32     = 193360022,
	R32_G32 = 1382734287,
	R16_G16 = 3666862864,
	R16 = 193355856,
	R8 = 5859695,
};

namespace DatUnpack
{
	char* roundPointerToNearest4(char* ptr);

	constexpr unsigned int hash(const char* s, int off = 0);

	int roundUp(int numToRound, int multiple);

	float bitFloat(float value, std::string signValue);

	float shortFloat(float value, std::string signValue);

	void getDataSet(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	size_t getDataLen(int items, std::string blockType);

	void getData_R32_G32_B32(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R8_G8_B8_A8(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R16_G16_B16_A16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R10_G10_B10_A2(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R32_G32(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R16_G16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R8(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);

	void getData_R32(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet);
}
