#include <common.h>
#include <fstream>

char* common::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return nullptr;

	std::size_t fileSize = static_cast<std::size_t>(file.tellg());
	file.seekg(0, std::ios::beg);

	char* data = new char[fileSize];
	file.read(data, fileSize);
	file.close();
	return data;
}

uint32_t common::chash(const std::string& str) {
	uint32_t hash = 5381;
	for (char c : str)
		hash = ((hash << 5) + hash) + static_cast<uint32_t>(c);

	return hash;
}

