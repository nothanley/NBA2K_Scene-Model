#include <meshbuffer.h>
#include <nbamodel.h>
#include <scenefile.h>
#include <common.h>
#include <memoryreader.h>

using namespace memreader;

DataBuffer::DataBuffer()
	:
	m_index(NULL),
	m_stride(NULL)
{
}

void DataBuffer::loadBinary()
{
	if (m_format.empty() || !m_size || m_path.empty())
		return;

	/* Perform decompression if nessecary*/
	// for now just search for decompressed binaries.
	common::replaceSubString(m_path, ".gz", ".bin");

	/* Load file data into memory */
	size_t fileLen         = NULL;
	std::string binaryPath = common::format_path(WORKING_DIR + "/" + m_path);
	char* fileBf           = common::readFile(binaryPath, &fileLen);
	if (!fileBf) return;

	/* Discern target data types */
	auto type = common::splitString(m_format, '_').front();
	auto pack = common::splitString(m_format, '_').back();
	::common::str_to_lower(pack);

	// Validate memory buffer size with target length
	char* dataSrc   = fileBf;
	size_t items    = m_size / getStride();
	size_t dataSize = DatUnpack::getDataLen(items, type);

	// load data elemenets from binary
	if (dataSize <= fileLen)
		DatUnpack::getDataSet(dataSrc, items, pack, type, this->data);

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

int DataBuffer::getStride()
{
	if (m_stride > 0 || m_format.empty())
		return m_stride;

	// Manually calculate stride length if non available
	auto type        = common::splitString(m_format, '_').front();
	m_stride = DatUnpack::getDataLen(1, type);

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
			case enPropertyTag::STRIDE:
				m_stride = value;
				break;
			default:
				break;
		};
	}
}

char*
DatUnpack::roundPointerToNearest4(char* ptr)
{
	std::ptrdiff_t diff = ptr - reinterpret_cast<char*>(0); // Get the pointer difference from nullptr
	std::ptrdiff_t roundedDiff = (diff + 3) & ~3; // Round up to the nearest multiple of 4
	char* roundedPtr = reinterpret_cast<char*>(roundedDiff); // Convert the rounded difference back to a pointer
	return roundedPtr;
}

constexpr unsigned int
DatUnpack::hash(const char* s, int off) {
	return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}

int
DatUnpack::roundUp(int numToRound, int multiple) {
	int remainder;

	if (multiple == 0)
		return numToRound;
	remainder = numToRound % multiple;
	if (remainder == 0)
		return numToRound;

	return (numToRound + multiple - remainder);
}

float
DatUnpack::bitFloat(float value, std::string type) {

	if (type == "snorm")
		value = float(value / 127.0);
	else if (type == "unorm")
		value = float(value / 255.0);
	return value;
}

float
DatUnpack::shortFloat(float value, std::string type) {
	if (type == "snorm")
		value = float(value / 32767.0);
	else if (type == "unorm")
		value = float(value / 65535.0);
	return value;
}

static constexpr uint64_t getMaxIntValue(int bits, bool sign)
{
	return sign ? (1LL << (bits - 1)) - 1 : (1ULL << bits) - 1;
}

static float decodeVariableLenFloat(float value, int bit_len, std::string type) 
{
	if (type == "snorm")
		value = float(value / ::getMaxIntValue(bit_len,true));
	else if (type == "unorm")
		value = float(value / ::getMaxIntValue(bit_len, false));

	return value;
}


void
DatUnpack::getDataSet(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	unsigned int key = hash(blockType.c_str());

	switch (key)
	{
		case R16_G16_B16_A16:
			getData_R16_G16_B16_A16(data, size, dataType, blockType, dataSet);
			break;
		case R10_G10_B10_A2:
			getData_R10_G10_B10_A2(data, size, dataType, blockType, dataSet);
			break;
		case R32_G32_B32:
			getData_R32_G32_B32(data, size, dataType, blockType, dataSet);
			break;
		case R8_G8_B8_A8:
			getData_R8_G8_B8_A8(data, size, dataType, blockType, dataSet);
			break;
		case R32_G32:
			getData_R32_G32(data, size, dataType, blockType, dataSet);
			break;
		case R16_G16:
			getData_R16_G16(data, size, dataType, blockType, dataSet);
			break;
		case R16:
			getData_R16(data, size, dataType, blockType, dataSet);
			break;
		case R8:
			getData_R8(data, size, dataType, blockType, dataSet);
			break;
		default:
			break;
	}

}

void
DatUnpack::getData_R32_G32_B32(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < size; i++) {
		float r32 = ReadFloat(data);
		float g32 = ReadFloat(data);
		float b32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
		dataSet.push_back(b32);
	}
}

void
DatUnpack::getData_R8_G8_B8_A8(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < size; i++) {
		float r8f, g8f, b8f, a8f;

		if (dataType == "snorm") {
			int8_t r8 = ReadInt8(data);
			int8_t g8 = ReadInt8(data);
			int8_t b8 = ReadInt8(data);
			int8_t a8 = ReadInt8(data);

			r8f = bitFloat(r8, dataType);
			g8f = bitFloat(g8, dataType);
			b8f = bitFloat(b8, dataType);
			a8f = bitFloat(a8, dataType);
		}
		else if (dataType == "unorm") {
			uint8_t r8 = ReadUInt8(data);
			uint8_t g8 = ReadUInt8(data);
			uint8_t b8 = ReadUInt8(data);
			uint8_t a8 = ReadUInt8(data);

			r8f = bitFloat(r8, dataType);
			g8f = bitFloat(g8, dataType);
			b8f = bitFloat(b8, dataType);
			a8f = bitFloat(a8, dataType);
		}
		else if (dataType == "sint") {
			int8_t r8 = ReadInt8(data);
			int8_t g8 = ReadInt8(data);
			int8_t b8 = ReadInt8(data);
			int8_t a8 = ReadInt8(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}
		else if (dataType == "uint") {
			uint8_t r8 = ReadUInt8(data);
			uint8_t g8 = ReadUInt8(data);
			uint8_t b8 = ReadUInt8(data);
			uint8_t a8 = ReadUInt8(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}


		dataSet.push_back(r8f);
		dataSet.push_back(g8f);
		dataSet.push_back(b8f);
		dataSet.push_back(a8f);
	}
}

static void convertPackVal(float& input, int num_bits, std::string type)
{
	if (type == "snorm" || type == "unorm") {
		input = decodeVariableLenFloat(input, num_bits, type);
	}
	else if (type == "sint") {
		// ...
	}
	else if (type == "uint") {
		// ...
	}
}

void
DatUnpack::getData_R10_G10_B10_A2(char*& data, int size, std::string type, std::string encodeFmt, std::vector<float>& dataSet)
{
	float r, g, b, a;
	uint32_t packedValue;

	for (int i = 0; i < size; i++) 
	{
		packedValue = ReadUInt32(data); // todo: check for bitswap requirements...

		r = (packedValue >> 22) & 0x3FF;
		g = (packedValue >> 12) & 0x3FF;
		b = (packedValue >> 2) & 0x3FF;
		a =  packedValue & 0x3;

		::convertPackVal(r, 10, type);
		::convertPackVal(g, 10, type);
		::convertPackVal(b, 10, type);
		::convertPackVal(a, 02, type);

		dataSet.push_back(r);
		dataSet.push_back(g);
		dataSet.push_back(b);
		dataSet.push_back(a);
	}
}

void
DatUnpack::getData_R16_G16_B16_A16(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < size; i++)
	{
		float r8f, g8f, b8f, a8f;

		if (dataType == "snorm") {
			int16_t r8 = ReadInt16(data);
			int16_t g8 = ReadInt16(data);
			int16_t b8 = ReadInt16(data);
			int16_t a8 = ReadInt16(data);

			r8f = shortFloat(r8, dataType);
			g8f = shortFloat(g8, dataType);
			b8f = shortFloat(b8, dataType);
			a8f = shortFloat(a8, dataType);
		}
		else if (dataType == "unorm") {
			uint16_t r8 = ReadUInt16(data);
			uint16_t g8 = ReadUInt16(data);
			uint16_t b8 = ReadUInt16(data);
			uint16_t a8 = ReadUInt16(data);

			r8f = shortFloat(r8, dataType);
			g8f = shortFloat(g8, dataType);
			b8f = shortFloat(b8, dataType);
			a8f = shortFloat(a8, dataType);
		}
		else if (dataType == "sint") {
			int16_t r8 = ReadInt16(data);
			int16_t g8 = ReadInt16(data);
			int16_t b8 = ReadInt16(data);
			int16_t a8 = ReadInt16(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}
		else if (dataType == "uint") {
			uint16_t r8 = ReadUInt16(data);
			uint16_t g8 = ReadUInt16(data);
			uint16_t b8 = ReadUInt16(data);
			uint16_t a8 = ReadUInt16(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}


		dataSet.push_back(r8f);
		dataSet.push_back(g8f);
		dataSet.push_back(b8f);
		dataSet.push_back(a8f);
	}
}

void
DatUnpack::getData_R32_G32(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < size; i++) 
	{
		float r32 = ReadFloat(data);
		float g32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
	}
}

void
DatUnpack::getData_R16_G16(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet) 
{
	size_t len = size * 2;

	for (size_t i = 0; i < len; i++) 
	{
		float result;

		if (dataType == "snorm") {
			auto val = ReadInt16(data);
			result = bitFloat(val, dataType);
		}
		else if (dataType == "unorm") {
			auto val = ReadUInt16(data);
			result = bitFloat(val, dataType);
		}
		else if (dataType == "sint") {
			auto val = ReadInt16(data);
			result = val;
		}
		else if (dataType == "uint") {
			auto val = ReadUInt16(data);
			result = val;
		}

		dataSet.push_back(result);
	}
}

void
DatUnpack::getData_R16(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	for (int i = 0; i < size; i++) 
	{
		float result;

		if (dataType == "snorm") {
			auto val = ReadInt16(data);
			result = bitFloat(val, dataType);
		}
		else if (dataType == "unorm") {
			auto val = ReadUInt16(data);
			result = bitFloat(val, dataType);
		}
		else if (dataType == "sint") {
			auto val = ReadInt16(data);
			result = val;
		}
		else if (dataType == "uint") {
			auto val = ReadUInt16(data);
			result = val;
		}

		dataSet.push_back(result);
	}
}

void
DatUnpack::getData_R8(char*& data, int size, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	for (int i = 0; i < size; i++) {
		float r8f;

		if (dataType == "snorm") {
			int8_t r8 = ReadInt8(data);

			r8f = bitFloat(r8, dataType);
		}
		else if (dataType == "unorm") {
			uint8_t r8 = ReadUInt8(data);

			r8f = bitFloat(r8, dataType);
		}
		else if (dataType == "sint") {
			int8_t r8 = ReadInt8(data);

			r8f = r8;
		}
		else if (dataType == "uint") {
			uint8_t r8 = ReadUInt8(data);

			r8f = r8;
		}

		dataSet.push_back(r8f);
	}
}


size_t DatUnpack::getDataLen(int items, std::string blockType)
{
	size_t size = items;
	unsigned int buf = hash(blockType.c_str());

	switch (buf)
	{
		case R10_G10_B10_A2:
			size *= 4;
			break;
		case R16_G16_B16_A16:
			size *= 8;
			break;
		case R32_G32_B32:
			size *= 12;
			break;
		case R8_G8_B8_A8:
			size *= 4;
			break;
		case R32:
			size *= 4;
			break;
		case R32_G32:
			size *= 8;
			break;
		case R16_G16:
			size *= 4;
			break;
		case R16:
			size *= 2;
			break;
		case R8:
			size *= 1;
			break;
		default:
			break;
	}

	//check alignment
	//size = roundUp(size, 4);
	return size;
}

