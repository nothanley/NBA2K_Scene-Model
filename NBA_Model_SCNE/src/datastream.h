#pragma once
#include <string>
#include <vector>

class CDataStream
{
public:
	CDataStream();

public:
	std::vector<float> data; // stores all data eg. pos, uvs, indices
	static bool writeDataToFile(const std::string& filePath, const std::string& data);
	static bool writeDataToFile(const std::string& filePath, const char* data, const size_t size);

protected:
	bool decompressGzFile(const std::string& filePath, std::string& targetPath);
	std::string findBinaryFile();

protected:
	std::string m_path;
	std::string m_binaryPath;
	int m_offset;
	int m_stride;
};



