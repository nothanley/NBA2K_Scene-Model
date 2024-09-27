#include <cereal/sceneserializer.h>
#include <cereal/modelserializer.h>
#include <cereal/materialserializer.h>
#include <cereal/effectserializer.h>
#include <cereal/objectserializer.h>
#include <cereal/textureserializer.h>

CSceneSerializer::CSceneSerializer(std::shared_ptr<CNBAScene>& scene)
	:
	m_scene(scene)
{
	this->registerSerials();
}

void CSceneSerializer::setScene(std::shared_ptr<CNBAScene>& scene)
{
	m_scene = scene;
}

void CSceneSerializer::setPath(const char* path)
{
	m_path = path;
}

std::shared_ptr<CNBAScene> CSceneSerializer::scene()
{
	return m_scene;
}

const JSON& CSceneSerializer::json() const
{
	return m_json;
}

const std::string& CSceneSerializer::path() const
{
	return m_path;
}

void CSceneSerializer::registerSerials()
{
	m_serializers["Model"]    = std::make_shared<CModelSerializer>(this);
	m_serializers["Texture"]  = std::make_shared<CTextureSerializer>(this);
	m_serializers["Material"] = std::make_shared<CMaterialSerializer>(this);
	m_serializers["Effect"]   = std::make_shared<CEffectSerializer>(this);
	m_serializers["Object"]   = std::make_shared<CObjectSerializer>(this);
}

static inline std::string formatJsonStream(const JSON& json)
{
	auto data = json.dump(1);
	data.erase(data.begin());
	data.erase(data.begin());
	data.erase(data.begin());
	data.pop_back();
	data.pop_back();
	return data;
};

bool CSceneSerializer::save(const char* path)
{
	// Initialize save path
	this->setPath(path);
	if (m_path.empty())
		return false;

	// create json data
	this->serialize();

	// open save file
	std::ofstream file(m_path);
	if (!file.is_open())
		return false;

	// send formatted json data to file
	file << ::formatJsonStream(m_json);
	file.close();

	return true;
}

void CSceneSerializer::serialize()
{
	// clear json
	this->m_json  = JSON();
	auto rootJson = JSON();

	// Collect Child JSON obj's
	this->serializeData("Effect", rootJson);
	this->serializeData("Texture", rootJson);
	this->serializeData("Material", rootJson);
	this->serializeData("Model", rootJson);
	this->serializeData("Object", rootJson);

	// define parent json obj
	m_json[m_scene->getName()] = rootJson;
	return;
}

void CSceneSerializer::serializeData(const std::string& key, JSON& parent) 
{
	if (m_serializers.find(key) != m_serializers.end())
	{
		auto child  = m_serializers[key]->serialize();
		parent[key] = *child.get();
	}

	return;
}


