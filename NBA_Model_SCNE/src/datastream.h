#pragma once
#include <string>
#include <vector>

enum {
	R16_G16_B16_A16 = 123583891,
	R10_G10_B10_A2 = 775460992,
	R32_G32_B32 = 1171538707,
	R8_G8_B8_A8 = 3503152076,
	R32 = 193360022,
	R32_G32 = 1382734287,
	R16_G16 = 3666862864,
	R16 = 193355856,
	R8 = 5859695,
};

class CDataStream
{

public:
	CDataStream();

public:
	std::vector<float> data; // stores all data eg. pos, uvs, indices

	static void convertPackVal(float& input, int num_bits, std::string type);
	static char* roundPointerToNearest4(char* ptr);
	static constexpr unsigned int hash(const char* s, int off = 0);
	static int roundUp(int numToRound, int multiple);
	static float bitFloat(float value, std::string signValue);
	static float shortFloat(float value, std::string signValue);
	static size_t getDataLen(int items, std::string encoding);

protected:
	void decodeBuffer(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R32_G32_B32(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R8_G8_B8_A8(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R16_G16_B16_A16(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R10_G10_B10_A2(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R32_G32(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R16_G16(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R8(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R16(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);
	void getData_R32(char*& data, int items, std::string type, std::string encoding, std::vector<float>& dst);


protected:
	int m_offset;
	int m_stride;
};

