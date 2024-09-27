#include <cereal/textureserializer.h>
#include <cereal/sceneserializer.h>
#include <cereal/meshjson.h>
#include <common.h>
#include <texture/texture_compress.h>

CTextureSerializer::CTextureSerializer(CSceneSerializer* parent)
	:
	m_parent(parent)
{
}

std::shared_ptr<JSON> 
CTextureSerializer::serialize()
{
	auto scene = m_parent->scene();
	m_json->clear();

	// collect model childs
	for (auto& model : scene->models())
	{
		this->processModel(model);
	}

	return m_json;
}

void CTextureSerializer::processModel(const std::shared_ptr<CNBAModel>& model)
{
	for (auto& mesh : model->getMeshes())
	{
		this->processMesh(mesh);
	}
}

inline static std::string getTexSavePath(const std::string& scene_path, const std::shared_ptr<CNSTexture>& texture, const std::string& subdir)
{
	auto parent_dir = common::get_parent_directory(scene_path);
	auto pos        = parent_dir.find_last_of("/");
	auto dir        = parent_dir.substr(0, pos) + "/" + subdir;

	// create sub directory if not exists
	if (!common::create_folder(dir.c_str()))
		return "";

	return dir + "/" + texture->generateFileId();
}

std::shared_ptr<JSON> CTextureSerializer::getTextureJson(const std::shared_ptr<CNSTexture>& texture)
{
	auto json = std::make_shared<JSON>();

	// gpu compress texture
	size_t binarySize  = NULL;
	std::string subDir = "textures";
	auto savePath      = ::getTexSavePath(m_parent->path(), texture, subDir);
	if (!CTextureCompressor::compress(texture, savePath.c_str(), binarySize))
		return nullptr;

	// fill texture json
	(*json)["Width"]             = texture->width();
	(*json)["Height"]            = texture->height();
	(*json)["Mips"]              = texture->mips();
	(*json)["Format"]            = "BC3_UNORM";
	(*json)["Min"]               = std::vector<float>{0,0,0,1};
	(*json)["Max"]               = std::vector<float>{1,1,1,1};
	(*json)["RangeCompress"]     = 15;
	(*json)["PixelDataSize"]     = binarySize;
	(*json)["CompressionMethod"] = 8;
	(*json)["Binary"]            = subDir + "/" + texture->generateFileId();

	return json;
}

void CTextureSerializer::processMesh(const std::shared_ptr<Mesh>& mesh)
{
	auto& material = mesh->material;

	for (auto& texture : material.textures())
	{
		auto json = this->getTextureJson(texture);
		if (!json) continue;

		(*m_json)[texture->name()] = *json;
	}
}

