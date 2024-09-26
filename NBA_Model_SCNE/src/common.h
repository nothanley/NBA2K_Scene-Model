// commonly used global functions and generic helpers

#include <string>
#include <vector>

#define TEXT_GREEN_CON 2
#define TEXT_RED_CON 4
#define TEXT_YELLOW_CON 6
#define TEXT_WHITE_CON 7
#define TEXT_AQUA_CON 3

// store active file path to global scope
extern std::string WORKING_DIR;

namespace common
{
    char* readFile(const std::string& filename, size_t* data_length = nullptr);
    uint32_t chash(const std::string& str);
    std::string get_exe_path();
    std::string format_path(const std::string& path);
    std::string get_parent_directory(const std::string& filePath);
	std::string get_relative_file_path(const std::string& name);
    std::vector<std::string> splitString(const std::string& str, const char delimiter);
    void removeSubString(std::string& str, const std::string target);
    void replaceSubString(std::string& str, const std::string old_string, const std::string new_string);
    void str_to_lower(std::string& string);
    std::string to_lower(const std::string& string);
    bool containsSubstring(const std::string& str, const std::string substring);
	bool fileExists(const std::string& filePath);
	std::string find_parent_directory(const std::string& path, const char* target_name);
	std::string findFileInDirectory(const std::string& mainDir, const std::string& filename);
    void set_console_text_color(int val);
    std::vector<std::string> findMatchingExtensionFiles(const char* directory, const char* extension);
    void ShowMessageBox(const std::string& message);
    void createFileBackup(const char* path);
    std::string findTargetFileExt(const char* directory, const char* extension);
}

