// Stores scene attributes and model children. Extrapolates CNBAModel objects from given JSON.

#include <vector>
#include <memory>
#include <string>
#pragma once

struct Material;
class CNBAModel;

class CNBAScene
{
public:
	CNBAScene(const char* name);

public:
	bool empty();
	std::shared_ptr<CNBAModel>& model(const int index);
	std::vector<std::shared_ptr<CNBAModel>>& models();
	
public:
	int getNumModels();
	const char* getName();
	void setName(const char* id);

public:
	void pushModel(const CNBAModel& model);

protected:
	std::string m_name;
	std::vector<std::shared_ptr<CNBAModel>> m_models;
	std::vector<Material> m_materials;
};


