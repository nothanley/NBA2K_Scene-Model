from ..Link.wrapper            import cmodellib
from ..SkinModel.skinmesh      import vCSkinMesh

class cNBASkinModel():
    
    def getNumMeshes(self):
        return len(self.__meshes)
    
    def getMeshes(self):
        return self.__meshes

    def __getNumMeshes(self):
        return cmodellib.getNumMeshes(self.__data)

    def __setup(self):
        total_meshes = self.__getNumMeshes()

        for i in range(total_meshes):
            mesh = vCSkinMesh(self.__data, i)
            self.__meshes.append(mesh)

        return
    
    def __init__(self, data=None):
        self.__data   = data
        self.__meshes = []
        
        if (self.__data != None):
            self.__setup()
        


    