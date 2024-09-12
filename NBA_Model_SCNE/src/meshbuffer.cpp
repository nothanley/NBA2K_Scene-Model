#include <meshbuffer.h>
#include <nbamodel.h>
#include <scenefile.h>
#include <common.h>
#include <memoryreader.h>

using namespace memreader;

DataBuffer::DataBuffer()
	:
	m_index(0),
	m_stride(1)
{
}

void DataBuffer::loadBinary()
{
	/* Load file data into memory */
	std::string binaryPath = common::format_path(WORKING_DIR + "/" + m_path);
	char* fileBf           = common::readFile(binaryPath);
	if (!fileBf) return;

	/* Decode memory stream */
	char* dataSrc = fileBf;
	auto type = common::splitString(m_format, '_').front();
	auto pack = common::splitString(m_format, '_').back();
	::common::str_to_lower(pack);

	// todo: validate stream length to avoid overrun ...

	// load data
	size_t len = m_size / m_stride;
	std::vector<float> dataTar;
	DatUnpack::getDataSet(dataSrc, len, pack, type, dataTar);
	
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

void DataBuffer::parse(JSON& json)
{
	for (JSON::iterator it = json.begin(); it != json.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
			case enPropertyTag::FORMAT:
				m_format = it.value();
				break;
			case enPropertyTag::STREAM:
				m_index  = it.value();
				break;
			case enPropertyTag::OFFSET:
				m_offset = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SCALE:
				m_scale  = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SIZE:
				m_size = it.value();
				break;
			case enPropertyTag::BINARY:
				m_path = it.value();
				break;
			case enPropertyTag::STRIDE:
				m_stride = it.value();
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
DatUnpack::bitFloat(float value, std::string signValue) {

	if (signValue == "snorm")
		value = float(value / 127.0);
	else if (signValue == "unorm")
		value = float(value / 255.0);
	return value;
}

float
DatUnpack::shortFloat(float value, std::string signValue) {
	if (signValue == "snorm")
		value = float(value / 32767.0);
	else if (signValue == "unorm")
		value = float(value / 65535.0);
	return value;
}

void
DatUnpack::getDataSet(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	unsigned int key = hash(blockType.c_str());

	switch (key)
	{
		case R10_G10_B10_A2:
			getData_R16_G16_B16_A16(data, verts, dataType, blockType, dataSet);
			break;
		case R16_G16_B16_A16:
			getData_R16_G16_B16_A16(data, verts, dataType, blockType, dataSet);
			break;
		case R32_G32_B32:
			getData_R32_G32_B32(data, verts, dataType, blockType, dataSet);
			break;
		case R8_G8_B8_A8:
			getData_R8_G8_B8_A8(data, verts, dataType, blockType, dataSet);
			break;
		case R32_G32:
			getData_R32_G32(data, verts, dataType, blockType, dataSet);
			break;
		case R16_G16:
			getData_R16_G16(data, verts, dataType, blockType, dataSet);
			break;
		case R16:
			getData_R16(data, verts, dataType, blockType, dataSet);
			break;
		case R8:
			getData_R8(data, verts, dataType, blockType, dataSet);
			break;
		default:
			break;
	}

}

void
DatUnpack::getData_R32_G32_B32(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < verts; i++) {
		float r32 = ReadFloat(data);
		float g32 = ReadFloat(data);
		float b32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
		dataSet.push_back(b32);
	}
}

void
DatUnpack::getData_R8_G8_B8_A8(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < verts; i++) {
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

void
DatUnpack::getData_R10_G10_B10_A2(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	for (int i = 0; i < verts; i++) {
		float r32 = ReadFloat(data);
		float g32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
	}
}

void
DatUnpack::getData_R16_G16_B16_A16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < verts; i++) {
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
DatUnpack::getData_R32_G32(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet) {

	for (int i = 0; i < verts; i++) {
		float r32 = ReadFloat(data);
		float g32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
	}
}

void
DatUnpack::getData_R16_G16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet) 
{
	size_t len = verts * 2;

	for (size_t i = 0; i < len; i++) {
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
DatUnpack::getData_R16(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	for (int i = 0; i < verts; i++) {
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
DatUnpack::getData_R8(char*& data, int verts, std::string dataType, std::string blockType, std::vector<float>& dataSet)
{
	for (int i = 0; i < verts; i++) {
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


