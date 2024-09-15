#include <meshbuffer.h>
#include <nbamodel.h>
#include <scenefile.h>
#include <common.h>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

DataBuffer::DataBuffer()
	:
	DataStream(),
	m_index(NULL),
	m_size(NULL)
{
}

void DataBuffer::loadBinary()
{
	if (m_format.empty() || !m_size || m_path.empty())
		return;

	/* Load file data into memory */
	size_t fileLen         = NULL;
	std::string binaryPath = this->findBinaryFile();
	char* fileBf           = common::readFile(binaryPath, &fileLen);
	if (binaryPath.empty())
		printf("\n[DataBuffer] Invalid scene - missing model data file: %s\n", m_path.c_str());

	/* Missing model data so must throw exception */
	if (binaryPath.empty() || !fileBf)
		throw std::runtime_error("");

	/* Discern target data types */
	auto type = common::splitString(m_format, '_').front();
	auto pack = common::splitString(m_format, '_').back();
	::common::str_to_lower(pack);

	// Validate memory buffer size with target length
	char* dataSrc   = fileBf;
	size_t items    = m_size / getStride();
	size_t dataSize = getDataLen(items, type);

	// load data elemenets from binary
	if (dataSize <= fileLen)
		decodeBuffer(dataSrc, items, pack, type, this->data);

	// free binary
	delete[] fileBf;
}

int DataBuffer::getStreamIdx()
{
	return m_index;
}

void DataBuffer::setStride(int val)
{
	m_stride = val;
}

void DataBuffer::setOffset(int val)
{
	m_offset = val;
}

int DataBuffer::getStride()
{
	if (m_stride > 0 || m_format.empty())
		return m_stride;

	// Manually calculate stride length if non available
	auto type = common::splitString(m_format, '_').front();
	m_stride  = getDataLen(1, type);

	return m_stride;
}

void DataBuffer::parse(JSON& json)
{
	for (JSON::iterator it = json.begin(); it != json.end(); ++it)
	{
		auto key = common::chash(it.key());
		auto value = it.value();

		switch (key)
		{
			case enPropertyTag::FORMAT:
				m_format = value;
				break;
			case enPropertyTag::STREAM:
				m_index  = value;
				break;
			case enPropertyTag::OFFSET:
				offset = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SCALE:
				scale  = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SIZE_:
				m_size = value;
				break;
			case enPropertyTag::BINARY:
				m_path = value;
				break;
			case enPropertyTag::BYTE_OFFSET:
				this->setOffset(value);
				break;
			case enPropertyTag::STRIDE:
				this->setStride(value);
				break;
			default:
				break;
		};
	}
}



bool writeDataToFile(const std::string& filePath, const std::string& data)
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

bool DataBuffer::decompressGzFile(const std::string& filePath, std::string& targetPath)
{
	size_t size;
	char* data = common::readFile(filePath, &size);
	if (!data) return false;

	if (gzip::is_compressed(data, size))
	{
		printf("\n[DataBuffer] Decompressing .gz file...");
		auto decompressed_data = gzip::decompress(data, size);
		
		common::replaceSubString(targetPath, ".gz", ".bin");
		::writeDataToFile(targetPath, decompressed_data);
	}

	// todo: handle already decompressed .gz files ... 
	delete[] data;
	return true;
}

std::string DataBuffer::findBinaryFile()
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
