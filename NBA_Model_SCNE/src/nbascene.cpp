#include <common.h>
#include <nbamodel.h>
#include <nbascene.h>
#include <meshstructs.h>

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
				readMaterials(ele.value());
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

static Material loadMaterial(JSON& obj)
{
	Material mtl;

	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			auto key = common::chash(it.key());
			auto val = it.value();

			switch (key)
			{
			case enMaterialTag::COLOR_MAP:
				mtl.color_map = val["Pixelmap"];
				break;
			case enMaterialTag::NORMAL_MAP:
				mtl.normal_map = val["Pixelmap"];
				break;
			case enMaterialTag::ROUGH_MAP:
				mtl.roughness_map = val["Pixelmap"];
				break;
			default:
				break;
			};
		}
	}

	return mtl;
}

void CNBAScene::readMaterials(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			auto mtl = ::loadMaterial(it.value());
			mtl.name = it.key();
			m_materials.push_back(mtl);
		}
	}
}


