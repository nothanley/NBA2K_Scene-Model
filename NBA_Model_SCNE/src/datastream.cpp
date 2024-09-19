#include <datastream.h>
#include <bin_codec.h>
#include <common.h>
#include <fstream>
#include <filesystem>
#include <gzip/utils.hpp>
#include <gzip/decompress.hpp>

CDataStream::CDataStream()
	:
	m_offset(NULL),
	m_stride(NULL)
{
}

bool CDataStream::writeDataToFile(const std::string& filePath, const std::string& data)
{
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile) return false;

	// write stream contents to file
	outFile.write(data.c_str(), data.size());

	// check write success...
	if (!outFile) return false;

	outFile.close();
	return true;
}

bool CDataStream::writeDataToFile(const std::string& filePath, const char* data, const size_t size)
{
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile) return false;

	// write stream contents to file
	outFile.write(data, size);

	// check write success...
	if (!outFile) return false;

	outFile.close();
	return true;
}

bool CDataStream::decompressGzFile(const std::string& filePath, std::string& targetPath)
{
	size_t size;
	char* data = common::readFile(filePath, &size);
	if (!data) return false;

	if (gzip::is_compressed(data, size))
	{
		printf("\n[CDataBuffer] Decompressing .gz file...");
		auto decompressed_data = gzip::decompress(data, size);

		common::replaceSubString(targetPath, ".gz", ".bin");
		writeDataToFile(targetPath, decompressed_data);
	}

	// todo: handle already decompressed .gz files ... 
	delete[] data;
	return true;
}

std::string CDataStream::findBinaryFile()
{
	std::string targetName = std::filesystem::path(m_path).filename().string();
	bool isCompressed = common::containsSubstring(m_path, ".gz");

	if (isCompressed)
	{
		auto compressedPath = common::findFileInDirectory(WORKING_DIR, targetName);

		if (!compressedPath.empty())
		{
			std::string outPath = compressedPath;
			return (decompressGzFile(compressedPath, outPath)) ? outPath : "";
		}
		else
		{
			// file not in dir - default search to decompressed .bin file ...
			common::replaceSubString(targetName, ".gz", ".bin");
		}
	}

	return common::findFileInDirectory(WORKING_DIR, targetName);
}

