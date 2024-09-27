#include <material/effect.h>
#include <common_json.h>
#include <common.h>
#include <filesystem>

CNSEffect::CNSEffect()
{

}

std::string 
CNSEffect::id()
{
	return m_name + ".fx";
}

void
CNSEffect::loadShaderResources()
{
	if (m_path.empty())
		return;

	// find all shader paths
	std::vector<std::string> shaderBinaries;

	auto dir = common::get_parent_directory(m_path);
	commonjson::findAllMatchingJsonValues(*m_data.get(), "Binary", shaderBinaries);

	// update paths
	for (auto& binary : shaderBinaries)
	{
		NSShader shader;
		shader.path = dir + "/Shaders/" + binary;
		shader.name = binary;

		// validate shader binaries
		// ...
		m_shaders.push_back(shader);
	}
}

void
CNSEffect::setPath(const char* path)
{
	// update name
	this->m_path = path;
	this->m_name = std::filesystem::path(path).stem().string();
}

void
CNSEffect::setName(const char* name)
{
	this->m_name = name;
}

std::string
CNSEffect::path()
{
	return m_path;
}

void
CNSEffect::setData(std::shared_ptr<JSON> data)
{
	this->m_data = data;

	if (data)
		this->loadShaderResources();
}

std::shared_ptr<JSON>
CNSEffect::data()
{
	return m_data;
}

const std::vector<NSShader>&
CNSEffect::shaders() const
{
	return m_shaders;
}

static void updateBinaryPath(JSON& obj, const char* key, const char* base_path)
{
	/* Recursive scan through json for all 'string' value types with matching input key */
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_string() && it.key() == key)
		{
			it.value() = base_path + it.value().get<std::string>();
		}
		else if (it.value().is_object())
		{
			::updateBinaryPath(it.value(), key, base_path);
		}
	}
}


void
CNSEffect::updateShaderPaths(const char* base_path)
{
	if (!m_data.get())
		return;

	auto unique_subdir = std::string(base_path) + "/";
	::updateBinaryPath(*m_data.get(), "Binary", unique_subdir.c_str());
}
