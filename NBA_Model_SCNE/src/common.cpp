#include <common.h>
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <filesystem>

#define _HAS_STD_BYTE 0
namespace fs = std::filesystem;

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

bool common::containsSubstring(const std::string& str, const std::string substring) {
	return str.find(substring) != std::string::npos;
}

void common::str_to_lower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) { return std::tolower(c); });
}

std::string common::to_lower(const std::string& string)
{
	std::string result = string;
	std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });

	return result;
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

bool common::fileExists(const std::string& filePath)
{
	return fs::exists(filePath) && fs::is_regular_file(filePath);
}

std::string common::find_parent_directory(const std::string& path, const char* target_name)
{
	fs::path currentPath = path;

	while (currentPath.has_parent_path()) {
		std::string directoryName = currentPath.filename().string();

		if (directoryName.find(target_name) != std::string::npos) {
			return currentPath.string();
		}

		currentPath = currentPath.parent_path();
	}

	return "";
}

std::string common::findFileInDirectory(const std::string& mainDir, const std::string& filename)
{
	if (!fs::exists(mainDir) || !fs::is_directory(mainDir))
		return "";

	// Recursively iterate through the directory and subdirectories for existing file
	std::string local_file;
	std::string target_fmt = filename;
	common::str_to_lower(target_fmt);

	for (const auto& entry : fs::recursive_directory_iterator(mainDir)) 
		if (fs::is_regular_file(entry)) 
		{
			local_file = entry.path().filename().string();
			common::str_to_lower(local_file); // match case...

			if (local_file == target_fmt)
				return entry.path().string();
		}

	return "";
}

void common::set_console_text_color(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, k);
}

std::vector<std::string> common::findMatchingExtensionFiles(const char* directory, const char* extension)
{
	std::vector<std::string> matchingFiles;

	if (!fs::exists(directory) || !fs::is_directory(directory))
		return matchingFiles;

	try {
		for (const auto& entry : fs::directory_iterator(directory)) {
			// Check if the entry is a regular file and has the specified extension
			if (entry.is_regular_file() && entry.path().extension() == extension) {
				// Add the full file path to the result vector
				matchingFiles.push_back(entry.path().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {}

	return matchingFiles;
}

void common::ShowMessageBox(const std::string& message) {
	// Convert std::string to a wide string (required by MessageBoxW)
	std::wstring wideMessage(message.begin(), message.end());

	// Create a message box with "OK" button
	MessageBoxW(
		nullptr,             // No parent window
		wideMessage.c_str(), // Message to display (wide string)
		L"Blender NBA Link",          // Title of the message box
		MB_OK                // OK button type
	);
}

void common::createFileBackup(const char* path) 
{
	try {
		fs::path originalFilePath(path);

		if (!fs::exists(originalFilePath))
			return;

		fs::path backupFilePath = originalFilePath;
		backupFilePath += ".bak";

		// Copy the original file to the backup file path
		fs::copy_file(originalFilePath, backupFilePath, fs::copy_options::overwrite_existing);
	}
	catch (const std::exception& e) 
	{
		// ...
	}
}

std::string common::findTargetFileExt(const char* directory, const char* extension)
{
	for (const auto& entry : std::filesystem::directory_iterator(directory)) 
	{
		if (entry.is_regular_file() && entry.path().extension() == extension) {
			return entry.path().string(); // Return the first matching file
		}
	}

	return "";
}

