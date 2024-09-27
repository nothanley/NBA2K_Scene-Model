#include <string>
#include <memory>
#include <vector>
#include <json.hpp>
#pragma once

// namespace types
using JSON = nlohmann::ordered_json;

// game material class
class CNSTexture
{
public:
	CNSTexture();
	CNSTexture(const char* name, const int width, const int height);

public:
	bool empty() const;
	void setSize(const int width, const int height, const int channels=4);
	void setPath(const char* path);
	void setType(const char* type);
	void setName(const char* name);
	void setPixmap(const std::vector<float>& pixmap);
	void setPixmap(const float* pixmap, const size_t& size);
	void setMipsTarget(const int mips);

public:
	std::string generateFileId() const;
	const std::vector<float>& pixmap() const;
	const std::string name() const;
	const std::string type() const;
	const std::string path() const;
	const int width() const;
	const int height() const;
	const int channels() const;
	const int mips() const;

private:
	std::string m_type;
	std::string m_name;
	std::string m_path;
	int m_mips;
	int m_width;
	int m_height;
	int m_channels;
	std::vector<float> m_pixMap;
};

