/* Stores and extrapolates abstract data from JSON and binary input */

#include <datastream.h>
#include <json.hpp>
#pragma once 

using JSON = nlohmann::json;
class CNBAModel;

enum enPropertyTag {
	FORMAT = 3007389838,
	STREAM = 3522070833,
	BYTE_OFFSET = 2071876544,
	OFFSET = 3348510508,
	SCALE  = 236250733,
	SIZE_  = 2089574848,
	BINARY = 2843576490,
	STRIDE = 3522075280,
};

class CDataBuffer : public CDataStream
{
public:
	CDataBuffer();

public:
	void parse(JSON& json);
	void loadBinary();
	int getStreamIdx();
	std::string getFormat() { return m_format; }

public:
	int getStride();
	void setStride(int val);
	void setOffset(int val);
	std::string id;
	std::vector<float> offset;
	std::vector<float> scale;

private:
	bool decompressGzFile(const std::string& filePath, std::string& targetPath);
	std::string findBinaryFile();

private:
	int m_index;
	std::string m_format;
	std::string m_path;
	int m_size;
};

