/* DLL Exports for usage and interface with Blender and python api */

#include <dll/interface_mesh.h>
#pragma once

/* Interface methods for saving or updating .SCNE data */

extern "C" __declspec(dllexport) bool getFileStatus(const char* path);

extern "C" __declspec(dllexport) bool updateScneFileMeshData(
    const char* path, const char* id, float* position, float* texcoords, float* normals, const int numVerts, const int numFaces, const int search_method);
