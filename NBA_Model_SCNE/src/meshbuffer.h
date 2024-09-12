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
	SIZE   = 2089574848,
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


private:
	int m_index;
	std::string m_format;
	std::string m_path;
	std::vector<float> m_offset;
	std::vector<float> m_scale;

	int m_stride;
	int m_size;
};


