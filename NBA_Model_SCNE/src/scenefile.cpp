#include <memoryreader.h>
#include <scenefile.h>
#include <json.hpp>
#include <common.h>
#include <nbamodel.h>
#include <nbascene.h>

using namespace memreader;

CSceneFile::CSceneFile(const char* path)
	: 
	m_path(path)
{
}

CSceneFile::~CSceneFile()
{
}

void
CSceneFile::load()
{
	if (!validate())
		throw std::runtime_error("Cannot read scene file.");

	/* load skin model object */
	CSceneFile::parse();
}

std::shared_ptr<CNBAScene>&
CSceneFile::scene()
{
	return m_scene;
}

void
CSceneFile::parse()
{
	printf("\n\n[CSceneFile] Loading Scene File: %s\n", m_path.c_str());
	
	/* Update global active file */
	WORKING_DIR = common::get_parent_directory(m_path);

	/* Iterate through scene json structure */
	for (JSON::iterator it = m_json.begin(); it != m_json.end(); ++it)
	{
		if (it.value().is_object()) {
			std::string scene_id = it.key();

			m_scene = std::make_shared<CNBAScene>(scene_id.c_str(), it.value());
			m_scene->parse();
		}
	}
}


bool
CSceneFile::validate()
{
	try 
	{
		auto data = formatInputJson(m_path);
		m_json = JSON::parse(data.c_str());
	}
	catch (...) {return false;}
	
	return true;
}


static std::string eraseChars(const std::string& str, const char target) {
	size_t startPos = 0;
	std::string result = str;

	// Find and remove all occurrences of the target character
	while ((startPos = result.find(target, startPos)) != std::string::npos) {
		result.erase(startPos, 1); 
	}

	return result;
}

static void makeJsonKeyUnique(std::string& rawJson, const std::string key_name)
{
	size_t startPos = 0;

	while ((startPos = rawJson.find(key_name, startPos)) != std::string::npos)
	{
		std::string unique_key = '\"' + ::eraseChars(key_name, '\"') + std::to_string(startPos) + '\"';
		rawJson.replace(startPos, key_name.length(), unique_key);
		startPos += unique_key.length();
	}
}

std::string
CSceneFile::formatInputJson(const std::string& path)
{
	std::ifstream inputFile(path);

	if (!inputFile)
		return "";

	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	// Correct object brackets
	std::string data = "{" + buffer.str() + "}";

	// Correct matching keys...
	common::removeSubString(data, "\tnull,");
	::makeJsonKeyUnique(data, "\"VertexBuffer\"");

	return data;
}