#include <common.h>
#include <nbamodel.h>
#include <nbascene.h>

CNBAScene::CNBAScene(const char* id)
	:
	m_name(id)
{
}

CNBAScene::CNBAScene(const char* id, JSON& json)
	: 
	m_name(id),
	m_json(json)
{
}

const char* CNBAScene::getName() {
	return this->m_name.c_str();
}

void CNBAScene::setName(const char* id) {
	this->m_name = id;
}


void CNBAScene::parse()
{
	for (JSON::iterator ele = m_json.begin(); ele != m_json.end(); ++ele)
	{
		if (ele.value().is_object())
		{
			auto key = common::chash(ele.key());

			switch (key)
			{
			case enSceneData::EFFECT:
				break;
			case enSceneData::MODEL:
				readModels(ele.value());
				break;
			case enSceneData::MATERIAL:
				break;
			case enSceneData::TEXTURE:
				break;
			case enSceneData::OBJECT:
				break;
			default:
				break;
			};
		}
	}
}

void CNBAScene::readModels(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object()) {
			std::string name = it.key();
			auto model = std::make_shared<CNBAModel>(name.c_str(), it.value());

			// check okay...
			model->parse();
			m_models.push_back(model);
		}
	}

}


