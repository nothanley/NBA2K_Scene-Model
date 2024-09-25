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

void CSceneSerializer::registerSerials()
{
	auto modelSerial = std::make_shared<CModelSerializer>(this);
	// todo: add more serializers here

	m_serializers["Model"] = modelSerial;
}

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
	file << m_json.dump(2);
	file.close();

	return true;
}

void CSceneSerializer::serialize()
{
	// clear json
	m_json = JSON();

	// Collect Child JSON obj's
	this->serializeData("Model");

	// todo: add more json data ...
	return;
}

void CSceneSerializer::serializeData(const std::string& key) 
{
	if (m_serializers.find(key) != m_serializers.end())
	{
		auto child  = m_serializers[key]->serialize();
		m_json[key] = *child.get();
	}

	return;
}


