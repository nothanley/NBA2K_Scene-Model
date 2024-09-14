// SceneDependencyGrabber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "filescanner.h"
#include <iostream>
#include <common.h>

static void run_scanner_user_file(const char* path)
{
    try
    {
        CSceneFileScanner inFile(path);
        inFile.load();
        printf("\n[CSceneFileScanner] Scene scan complete.\n");
    }
    catch (...)
    {
        common::set_console_text_color(TEXT_RED_CON);
        printf("\n[File Scanner] Failed to process/scan .scne file.\n");
        common::set_console_text_color(TEXT_WHITE_CON);
    }
}

static void run_scanner_all_local_files()
{
    auto home_dir = common::get_exe_path();

    std::vector<std::string> scnePaths;
    scnePaths = common::findMatchingExtensionFiles(home_dir.c_str(), ".SCNE");

    if (scnePaths.empty())
    {
        printf("\n[CSceneFileScanner] No .scne files found in folder: (%s)\n", home_dir.c_str());
        return;
    }

    for (auto& path : scnePaths)
    {
        try
        {
            CSceneFileScanner inFile(path.c_str());
            inFile.load();
            printf("\n[CSceneFileScanner] Scene scan complete.\n");
        }
        catch (...)
        {
            common::set_console_text_color(TEXT_RED_CON);
            printf("\n[File Scanner] Failed to process/scan .scne file: (%s).\n", path.c_str());
            common::set_console_text_color(TEXT_WHITE_CON);
        }
    }

}


int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        // Run scanner on all .scne files in exe directory.
        run_scanner_all_local_files();
    }
    else
    {
        // Retrieve the file path from the command-line arguments
        std::string filePath = argv[1];

        // Run scanner on specified path
        run_scanner_user_file(argv[1]);
    }

    system("pause");
    return 1;
}

