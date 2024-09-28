from ..Link.wrapper import cmodellib
import numpy as np
import ctypes

def getSkinData(cmodel, mesh_index):
    return cmodellib.getSkinData(cmodel, mesh_index)

def getAllSkinGroups(cskin):
    numBones = ctypes.c_int()
    data     = cmodellib.getAllSkinGroups(cskin, ctypes.byref(numBones))
    if (data == None): 
        return None
    
    # Convert the received data to a Python list of strings
    skin_groups = [ctypes.string_at(data[i]).decode("utf-8") for i in range(numBones.value)]

    # Free allocated memory
    cmodellib.free_string_arr(data)
    return skin_groups

def getAllSkinWeights(cskin, groups):
    if (groups == None): return None

    size = ctypes.c_int()
    weights = [float] * len(groups)

    for i, group in enumerate(groups):
        data = cmodellib.getAllJointWeights(cskin, group.encode('utf-8'), ctypes.byref(size) )
        weights[i] = np.ctypeslib.as_array(data, shape=(size.value,)).tolist()

        # Free allocated memory
        cmodellib.free_float_array(data)
    return weights

class vCMeshWeights():
    def __debugLog(self):
        for i, group in enumerate(self.groups):
            print("\n[MDL Addon] Vertex Group: ",   group)
            # print("\n[MDL Addon] Vertex Weights: ", self.weights[i])
        return
    
    def __setup(self):
        self.groups  = getAllSkinGroups(self.__data)
        self.weights = getAllSkinWeights(self.__data, self.groups)
        # self.__debugLog()
        return

    def __init__(self, data, mesh_index):
        self.__data  = getSkinData(data, mesh_index)
        self.groups  = None
        self.weights = None

        # Sets up skin only if the mesh has a valid struct ptr
        if (self.__data != None):
            self.__setup()