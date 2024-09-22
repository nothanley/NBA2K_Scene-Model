#include <bin_format.h>
#include <algorithm>
#include <memoryreader.h>

using namespace memreader;

static constexpr uint64_t getMaxIntValue(int bits, bool sign)
{
    return sign ? (1LL << (bits - 1)) - 1 : (1ULL << bits) - 1;
}

static float unpackValue(const float input, const int num_bits, const std::string& type)
{
    if (type == "snorm")
        return (input / ::getMaxIntValue(num_bits, true));

	if (type == "unorm")
		return (input / ::getMaxIntValue(num_bits, false));

	return input;
}

template <int Channels>
int Format_32Bit<Channels>::get_channels()
{
	return Channels;
}
template <int Channels>
int Format_16Bit<Channels>::get_channels()
{
	return Channels;
}

template <int Channels>
int Format_8Bit<Channels>::get_channels()
{
	return Channels;
}

int R10G10B10A2::get_channels()
{
	return 4;
}

template <int Channels>
int Format_32Bit<Channels>::get_size(const int items)
{
	return items * sizeof(int32_t) * Channels;
}
template <int Channels>
int Format_16Bit<Channels>::get_size(const int items)
{
	return items * sizeof(int16_t) * Channels;
}

template <int Channels>
int Format_8Bit<Channels>::get_size(const int items)
{
	return items * sizeof(int8_t) * Channels;
}

int R10G10B10A2::get_size(const int items)
{
	return items * sizeof(int32_t);
}

template <int Channels>
void Format_32Bit<Channels>::decode(FMT_DT_PARAMS)
{
	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * stride) + offset;

		for (int j = 0; j < Channels; j++) // Loop through RBGA Channels
			target.push_back( ReadFloat(data) );
	}
}


template <int Channels>
void Format_16Bit<Channels>::decode(FMT_DT_PARAMS)
{
	auto unpack = [type](char*& data) -> float{
		if (type == "snorm" || type == "sint")
			return ::unpackValue(ReadInt16(data), 16, type);
		else if (type == "unorm" || type == "uint")
			return ::unpackValue(ReadUInt16(data), 16, type);
	};

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * stride) + offset;

		for (int j = 0; j < Channels; j++) // Loop through RBGA Channels
			target.push_back(unpack(data));
	}
}


template <int Channels>
void Format_8Bit<Channels>::decode(FMT_DT_PARAMS)
{
	auto unpack = [type](char*& data) -> float{
		if (type == "snorm" || type == "sint")
			return ::unpackValue(ReadInt8(data), 8, type);
		else if (type == "unorm" || type == "uint")
			return ::unpackValue(ReadUInt8(data), 8, type);
	};

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * stride) + offset;

		for (int j = 0; j < Channels; j++) // Loop through RBGA Channels
			target.push_back(unpack(data));
	}
}

void R10G10B10A2::decode(FMT_DT_PARAMS)
{
	float r, g, b, a;
	uint32_t packedValue;

	for (int i = 0; i < size; i++)
	{
		char* data = src + (i * stride) + offset;
		packedValue = ReadUInt32(data);

		r = (packedValue >> 0)  & 0x3FF;
		g = (packedValue >> 10) & 0x3FF;
		b = (packedValue >> 20) & 0x3FF;
		a = (packedValue >> 30) & 0x3;

		target.push_back( unpackValue(r, 10, type) );
		target.push_back( unpackValue(g, 10, type) );
		target.push_back( unpackValue(b, 10, type) );
		target.push_back( unpackValue(a, 02, type) );
	}
};

template <int Channels>
void Format_32Bit<Channels>::updateData(INJ_DT_PARAMS)
{
	int numItems = size / Channels;

	for (int i = 0; i < numItems; i++)
	{
		char* stream = src + (i * stride) + offset;
		
		for (int j = 0; j < Channels; j++)
		{
			int index = (i * Channels) + j;
			WriteFloat(stream, target[index]);
		}
	}
}

template <int Channels>
void Format_16Bit<Channels>::updateData(INJ_DT_PARAMS)
{
	int numItems = size / Channels;

	for (int i = 0; i < numItems; i++)
	{
		char* stream = src + (i * stride) + offset;

		for (int j = 0; j < Channels; j++)
		{
			int index    = (i * Channels) + j;
			float value  = target[index];

			if (type == "snorm" || type == "sint")
			{
				int16_t pack = (type == "snorm") ? (value * ::getMaxIntValue(m_bits, true)) : value;
				WriteSInt16(stream, pack);
			}
			else if (type == "unorm" || type == "uint")
			{
				uint16_t pack = (type == "unorm") ? (value * ::getMaxIntValue(m_bits, false)) : value;
				WriteUInt16(stream, pack);
			}
		}
	}
}

template <int Channels>
void Format_8Bit<Channels>::updateData(INJ_DT_PARAMS)
{
	int numItems = size / Channels;

	for (int i = 0; i < numItems; i++)
	{
		char* stream = src + (i * stride) + offset;

		for (int j = 0; j < Channels; j++)
		{
			int index   = (i * Channels) + j;
			float value = target[index];

			if (type == "snorm" || type == "sint")
			{
				int8_t pack = (type == "snorm") ? (value * ::getMaxIntValue(m_bits, true)) : value;
				WriteSInt8(stream, pack);
			}
			else if (type == "unorm" || type == "uint")
			{
				uint8_t pack = (type == "unorm") ? (value * ::getMaxIntValue(m_bits, false)) : value;
				WriteUInt8(stream, pack);
			}
		}
	}
}

void R10G10B10A2::updateData(INJ_DT_PARAMS)
{
	int channels = 4;
	int numItems = size / channels;

	for (int i = 0; i < numItems; i++)
	{
		char* stream = src + (i * stride) + offset;
		std::vector<float> values;

		// Load 4 float values from the target vector
        uint32_t packedR = (target[(i * channels) + 0]);
        uint32_t packedG = (target[(i * channels) + 1]);
        uint32_t packedB = (target[(i * channels) + 2]);
        uint32_t packedA = (target[(i * channels) + 3]);

        // Combine packed values into a single 32-bit unsigned integer
        uint32_t packedValue =  (packedR & 0x3FF)        
                             | ((packedG & 0x3FF) << 10)
                             | ((packedB & 0x3FF) << 20)
                             | ((packedA & 0x3)   << 30); 

        // Write the packed value into the stream
		WriteUInt32(stream, packedValue);
	}
}

