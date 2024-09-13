#include <common.h>
#include <fstream>
#include <Windows.h>
#include <algorithm>

std::string WORKING_DIR = "";

char* common::readFile(const std::string& filename, size_t* data_length)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return nullptr;

	std::size_t fileSize = static_cast<std::size_t>(file.tellg());
	file.seekg(0, std::ios::beg);

	char* data = new char[fileSize];
	file.read(data, fileSize);
	file.close();

	*data_length = (data_length) ? fileSize : NULL;
	return data;
}

uint32_t common::chash(const std::string& str) {
	uint32_t hash = 5381;
	for (char c : str)
		hash = ((hash << 5) + hash) + static_cast<uint32_t>(c);

	return hash;
}

std::string common::get_exe_path()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string fullPath(buffer);
	size_t lastSlash = fullPath.find_last_of("\\");
	return fullPath.substr(0, lastSlash);
}

void common::removeSubString(std::string& str, const std::string target)
{
	size_t startPos = 0;

	while ((startPos = str.find(target, startPos)) != std::string::npos)
	{
		str.replace(startPos, target.length(), "");
	}
}

void common::replaceSubString(std::string& str, const std::string old_string, const std::string new_string)
{
	size_t startPos = 0;

	while ((startPos = str.find(old_string, startPos)) != std::string::npos)
	{
		str.replace(startPos, old_string.length(), new_string);
		startPos += new_string.length();
	}
}

bool common::containsSubstring(const std::string& str, const std::string& substring) {
	return str.find(substring) != std::string::npos;
}

void common::str_to_lower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) { return std::tolower(c); });
}

std::string common::format_path(const std::string& path)
{
	std::string result;

	for (char ch : path)
	{	// Format all back/forward slashes
		if (ch == '\\')
			result.push_back('/');
		else
			result.push_back(ch);
	}
	return result;
}

std::string common::get_relative_file_path(const std::string& name)
{
	std::string file_path = get_exe_path() + "/" + name;
	return common::format_path(file_path);
};

std::string common::get_parent_directory(const std::string& filePath) 
{
	size_t pos = filePath.find_last_of("/\\");

	if (pos != std::string::npos) {
		return filePath.substr(0, pos);
	}

	return "";
}

std::vector<std::string> common::splitString(const std::string& str, const char delimiter) {
	std::vector<std::string> result;
	size_t pos = str.find(delimiter);

	if (pos != std::string::npos) {
		// First part before the delimiter
		result.push_back(str.substr(0, pos));
		// Second part after the delimiter
		result.push_back(str.substr(pos + 1));
	}

	return result;
}