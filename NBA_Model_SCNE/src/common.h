#include <string>
#include <vector>

// store active file path
extern std::string WORKING_DIR;

namespace common
{
    // commonly used global helpers
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
}

