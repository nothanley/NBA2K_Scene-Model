#include <scenefile.h>
#pragma once

typedef std::vector<std::string> string_list;

class CSceneFileScanner : public CSceneFile
{

public:
	CSceneFileScanner(const char* path);
	~CSceneFileScanner();

public:
	void load() override;

private:
	void processFile(int index, const std::string& tool_path, const std::string& target_dir);
	std::string extract_file(const std::string& tool_path, const std::string& file_name); // extracts file from game archives - returns local file path if successful 
	void updateFromManifest(); // updates full file paths by scanning the game manifest
	void filterExisting(); // scans through working dir and subdirs for missing dependencies
	void grabDependencies();
	void getFileDependencyList();
	void findGamePath();

private:
	string_list m_fileList;
	std::string m_gamePath;
};

