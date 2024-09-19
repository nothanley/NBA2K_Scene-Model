/* DLL Exports for usage and interface with Blender and python api */

#include <dll/interface_mesh.h>
#pragma once

/* Interface methods for saving or updating .SCNE data */

extern "C" __declspec(dllexport) bool getFileStatus(const char* path);

extern "C" __declspec(dllexport) bool updateScneFileMeshData(
    const char* path, const char* id, float* position, float* texcoords, float* normals, const int numVerts, const int numFaces, const int search_method);





/* .CPP Definitions - MOVE ME */

#include <sceneupdate.h>
#include <nbascene>
#include <vector>

bool getFileStatus(const char* path)
{
    USE_DEBUG_LOGS   = false;
    INCLUDE_LODS     = false;
    MERGE_MESH_PRIMS = true;

    /* Try to load file and contents */
    try
    {
        CSceneFile file(path);
        file.load();

        if (file.scene()->empty())
            return false;

        printf("\n[CNBAInterface] Found total models: %d", file.scene()->getNumModels());
        return true;
    }
    catch (...) {}

    printf("[CNBAScene] Failed to read file.\n");
    return false;
};

bool updateScneFileMeshData(
    const char* path,
    const char* id,
    float* position,
    float* texcoords,
    float* normals,
    const int numVerts,
    const int numFaces,
    const int search_method)
{
    USE_DEBUG_LOGS = false;
    INCLUDE_LODS   = false;
    MERGE_MESH_PRIMS = true;

    /* Try to load file and contents */
    try
    {
        CSceneUpdate file(path, true);
        file.load();

        if (file.scene()->empty())
            return false;

        StUpdatePkg clientPkg{ id, position, texcoords, normals, numVerts, numFaces, search_method };
        file.update(&clientPkg);
        return true;
    }
    catch (...) {}

    printf("[CNBAScene] Failed to read file.\n");
    return false;
};


/* .CPP END */



