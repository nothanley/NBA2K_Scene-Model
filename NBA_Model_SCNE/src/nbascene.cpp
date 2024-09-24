#include <common.h>
#include <nbamodel.h>
#include <nbascene.h>
#include <meshstructs.h>

CNBAScene::CNBAScene(const char* id)
	:
	m_name(id)
{
}

int CNBAScene::getNumModels() {
	return m_models.size();
}

const char* CNBAScene::getName() {
	return this->m_name.c_str();
}

void CNBAScene::setName(const char* id) {
	this->m_name = id;
}

std::shared_ptr<CNBAModel>& CNBAScene::model(const int index)
{
	return m_models.at(index);
}

std::vector<std::shared_ptr<CNBAModel>>& CNBAScene::models()
{
	return m_models;
}

bool CNBAScene::empty()
{
	return m_models.empty();
}

void CNBAScene::pushModel(const CNBAModel& model)
{
	auto newModel = std::make_shared<CNBAModel>(model);
	m_models.push_back(newModel);
}