/* DLL Exports for usage and interface with Blender and python api */

#include <cstring>
#include <NBAScene>
#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

// Class/Struct Initializer Funcs
DLL_EXPORT void* getNewSkinMesh(const char* name);
DLL_EXPORT void* getNewSkinModel(const char* name);
DLL_EXPORT void* getNewSceneObj(const char* name);


// Mesh Data Population Funcs



/* CPP Begin - MOVE ME */

void* getNewSkinMesh(const char* name)
{
	return new Mesh{ name };
}

void* getNewSkinModel(const char* name)
{
	return new CNBAModel(name);
}

void* getNewSceneObj(const char* name)
{
	return new CNBAScene(name);
}

/////////////////////////////

