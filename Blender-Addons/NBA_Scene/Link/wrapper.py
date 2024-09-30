import ctypes
import os
global lib

def GetDLLPath():
    pyPath = os.path.abspath(__file__)
    dllPath = os.path.join(os.path.dirname(pyPath), "nbamodel.dll")
    return dllPath

class cmesh():
    global lib

    @staticmethod
    def new():
        return cmesh()
    
    def __build_cmesh(self):
        lib.getNewSkinMesh.restype = ctypes.c_void_p
        return lib.getNewSkinMesh()
    
    def set_data(self, vertices, index_list):
        lib.setMeshData.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]

        # Convert python list to c array
        verts     = (ctypes.c_float * len(vertices))(*vertices)
        face_list = (ctypes.c_int   * len(index_list))(*index_list)

        return lib.setMeshData(self.__struct, verts, face_list, len(vertices), len(index_list))

    def set_skin_data(self, skin):
        if (skin == None):
            return
        
        lib.setMeshSkinData.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int]
        
        # Convert python list to c array
        cindices = (ctypes.c_int   * len(skin.blendindices))(*skin.blendindices)
        cweights = (ctypes.c_float * len(skin.blendweights))(*skin.blendweights)

        # Send data to cmesh struct
        return lib.setMeshSkinData(self.__struct, cindices, cweights, len(cindices), skin.weights_per_vertex)
    
    
    def set_material(self, material_name):
        lib.setMeshMaterial.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        return lib.setMeshMaterial(self.__struct, material_name.encode("utf-8"))

    def add_texture(self, texture):
        if not texture: return

        lib.setMaterialTexture.argtypes = [ctypes.c_void_p, 
                                           ctypes.c_char_p,
                                           ctypes.c_char_p, 
                                           ctypes.c_int, 
                                           ctypes.c_int, 
                                           ctypes.c_int, 
                                           ctypes.POINTER(ctypes.c_float)]
        
        name   = texture.name
        type   = texture.type
        width  = texture.width
        height = texture.height
        size   = width * height * 4
        pixmap = texture.get_pixel_buffer()
        
        return lib.setMaterialTexture(self.__struct, name.encode("utf-8"), type.encode("utf-8"), width, height, size, pixmap)

    def set_normals(self, normals):
        lib.setMeshNormals.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int]

        # Convert python list to c array
        cnorms = (ctypes.c_float * len(normals))(*normals)

        # Send data to cmesh struct
        return lib.setMeshNormals(self.__struct, cnorms, len(cnorms))
    
    def add_uv_channel(self, texcoords):
        lib.addUvMap.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int]

        # Convert python list to c array
        ctexcoords = (ctypes.c_float * len(texcoords))(*texcoords)

        # Send data to cmesh struct
        return lib.addUvMap(self.__struct, ctexcoords, len(ctexcoords))
    
    def add_vertex_colors(self, color_map):
        if (color_map == None): return
        lib.addMeshColorMap.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int]

        # Convert python list to c array
        cmap = (ctypes.c_float * len(color_map))(*color_map)

        # Send data to cmesh struct
        return lib.addMeshColorMap(self.__struct, cmap, len(cmap))
    
    def set_skin_data(self, skin):
        if (skin == None):
            return
        
        lib.setMeshSkinData.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int]
        
        # Convert python list to c array
        cindices = (ctypes.c_int   * len(skin.blendindices))(*skin.blendindices)
        cweights = (ctypes.c_float * len(skin.blendweights))(*skin.blendweights)

        # Send data to cmesh struct
        return lib.setMeshSkinData(self.__struct, cindices, cweights, len(cindices), skin.weights_per_vertex)
    
    def set_name_info(self, mesh_name, material_name):
        lib.setMeshNameInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]
        lib.setMeshNameInfo(self.__struct, mesh_name.encode("utf-8"), material_name.encode("utf-8"))
        return
    
    def set_model_game_flags(self, scene_flag, motion_flag):
        lib.setModelGameFlags.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
        lib.setModelGameFlags(self.__struct, scene_flag, motion_flag)
        return

    def add_blendshape(self, shape):
        if (not shape):
            return

        num_coords = len(shape.verts)
        cvertices = (ctypes.c_float * num_coords)(*shape.verts)

        lib.addBlendShape.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int]
        return lib.addBlendShape(self.__struct, shape.name.encode("utf-8"), cvertices, num_coords)

    def set_blendshapes(self, shapes):
        if (not shapes): return
        for shape in shapes:
            self.add_blendshape(shape)
        return
    
    def get_cstruct(self):
        return self.__struct
    
    def free(self):
        lib.freeMesh.argtypes = [ctypes.c_void_p]
        lib.freeMesh(self.__struct)
        return
    
    def __init__(self):
        self.__struct = self.__build_cmesh()

class cscnelib():
    global lib

    @staticmethod
    def getFileStatus(file_path):
        
        lib = ctypes.CDLL( GetDLLPath() )
        
        lib.getFileStatus.restype  = ctypes.c_bool
        lib.getFileStatus.argtypes = [ctypes.c_char_p]
        return lib.getFileStatus(file_path.encode('utf-8'))
    
    @staticmethod
    def updateFileMesh(file_path, id, vertices, texcoords, normals, num_verts, num_tris, search_method):

        lib = ctypes.CDLL( GetDLLPath() )

        lib.updateMeshData.restype  = ctypes.c_bool
        lib.updateMeshData.argtypes = [ctypes.c_char_p, # file path
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

        return lib.updateMeshData(
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
    def saveModelToFile(cmodel, path):
        lib.saveModelToFile.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        return lib.saveModelToFile(cmodel, path.encode("utf-8"))

    @staticmethod
    def free_model(cmodelfile):
        lib.release_model.argtypes = [ctypes.c_void_p]
        return lib.release_model(cmodelfile)
    
    @staticmethod
    def free_scene(cmodelfile):
        lib.release_scene.argtypes = [ctypes.c_void_p]
        return lib.release_scene(cmodelfile)
    
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

    @staticmethod
    def addMeshToModel(cskinmodel, cskinmesh):
        lib.linkMeshToModel.argtypes  = [ctypes.c_void_p, ctypes.c_void_p]
        return lib.linkMeshToModel(cskinmodel, cskinmesh)
    
    @staticmethod
    def getNumBones(cskinmodel):
        lib.getNumBones.restype  = ctypes.c_int
        lib.getNumBones.argtypes = [ctypes.c_void_p]
        return lib.getNumBones(cskinmodel)

    @staticmethod
    def getBoneParentIndex(cskinmodel, joint_index):
        lib.getBoneParentIndex.restype  = ctypes.c_int
        lib.getBoneParentIndex.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getBoneParentIndex(cskinmodel, joint_index)

    @staticmethod
    def getBoneMatrix(cskinmodel, joint_index):
        lib.getBoneMatrix.restype  = ctypes.POINTER(ctypes.c_float)
        lib.getBoneMatrix.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getBoneMatrix(cskinmodel, joint_index)

    @staticmethod
    def getBoneName(cskinmodel, joint_index):
        lib.getBoneName.restype  = ctypes.c_char_p
        lib.getBoneName.argtypes = [ctypes.c_void_p, ctypes.c_int]
        return lib.getBoneName(cskinmodel, joint_index)

    @staticmethod
    def getSkinData(cskinmodel, mesh_index):
        lib.getSkinData.restype  = ctypes.c_void_p
        lib.getSkinData.argtypes = [ctypes.c_void_p, ctypes.c_int] 
        return lib.getSkinData(cskinmodel, mesh_index)

    @staticmethod
    def getAllSkinGroups(cskinmodel, num_bones_p):
        lib.getAllSkinGroups.restype  = ctypes.POINTER(ctypes.c_char_p)
        lib.getAllSkinGroups.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)] 
        return lib.getAllSkinGroups(cskinmodel, num_bones_p)

    @staticmethod
    def getAllJointWeights(cskinmodel, bone_name, size_p):
        lib.getAllJointWeights.restype  = ctypes.POINTER(ctypes.c_float)
        lib.getAllJointWeights.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int)] 
        return lib.getAllJointWeights(cskinmodel, bone_name, size_p)

    @staticmethod
    def getMaterialFaceGroup(cskinmodel, mesh_index, group_index, face_begin_p, face_end_p):
        lib.getMaterialFaceGroup.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)] 
        return lib.getMaterialFaceGroup(cskinmodel, mesh_index, group_index, face_begin_p, face_end_p)
    
    @staticmethod
    def getAllFaceGroups(cskinmodel, mesh_index, size_p):
        lib.getAllFaceGroups.restype  = ctypes.POINTER(ctypes.c_char_p)
        lib.getAllFaceGroups.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.POINTER(ctypes.c_int)] 
        return lib.getAllFaceGroups(cskinmodel, mesh_index, size_p)

    @staticmethod
    def addBoneToModel(cskinmodel, bone_name, matrix, index, parent_name ):
        lib.setNewModelBone.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_char_p]

        # Convert python list to c array
        parent_name = "" if not parent_name else parent_name
        cmatrices = (ctypes.c_float * 4)(*matrix)
        return lib.setNewModelBone(cskinmodel, bone_name.encode("utf-8"), cmatrices, index, parent_name.encode("utf-8"))


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
        self.load_scene           = self.getLoadOperator()
        self.release_scene_file   = self.getDeleteOperator()


