#include <cereal/genericserializer.h>
#include <scenereader.h>
#include <nbascene>
#pragma once

class CSceneSerializer
{
public:
	CSceneSerializer(std::shared_ptr<CNBAScene>& scene);

public:
	bool save(const char* path);
	void setScene(std::shared_ptr<CNBAScene>& scene);
	void setPath(const char* path);
	std::shared_ptr<CNBAScene> scene();
	const JSON& json() const;

private:
	void serialize();
	void registerSerials();
	void serializeData(const std::string& key);
	std::unordered_map<std::string, std::shared_ptr<CSerializer>> m_serializers;

private:
	JSON m_json;
	std::string m_path;
	std::shared_ptr<CNBAScene> m_scene;
};



