#include <scenereader.h>
#include <modelreader.h>
#include <common.h>


CSceneReader::CSceneReader(const char* id, JSON& json)
	:
	CNBAScene(id),
	m_json(json)
{
}

void CSceneReader::parse()
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

void CSceneReader::readModels(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object()) {
			std::string name = it.key();
			auto model = std::make_shared<CModelReader>(name.c_str(), it.value());

			// check okay...
			model->parse();
			m_models.push_back(model);
		}
	}

}

void CSceneReader::loadMaterial(JSON& obj)
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
	
	// todo: finish material reader and push to scene ...
	return;
}

void CSceneReader::readMaterials(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_object())
		{
			loadMaterial(it.value());
			//mtl.name = it.key();
		}
	}
}


