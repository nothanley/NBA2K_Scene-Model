#include <datastream.h>
#include <memoryreader.h>

using namespace memreader;

DataStream::DataStream()
	:
	m_offset(NULL),
	m_stride(NULL)
{
}

static constexpr uint64_t getMaxIntValue(int bits, bool sign)
{
	return sign ? (1LL << (bits - 1)) - 1 : (1ULL << bits) - 1;
}

static float unpackVarF(float value, int bit_len, std::string type)
{
	if (type == "snorm")
		value = float(value / ::getMaxIntValue(bit_len, true));
	else if (type == "unorm")
		value = float(value / ::getMaxIntValue(bit_len, false));

	return value;
}

static void convertPackVal(float& input, int num_bits, std::string type)
{
	if (type == "snorm" || type == "unorm") {
		input = unpackVarF(input, num_bits, type);
	}
	else if (type == "sint") {
		// ...
	}
	else if (type == "uint") {
		// ...
	}
}

char*
DataStream::roundPointerToNearest4(char* ptr)
{
	std::ptrdiff_t diff = ptr - reinterpret_cast<char*>(0); // Get the pointer difference from nullptr
	std::ptrdiff_t roundedDiff = (diff + 3) & ~3; // Round up to the nearest multiple of 4
	char* roundedPtr = reinterpret_cast<char*>(roundedDiff); // Convert the rounded difference back to a pointer
	return roundedPtr;
}

constexpr unsigned int
DataStream::hash(const char* s, int off) {
	return !s[off] ? 5381 : (hash(s, off + 1) * 33) ^ s[off];
}

int
DataStream::roundUp(int numToRound, int multiple) {
	int remainder;

	if (multiple == 0)
		return numToRound;
	remainder = numToRound % multiple;
	if (remainder == 0)
		return numToRound;

	return (numToRound + multiple - remainder);
}

float
DataStream::bitFloat(float value, std::string type) {

	if (type == "snorm")
		value = float(value / 127.0);
	else if (type == "unorm")
		value = float(value / 255.0);
	return value;
}

float
DataStream::shortFloat(float value, std::string type) {
	if (type == "snorm")
		value = float(value / 32767.0);
	else if (type == "unorm")
		value = float(value / 65535.0);
	return value;
}

void
DataStream::decodeBuffer(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet)
{
	unsigned int key = hash(encoding.c_str());

	switch (key)
	{
	case R16_G16_B16_A16:
		getData_R16_G16_B16_A16(src, size, type, encoding, dataSet);
		break;
	case R10_G10_B10_A2:
		getData_R10_G10_B10_A2(src, size, type, encoding, dataSet);
		break;
	case R32_G32_B32:
		getData_R32_G32_B32(src, size, type, encoding, dataSet);
		break;
	case R8_G8_B8_A8:
		getData_R8_G8_B8_A8(src, size, type, encoding, dataSet);
		break;
	case R32_G32:
		getData_R32_G32(src, size, type, encoding, dataSet);
		break;
	case R16_G16:
		getData_R16_G16(src, size, type, encoding, dataSet);
		break;
	case R32:
		getData_R32(src, size, type, encoding, dataSet);
		break;
	case R16:
		getData_R16(src, size, type, encoding, dataSet);
		break;
	case R8:
		getData_R8(src, size, type, encoding, dataSet);
		break;
	default:
		printf("\n[DataUnpack] Error: could not resolve encoding format: %s", encoding.c_str());
		break;
	}

}

void
DataStream::getData_R32_G32_B32(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet) 
{
	float r32, g32, b32;

	for (int i = 0; i < size; i++) 
	{
		char* data = src + (i * m_stride) + m_offset;

		r32 = ReadFloat(data);
		g32 = ReadFloat(data);
		b32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
		dataSet.push_back(b32);
	}
}

void
DataStream::getData_R8_G8_B8_A8(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet) 
{
	float r8f, g8f, b8f, a8f;

	for (int i = 0; i < size; i++) 
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			int8_t r8 = ReadInt8(data);
			int8_t g8 = ReadInt8(data);
			int8_t b8 = ReadInt8(data);
			int8_t a8 = ReadInt8(data);

			r8f = bitFloat(r8, type);
			g8f = bitFloat(g8, type);
			b8f = bitFloat(b8, type);
			a8f = bitFloat(a8, type);
		}
		else if (type == "unorm") {
			uint8_t r8 = ReadUInt8(data);
			uint8_t g8 = ReadUInt8(data);
			uint8_t b8 = ReadUInt8(data);
			uint8_t a8 = ReadUInt8(data);

			r8f = bitFloat(r8, type);
			g8f = bitFloat(g8, type);
			b8f = bitFloat(b8, type);
			a8f = bitFloat(a8, type);
		}
		else if (type == "sint") {
			int8_t r8 = ReadInt8(data);
			int8_t g8 = ReadInt8(data);
			int8_t b8 = ReadInt8(data);
			int8_t a8 = ReadInt8(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}
		else if (type == "uint") {
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
DataStream::getData_R10_G10_B10_A2(char*& src, int size, std::string type, std::string encodeFmt, std::vector<float>& dataSet)
{
	float r, g, b, a;
	uint32_t packedValue;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;
		packedValue = ReadUInt32(data); // todo: check for bitswap requirements...

		r = (packedValue >> 22) & 0x3FF;
		g = (packedValue >> 12) & 0x3FF;
		b = (packedValue >> 2) & 0x3FF;
		a = packedValue & 0x3;

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
DataStream::getData_R16_G16_B16_A16(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet) 
{
	float r8f, g8f, b8f, a8f;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			int16_t r8 = ReadInt16(data);
			int16_t g8 = ReadInt16(data);
			int16_t b8 = ReadInt16(data);
			int16_t a8 = ReadInt16(data);

			r8f = shortFloat(r8, type);
			g8f = shortFloat(g8, type);
			b8f = shortFloat(b8, type);
			a8f = shortFloat(a8, type);
		}
		else if (type == "unorm") {
			uint16_t r8 = ReadUInt16(data);
			uint16_t g8 = ReadUInt16(data);
			uint16_t b8 = ReadUInt16(data);
			uint16_t a8 = ReadUInt16(data);

			r8f = shortFloat(r8, type);
			g8f = shortFloat(g8, type);
			b8f = shortFloat(b8, type);
			a8f = shortFloat(a8, type);
		}
		else if (type == "sint") {
			int16_t r8 = ReadInt16(data);
			int16_t g8 = ReadInt16(data);
			int16_t b8 = ReadInt16(data);
			int16_t a8 = ReadInt16(data);

			r8f = r8;
			g8f = g8;
			b8f = b8;
			a8f = a8;
		}
		else if (type == "uint") {
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
DataStream::getData_R32_G32(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet) 
{
	float r32, g32;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;

		r32 = ReadFloat(data);
		g32 = ReadFloat(data);

		dataSet.push_back(r32);
		dataSet.push_back(g32);
	}
}

void
DataStream::getData_R16_G16(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet)
{
	float r_f, g_f;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			auto r_i = ReadInt16(data);
			auto g_i = ReadInt16(data);

			r_f = unpackVarF(r_i, 16, type);
			g_f = unpackVarF(g_i, 16, type);
		}
		else if (type == "unorm") {
			auto r_i = ReadUInt16(data);
			auto g_i = ReadUInt16(data);

			r_f = unpackVarF(r_i, 16, type);
			g_f = unpackVarF(g_i, 16, type);
		}
		else if (type == "sint") {
			r_f = ReadInt16(data);
			g_f = ReadInt16(data);
		}
		else if (type == "uint") {
			r_f = ReadInt16(data);
			g_f = ReadInt16(data);
		}

		dataSet.push_back(r_f);
		dataSet.push_back(g_f);
	}
}

void
DataStream::getData_R32(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet)
{
	float result;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			auto val = ReadInt32(data);
			result = unpackVarF(val, 32, type);
		}
		else if (type == "unorm") {
			auto val = ReadUInt32(data);
			result = unpackVarF(val, 32, type);
		}
		else if (type == "sint") {
			auto val = ReadInt32(data);
			result = val;
		}
		else if (type == "uint") {
			auto val = ReadUInt32(data);
			result = val;
		}

		dataSet.push_back(result);
	}
}

void
DataStream::getData_R16(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet)
{
	float result;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			auto val = ReadInt16(data);
			result = bitFloat(val, type);
		}
		else if (type == "unorm") {
			auto val = ReadUInt16(data);
			result = bitFloat(val, type);
		}
		else if (type == "sint") {
			auto val = ReadInt16(data);
			result = val;
		}
		else if (type == "uint") {
			auto val = ReadUInt16(data);
			result = val;
		}

		dataSet.push_back(result);
	}
}

void
DataStream::getData_R8(char*& src, int size, std::string type, std::string encoding, std::vector<float>& dataSet)
{
	float r8f;

	for (int i = 0; i < size; i++) 
	{
		char* data = src + (i * m_stride) + m_offset;

		if (type == "snorm") {
			int8_t r8 = ReadInt8(data);

			r8f = bitFloat(r8, type);
		}
		else if (type == "unorm") {
			uint8_t r8 = ReadUInt8(data);

			r8f = bitFloat(r8, type);
		}
		else if (type == "sint") {
			int8_t r8 = ReadInt8(data);

			r8f = r8;
		}
		else if (type == "uint") {
			uint8_t r8 = ReadUInt8(data);

			r8f = r8;
		}

		dataSet.push_back(r8f);
	}
}


size_t 
DataStream::getDataLen(int items, std::string encoding)
{
	size_t size = items;
	unsigned int buf = hash(encoding.c_str());

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

