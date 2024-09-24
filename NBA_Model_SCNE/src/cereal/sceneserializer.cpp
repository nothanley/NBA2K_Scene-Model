#include <cereal/sceneserializer.h>


CSceneSerializer::CSceneSerializer(std::shared_ptr<CNBAScene>& scene)
	:
	m_scene(scene)
{
}


void CSceneSerializer::setScene(std::shared_ptr<CNBAScene>& scene)
{
	m_scene = scene;
}

