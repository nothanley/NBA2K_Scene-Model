#include <string>
#include <scenereader.h>
#include <nbascene>
#pragma once

class CSceneSerializer
{
public:
	CSceneSerializer(std::shared_ptr<CNBAScene>& scene);

public:
	void setScene(std::shared_ptr<CNBAScene>& scene);

private:
	std::shared_ptr<CNBAScene> m_scene;
};


