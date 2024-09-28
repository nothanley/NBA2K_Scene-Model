from ..Link.wrapper            import cmodellib
from ..SkinModel.skinmesh      import vCSkinMesh
from ..SkinModel.skinrig       import vCSkeleton

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

    def getSkeleton(self):
        return self.__skeleton
    
    def has_skeleton(self):
        skeleton = self.getSkeleton()
        return (len(skeleton.bones) != 0)
    
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

        self.__skeleton = vCSkeleton(self.__data) # Gather all rig/joint data
        return
    
    def __init__(self, data=None, index=None):
        self.__index  = index
        self.__data   = data
        self.__skeleton = None
        self.__meshes = []
        
        if (self.__data != None):
            self.__setup()
        


    