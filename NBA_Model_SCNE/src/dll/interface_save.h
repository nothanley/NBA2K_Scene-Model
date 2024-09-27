/* DLL Exports for usage and interface with Blender and python api */
#include <dll/interface_mesh.h>
#pragma once

#define DLLEX extern "C" __declspec(dllexport)

/* Interface methods for saving or updating .SCNE data */
DLLEX bool getFileStatus(const char* path);
DLLEX bool updateMeshData(
    const char* path,
    const char* id, 
    float* position,
    float* texcoords,
    float* normals, 
    const int numVerts,
    const int numFaces,
    const int search_method);

