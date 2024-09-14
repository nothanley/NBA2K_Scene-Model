#include "filescanner.h"
#include <common.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>   // For _chdir() on Windows

CSceneFileScanner::CSceneFileScanner(const char* path)
	:
	CSceneFile(path)
{
}

CSceneFileScanner::~CSceneFileScanner()
{
}

void CSceneFileScanner::load()
{
	if (!validate())
		throw std::runtime_error("Cannot read scene file.");

	this->findGamePath();
	this->getFileDependencyList();
	this->filterExisting();
	this->grabDependencies();
}

void
CSceneFileScanner::filterExisting() 
{
	string_list filteredPaths;
	std::string localPath;
	auto parent_dir = common::get_parent_directory(m_path);

	for (auto& file_name : m_fileList)
	{
		// First, search for regular file (.gz compressed if applicable)
		localPath = common::findFileInDirectory(parent_dir, file_name);
		
		// secondly, search for decompressed files...
		if (localPath.empty())
		{
			std::string decompName = file_name;
			common::replaceSubString(decompName, ".gz", ".bin");
			localPath = common::findFileInDirectory(parent_dir, decompName);
		}

		// filter file path if either were found
		if (localPath.empty())
			filteredPaths.push_back(file_name);
	}

	this->m_fileList = filteredPaths;
}

static void findAllMatchingJsonValues(JSON& obj, const char* target_key, string_list& list)
{
	/* Recursive scan through json for all 'string' value types with matching input key */
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_string() && it.key() == target_key)
		{
			// format string case
			std::string child_string = it.value();
			common::str_to_lower(child_string);

			// append to list
			list.push_back(child_string);
		}
		else if (it.value().is_object())
		{
			::findAllMatchingJsonValues(it.value(), target_key, list);
		}
	}
}

void
CSceneFileScanner::getFileDependencyList()
{
	if (!validate())
		throw std::runtime_error("Cannot read scene file.");

	// log status 
	common::set_console_text_color(TEXT_AQUA_CON);
	printf("\n\n[CSceneFileScanner] Scanning scene file: %s\n", m_path.c_str());
	common::set_console_text_color(TEXT_WHITE_CON);

	// scan json for target key values - only scans for binary files but could apply to textures etc.
	::findAllMatchingJsonValues(m_json, "Binary", m_fileList);
}

void
CSceneFileScanner::findGamePath()
{
	auto parent_dir  = common::get_parent_directory(m_path);
	this->m_gamePath = common::find_parent_directory(parent_dir, "NBA 2K25");

	if (m_gamePath.empty())
	{
		std::cerr << "[CSceneFileScanner] Failed to locate NBA 2K25 game path. Please ensure the scanner is run within the 'mods' folder." << std::endl;
		throw std::runtime_error("");
	}
}

void
CSceneFileScanner::updateFromManifest()
{
	auto manifestPath = m_gamePath + "/manifest";

	// Check if file is accessible
	std::ifstream file(manifestPath);
	if (!common::fileExists(manifestPath) || !file.is_open()) {
		std::cerr << "[CSceneFileScanner] Failed to access/read NBA 2K25 manifest." << std::endl;
		throw std::runtime_error("");
	}

	// Scan through manifest items
	std::string line, item;
	while (std::getline(file, line))
	{
		std::istringstream stream(line);

		// Check/Update if the item matches our target file name
		if (std::getline(stream, item, ','))
			for (auto& file_name : m_fileList)
				file_name = (common::containsSubstring(item, file_name)) ? item : file_name;
	}

	file.close();
}

std::string
CSceneFileScanner::extract_file(const std::string& tool_path, const std::string& target_file)
{
	std::string command = "\"" + tool_path + "\" " + target_file;

	// set working directory to tool path
	auto orig_dir = std::filesystem::current_path();
	auto tool_dir = common::get_parent_directory(tool_path);
	std::filesystem::current_path(tool_dir);

	// Call mod.exe for target file
	system(command.c_str());

	// Scan subdirectories for result
	auto file_name = std::filesystem::path(target_file).filename().string();
	auto result    = common::findFileInDirectory(m_gamePath, file_name);

	// restore working directory
	std::filesystem::current_path(orig_dir);
	return result;
}



void
CSceneFileScanner::grabDependencies()
{
	/* Use mod tool to extract files */
	auto targetDir   = common::get_parent_directory(m_path);
	auto modToolPath  = m_gamePath + "/mod.exe";

	if (!common::fileExists(modToolPath)){
		std::cerr << "[CSceneFileScanner] Failed to locate NBA 2K25 'mod.exe' tool." << std::endl;
		throw std::runtime_error("");
	}

	/* Search manifest for full dependency file paths */
	printf("\n\n[CSceneFileScanner] Scanning manifest items...\n");
	this->updateFromManifest();

	/* Extract files from archive using dependency list */
	for (int i = 0; i < m_fileList.size(); i++)
	{
		this->processFile(i, modToolPath, targetDir);
	}
}

void
CSceneFileScanner::processFile(int index, const std::string& tool_path, const std::string& target_dir)
{
	int item_total = m_fileList.size() - 1;

	if (index > item_total) return;
	auto& fileName = m_fileList.at(index);

	// Log status...
	common::set_console_text_color(TEXT_YELLOW_CON);
	printf("\n[CSceneFileScanner] (%d/%d) Extracting file: \"%s\"...\n", index, item_total, fileName.c_str());
	common::set_console_text_color(TEXT_WHITE_CON);

	// Run 'mod.exe' for target file
	auto result = extract_file(tool_path.c_str(), fileName.c_str());

	if (!result.empty())
	{	// Copy results to .scene folder
		auto src = std::filesystem::path(result);
		auto dst = std::filesystem::path(target_dir) / src.filename();
		std::filesystem::copy_file(result, dst, std::filesystem::copy_options::overwrite_existing);

		common::set_console_text_color(TEXT_GREEN_CON);
		printf("[CSceneFileScanner] Copied file to \"%s\"\n", dst.string().c_str());
		common::set_console_text_color(TEXT_WHITE_CON);
	}
	else {
		std::cerr << "\n[CSceneFileScanner] Failed to extract file.";
	}
}

