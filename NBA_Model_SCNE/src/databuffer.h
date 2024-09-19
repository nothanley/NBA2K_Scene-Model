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
	bool saveBinary(char* data, const size_t size);
	void loadBinary();

public:
	int getDataOffset();
	int getStride();
	int getStreamIdx();
	void setStride(int val);
	void setOffset(int val);

public:
	std::vector<uint8_t> getBinary(); // returns a copy of the source binary buffer
	std::string getFormat();
	std::string getEncoding();
	std::string getType();

public:
	std::string id;
	std::vector<float> translate;
	std::vector<float> scale;

private:
	void loadFileData(char* src, const size_t& size);
	void readFileData(char*& data, size_t& file_size);

private:
	int m_index;
	std::string m_format;
	int m_size;
};

