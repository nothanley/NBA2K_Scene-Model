import ctypes
import os
global lib

def GetDLLPath():
    pyPath = os.path.abspath(__file__)
    dllPath = os.path.join(os.path.dirname(pyPath), "nbamodel.dll")
    return dllPath

class cscnelib():
    global lib

    @staticmethod
    def getFileStatus(file_path):
        lib.getFileStatus.restype  = ctypes.c_bool
        lib.getFileStatus.argtypes = [ctypes.c_char_p]
        return lib.getFileStatus(file_path.encode('utf-8'))
    
    @staticmethod
    def updateFileMesh(file_path, id, vertices, texcoords, normals, num_verts, num_tris, search_method):
        lib.updateScneFileMeshData.restype  = ctypes.c_bool
        lib.updateScneFileMeshData.argtypes = [ctypes.c_char_p, # file path
                                               ctypes.c_char_p, # mesh id
                                               ctypes.POINTER(ctypes.c_float), # position input
                                               ctypes.POINTER(ctypes.c_float), # texcoords input
                                               ctypes.POINTER(ctypes.c_float), # normals input
                                               ctypes.c_int, # num verts
                                               ctypes.c_int, # num tris
                                               ctypes.c_int  # enum search method
                                               ]
        
        # Convert python list to c array
        c_verts     = (ctypes.c_float * num_verts)(*vertices)
        c_texcoords = (ctypes.c_float * num_verts)(*texcoords)
        c_norms     = (ctypes.c_float * num_verts)(*normals)

        return lib.updateScneFileMeshData(
            file_path.encode('utf-8'),
            id.encode('utf-8'),
            c_verts, 
            c_texcoords,
            c_norms,
            num_verts, 
            num_tris,
            search_method)
    
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
    def getNumModels(cscenefile):
        lib.getModelTotal.restype  = ctypes.c_int
        lib.getModelTotal.argtypes = [ctypes.c_void_p]
        return lib.getModelTotal(cscenefile)
    
    @staticmethod
    def getSceneModel(cscenefile, model_index):
        lib.getSceneModel.restype  = ctypes.c_void_p
        lib.getSceneModel.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getSceneModel(cscenefile, model_index)
    
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
        # Defines Call for 'void* loadModelFile(const char* filePath, CNBAScene* pScene, bool use_lods, bool split_groups)'
        self.dynamic_lib.loadModelFile.restype  = ctypes.c_void_p
        self.dynamic_lib.loadModelFile.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p), ctypes.c_bool, ctypes.c_bool]
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
        self.load_scene           = self.getLoadOperator()
        self.release_scene_file   = self.getDeleteOperator()

#     @staticmethod
#     def loadSCNEFile(file_path, pScene, use_lods, split_groups):
#         # Defines Call for 'void* loadModelFile(const char* filePath, CNBAScene* pScene, bool use_lods, bool split_groups)'
#         lib.loadModelFile.restype  = ctypes.c_void_p
#         lib.loadModelFile.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p), ctypes.c_bool, ctypes.c_bool]
#         return lib.loadModelFile(file_path, pScene, use_lods, split_groups)
    
#     @staticmethod
#     def freeSCNEModel(cskinmodel):
#         # Defines Call for 'void release_model_file(void* pModelFile)'
#         lib.release_model_file.argtypes = [ctypes.c_void_p]
#         return lib.release_model_file(cskinmodel)