#include <string>

namespace common
{
	char* readFile(const std::string& filename);
	uint32_t chash(const std::string& str);
}