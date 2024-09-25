// Debug reader usage - modify path to test .scne file io

#include <scenefile.h>
#include <nbascene.h>
#include <nbamodel.h>
#include <sceneupdate.h>

#include <iostream>
#include <memory>


void run_loader_debug()
{
	// Open and load a scene file given a local path.
	CSceneFile inFile(
		//"C:/SteamLibrary/steamapps/common/NBA 2K25/mods/char/sig/hihead.scne" );
		//"C:/Users/wauke/Downloads/NBA 2K25 Research/mods/props/appliance_boombox_generic-2/appliance_boombox_generic.SCNE");
		//"C:/Users/wauke/Downloads/NBA 2K25 Research/mods/balls/ball.SCNE");
		"C:/Users/wauke/Desktop/NBA2K_OUT_DEV/nba_test.scne");

	// Parse all scene data from file path
	inFile.load();

	// Access CNBAScene and CNBAModel children objects.
	auto scene = inFile.scene();

	// test usage - iter through models and meshes
	for (auto& model : scene->models())
		auto mesh = model->getMesh();

	printf("");
};

void run_update_debug()
{
	CSceneUpdate file("C:/Users/wauke/Downloads/NBA 2K25 Research/mods/balls/ball.SCNE", true);
	file.load();

	if (file.scene()->empty())
		return;

	auto mesh = file.scene()->models().front()->getMesh();

	StUpdatePkg clientPkg
	{
		mesh->name,
		mesh->vertices.data(),
		mesh->uvs.front().map.data(),
		mesh->normals.data(),
		mesh->vertices.size(),
		mesh->triangles.size(),
		1 // ENUM VAL - VTX ID = 1
	};

	file.update(&clientPkg);
};


#include <cereal/sceneserializer.h>
void run_model_serialize()
{
	CSceneUpdate file("C:/Users/wauke/Downloads/NBA 2K25 Research/mods/balls/ball.SCNE", true);
	file.load();

	if (file.scene()->empty()) 
		return;

	// Serialize model to file
	auto model = file.scene()->models().front();
	auto scene = std::make_shared<CNBAScene>(model->name().c_str());

	// Create new scene - add data 
	scene->pushModel(*model.get());

	CSceneSerializer serializer(scene);
	serializer.save("C:/Users/wauke/Desktop/NBA2K_OUT_DEV/out_file.scne");
}

int main()
{
	USE_DEBUG_LOGS = true;

	run_loader_debug();
	//run_update_debug();
	//run_model_serialize();
	
	printf("\n\n[DEBUG-MAIN] Finished debug reader...");
}


