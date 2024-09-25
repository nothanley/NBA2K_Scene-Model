#include <cereal/sceneserializer.h>
#include <cereal/modelserializer.h>

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
	auto modelSerial = std::make_shared<CModelSerializer>(this);
	// todo: add more serializers here

	m_serializers["Model"] = modelSerial;
}

static inline std::string formatJsonStream(const JSON& json)
{
	auto data = json.dump(4);
	data.erase(data.begin());
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
	this->serializeData("Model", rootJson);

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


