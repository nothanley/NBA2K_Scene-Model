#include <databuffer.h>
#include <nbamodel.h>
#include <scenefile.h>
#include <common.h>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <bin_codec.h>

CDataBuffer::CDataBuffer()
	:
	CDataStream(),
	m_index(NULL),
	m_size(NULL)
{
}

int CDataBuffer::getStreamIdx()
{
	return m_index;
}

void CDataBuffer::setStride(int val)
{
	m_stride = val;
}

void CDataBuffer::setOffset(int val)
{
	m_offset = val;
}

std::string CDataBuffer::getEncoding()
{
	return common::splitString(m_format, '_').front();
};

std::string CDataBuffer::getType()
{
	auto type = common::splitString(m_format, '_').back();
	::common::str_to_lower(type);
	return type;
};

std::string CDataBuffer::getFormat() {
	return m_format;
}

int CDataBuffer::getStride()
{
	if (m_stride > 0 || m_format.empty())
		return m_stride;

	// Manually calculate stride length if non available
	auto encoding = getEncoding();
	auto type     = getType();

	BinaryCodec codec(encoding,type);
	m_stride = codec.size(1);
	return m_stride;
}

int CDataBuffer::getDataOffset()
{
	return m_offset;
}

void CDataBuffer::parse(JSON& json)
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
				translate = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
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

void CDataBuffer::readFileData(char*& data, size_t& file_size)
{
	m_binaryPath = this->findBinaryFile();
	data         = common::readFile(m_binaryPath, &file_size);

	/* Missing model data - must throw exception */
	if (m_binaryPath.empty() || !data) {
		printf("\n[CDataBuffer] Invalid scene - inaccessible data file: %s\n", m_path.c_str());
		throw std::runtime_error("Invalid data buffer.");
	}
}

void CDataBuffer::loadFileData(char* src, const size_t& size)
{
	std::string encoding = getEncoding();
	std::string type     = getType();

	// Validate memory buffer size with target length
	BinaryCodec codec(encoding, type);
	size_t items = m_size / getStride();
	size_t dataSize = codec.size(items);

	// load data elemenets from binary
	if (dataSize <= size)
		codec.decode(src, items, data, m_offset, m_stride);
};

void CDataBuffer::loadBinary()
{
	if (m_format.empty() || !m_size || m_path.empty())
		return;

	size_t size(NULL);
	char* binary;

	// Process file binary
	this->readFileData(binary, size);
	this->loadFileData(binary, size);

	// free binary
	delete[] binary;
}

std::vector<uint8_t>
CDataBuffer::getBinary()
{
	std::vector<uint8_t> data;
	size_t size(NULL);
	char* binary;

	if (m_format.empty() || !m_size || m_path.empty())
		return data;

	// Process file binary
	this->readFileData(binary, size);

	// Copy data to target vector
	data.resize(size);
	memcpy(data.data(), binary, size);

	// free allocated binary
	delete[] binary;

	return data;
}

bool CDataBuffer::saveBinary(char* data, const size_t size)
{
	if (!data || m_binaryPath.empty() || m_path.empty())
		return false;
	
	common::createFileBackup(m_binaryPath.c_str());

	return writeDataToFile(m_binaryPath, data, size);;
}


