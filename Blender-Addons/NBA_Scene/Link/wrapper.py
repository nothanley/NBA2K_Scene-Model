import ctypes
import os
global lib

def GetDLLPath():
    pyPath = os.path.abspath(__file__)
    dllPath = os.path.join(os.path.dirname(pyPath), "nbamodel.dll")
    return dllPath

class cmodellib():

    global lib

    @staticmethod
    def free_cskinmodel(cmodelfile):
        lib.release_model.argtypes = [ctypes.c_void_p]
        return lib.release_model(cmodelfile)

    @staticmethod
    def free_float_array(cfloat_arr):
        lib.freeMemory_float32.argtypes = [ctypes.POINTER(ctypes.c_float)] 
        lib.freeMemory_float32(cfloat_arr)
        return
    
    @staticmethod
    def free_int_arr(cint_arr):
        lib.freeMemory_intArr.argtypes = [ctypes.POINTER(ctypes.c_int)] 
        lib.freeMemory_intArr(cint_arr)
        return
    
    @staticmethod
    def free_string_arr(ccharp_arr):
        lib.freeMemory_charArrPtr.argtypes = [ctypes.POINTER(ctypes.c_char_p)]
        lib.freeMemory_charArrPtr(ccharp_arr)
        return
    
    @staticmethod
    def free_cskin(cskin_ptr):
        lib.freeMemory_skinData.argtypes = [ctypes.c_void_p]
        lib.freeMemory_skinData(cskin_ptr)
        return
    
    @staticmethod
    def getNumMeshes(cskinmodel):
        lib.getMeshTotal.restype  = ctypes.c_int
        lib.getMeshTotal.argtypes = [ctypes.c_void_p]
        return lib.getMeshTotal(cskinmodel)
    
    @staticmethod
    def getNewSkinModel():
        lib.getNewSkinModel.restype  = ctypes.c_void_p
        return lib.getNewSkinModel()
        
    @staticmethod
    def getMeshName(cskinmodel, mesh_index):
        lib.getMeshName.restype  = ctypes.c_char_p
        lib.getMeshName.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getMeshName(cskinmodel, mesh_index)

    @staticmethod
    def getNumTriangles(cskinmodel, mesh_index):
        lib.getNumTriangles.restype  = ctypes.c_int
        lib.getNumTriangles.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getNumTriangles(cskinmodel, mesh_index)

    @staticmethod
    def getNumVerts(cskinmodel, mesh_index):
        lib.getNumVerts.restype  = ctypes.c_int
        lib.getNumVerts.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getNumVerts(cskinmodel, mesh_index)

    @staticmethod
    def getMeshTriangleList(cskinmodel, mesh_index):
        lib.getMeshTriangleList.restype  = ctypes.POINTER(ctypes.c_uint32)
        lib.getMeshTriangleList.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getMeshTriangleList(cskinmodel, mesh_index)

    @staticmethod
    def getMeshNormals(cskinmodel, mesh_index):
        lib.getMeshNormals.restype  = ctypes.POINTER(ctypes.c_float)
        lib.getMeshNormals.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getMeshNormals(cskinmodel, mesh_index)

    @staticmethod
    def getVertexData(cskinmodel, mesh_index):
        lib.getVertexData.restype  = ctypes.POINTER(ctypes.c_float)
        lib.getVertexData.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getVertexData(cskinmodel, mesh_index)

    @staticmethod
    def getMaterialName(cskinmodel, mesh_index):
        lib.getMaterialName.restype  = ctypes.c_char_p
        lib.getMaterialName.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getMaterialName(cskinmodel, mesh_index)

    @staticmethod
    def getMeshUvChannel(cskinmodel, mesh_index, map_index):
        lib.getMeshUvChannel.restype  = ctypes.POINTER(ctypes.c_float)
        lib.getMeshUvChannel.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int] 
        return lib.getMeshUvChannel(cskinmodel, mesh_index, map_index)

    @staticmethod
    def getNumUvChannels(cskinmodel, mesh_index):
        lib.getNumUvChannels.restype  = ctypes.c_int
        lib.getNumUvChannels.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getNumUvChannels(cskinmodel, mesh_index)

class ExternalLibary():
    def getLoadOperator(self):
        # Defines Call for 'void* loadModelFile(const char* filePath, CNBAScene* pScene)'
        self.dynamic_lib.loadModelFile.restype  = ctypes.c_void_p
        self.dynamic_lib.loadModelFile.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p)]
        return self.dynamic_lib.loadModelFile
    
    def getDeleteOperator(self):
        # Defines Call for 'void release_model_file(void* pModelFile)'
        self.dynamic_lib.release_model_file.argtypes = [ctypes.c_void_p]
        return self.dynamic_lib.release_model_file

    def __init__(self):
        # Define global library handle
        global lib
        lib = ctypes.CDLL( GetDLLPath() )
        
        self.dynamic_lib          = lib
        self.load_model           = self.getLoadOperator()
        self.release_model_file   = self.getDeleteOperator()

