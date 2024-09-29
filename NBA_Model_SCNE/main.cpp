// Debug reader usage - modify path to test .scne file io

#include <scenefile.h>
#include <nbascene.h>
#include <nbamodel.h>
#include <sceneupdate.h>

#include <iostream>
#include <memory>



int main()
{
	USE_DEBUG_LOGS = true;

	//run_material_reader();
	run_loader_debug();
	//run_update_debug();
	//run_model_serialize();
	
	printf("\n\n[DEBUG-MAIN] Finished debug reader...");
}


