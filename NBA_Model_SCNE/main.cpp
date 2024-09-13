// Debug reader usage - modify path to test .scne file io

#include <scenefile.h>
#include <nbascene.h>
#include <nbamodel.h>

#include <iostream>
#include <memory>

int main()
{
	// Open and load a scene file given a local path.
	CSceneFile inFile(
		"C:/Users/wauke/Downloads/NBA 2K25 Research/mods/balls/ball.scne" );

	//CSceneFile inFile(
		//"C:/Users/wauke/Downloads/Player Model/png2020/hihead.scne");

	// Parse all scene data from file path
	inFile.load();

	// Access CNBAScene and CNBAModel children objects.
	auto scene  = inFile.scene();

	// test usage - iter through models and meshes
	for (auto& model : scene->models())
	{
		auto mesh = model->getMesh();
		//printf("\n[DebugMsh] Browsing mesh: %s", mesh.name.c_str());
	}

	printf("\n\n[DEBUG-MAIN] Finished debug reader...");
}

