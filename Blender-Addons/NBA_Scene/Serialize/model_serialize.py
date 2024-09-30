from ..Link.wrapper import cmesh, cmodellib

class CModelSerializer():

    @staticmethod
    def to_cmodel(skinmeshes):
        cmodel = CModelSerializer()
        cmodel.set_meshes(skinmeshes)
        cmodel.build()
        return cmodel

    def set_material_link(self, cskinmesh, material):        
        # set textures
        cskinmesh.set_material(material.name)
        cskinmesh.add_texture(material.color_map)
        cskinmesh.add_texture(material.normal_map)
        cskinmesh.add_texture(material.rdmo_map)
        return
    
    def build(self):
        # Link all child mesh data to new cmodel
        for mesh in self.meshes:
            cskinmesh = cmesh.new()
            cskinmesh.set_name_info(mesh.mesh_name, mesh.name)
            cskinmesh.set_data(mesh.vertices, mesh.index_list)
            cskinmesh.set_normals(mesh.vertex_normals)
            cskinmesh.set_skin_data(mesh.skin)
            
            for mat in mesh.materials:
                self.set_material_link(cskinmesh, mat)
            
            for map in mesh.texcoords:
                cskinmesh.add_uv_channel(map)

            self.__add_cmesh(cskinmesh) # parent model to new cmesh
        return
    
    def set_meshes(self, mesh_list):
        self.meshes = mesh_list
        return
    
    def set_skeleton(self, armature):
        if (armature == None): 
            return
        
        for index, bone in enumerate(armature.bones):
            cmodellib.addBoneToModel(self.__model, bone.name, bone.matrix, index, bone.parent)
        return
    
    def free(self):
        cmodellib.free_model(self.__model)
        self.__model = None
        return 
    
    def save_to_file(self, path):
        if (path == None):
            return False
        
        return cmodellib.saveModelToFile(self.__model, path)
    
    def __add_cmesh(self, cmesh):
        mesh_p = cmesh.get_cstruct()
        return cmodellib.addMeshToModel(self.__model, mesh_p)
    
    def __get_cmodel_p(self):
        return cmodellib.getNewSkinModel()

    def __init__(self):
        self.__model       = self.__get_cmodel_p()
        self.meshes        = []





