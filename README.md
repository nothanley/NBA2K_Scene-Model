# NBA2K Scene Reader

This is an early WIP C++ library designed to read, decode, and unpack `.scne` files from NBA2K25. This project is primarily used as a wrapper for interpreting the file structure of NBA2K scenes and models, making it accessible for Blender via Python.

### Prerequisites

- Visual Studio (recommended version supporting C++14)
- C++14 compatible compiler
- Blender (for usage with Blender/Python integration)

### Building the Project

1. Clone the repository.
2. Open the provided Visual Studio solution.
3. Ensure that your environment is set to build with **C++14**.
4. Configure the target platform (e.g., DLL_Library x64).
5. Build the project to generate the required module or `.dll`.

### Integration with Blender

After building the project, you can use CTypes to create Python bindings that allow Blender to read NBA2K scene files through this library.
