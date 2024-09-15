from ..Link.wrapper            import cmodellib
from ..SkinModel.skinmesh      import vCSkinMesh

class CNBAScene():

    def __get_total_models(self):
        return cmodellib.getNumModels(self.__data)

    def __setup(self): 
        self.num_models = self.__get_total_models()
        return
    
    def __init__(self, data=None):
        self.__data     = data
        self.num_models = 0 
        
        if (self.__data != None):
            self.__setup()
        
class cNBASkinModel():
    
    def getNumMeshes(self):
        return len(self.__meshes)
    
    def getMeshes(self):
        return self.__meshes

    def __getCSceneModel(self):
        return cmodellib.getSceneModel(self.__data, self.__index)
    
    def __getNumMeshes(self):
        return cmodellib.getNumMeshes(self.__data)

    def __setup(self):
        self.__data  = self.__getCSceneModel()
        total_meshes = self.__getNumMeshes()

        for i in range(total_meshes):
            mesh = vCSkinMesh(self.__data, i)
            self.__meshes.append(mesh)

        return
    
    def __init__(self, data=None, index=None):
        self.__index  = index
        self.__data   = data
        self.__meshes = []
        
        if (self.__data != None):
            self.__setup()
        


    