import bpy
import ctypes
from ...Link.wrapper import ExternalLibary, cscnelib
from ...SkinModel.skinmodel import CNBAScene, cNBASkinModel
from ..blender_save import *
from ..blender_scene import *

class CMeshLink():

    @staticmethod
    def is_vertex_match(game_mesh, blend_mesh):
        game_vtx_count  = len(game_mesh.vertices)
        blend_vtx_count = len(blend_mesh.vertices)

        return (game_vtx_count == blend_vtx_count)

    @staticmethod
    def is_triangle_match(game_mesh, blend_mesh):
        blend_mesh.calc_loop_triangles()  # Calculate triangle data if not already available

        blend_tri_count = len(blend_mesh.loop_triangles)
        game_tri_count  = len(game_mesh.index_list)

        return (blend_tri_count == game_tri_count)
    
    @staticmethod
    def is_mesh_match(source_mesh, game_mesh):
        has_matching_verts = CMeshLink.is_vertex_match(game_mesh, source_mesh)
        has_matching_faces = CMeshLink.is_triangle_match(game_mesh, source_mesh)
        return has_matching_verts and has_matching_faces
    
    @staticmethod
    def find_object(object_id):
        return bpy.data.objects.get(object_id, None)
    
    def load_mesh(self, context):
        collection = getNewSceneCollection(self.name)
        object     = loadSkinMesh(self.target_mesh, None, self.name)
        collection.objects.link(object)
        return True
    
    def send_update(self, context, path):
        # Retrieve source object
        blender_obj = self.find_object(self.blender_obj_id)

        # Validate scene source
        if not blender_obj:
            print(f"[NSLink] Blender mesh '{self.blender_obj_id}' was deleted or destroyed. Failed to update.")
            return False
        
        if not self.is_mesh_match(blender_obj.data, self.target_mesh):
            print(f"[NSLink] Blender mesh '{self.blender_obj_id}' has mismatching triangles and vertices. Failed to update.")
            return False
        
        # Define Mesh Data
        mesh_name     = self.target_mesh.name
        verts         = getMeshVertices(blender_obj)
        texcoords     = verts # getTexCoords(self.blender_obj)
        norms         = getVertexNorms(blender_obj)
        num_tris      = len(self.target_mesh.index_list)
        search_method = int(context.scene.ns_inject_mode)
        print(f"[NSLink] Injecting Blender mesh '{self.blender_obj_id}' ...")

        # Perform scene json update
        return cscnelib.updateFileMesh(path, mesh_name, verts, texcoords, norms, len(verts), num_tris, search_method)
    
    def __init__(self):
        self.name              = None
        self.target_mesh       = None
        self.blender_obj_id    = None
        self.index             = -1
        return

class CLinkBuilder():
    @staticmethod
    def find_links(game_meshes, blender_meshes):
        # Scan Scene for target/source meshes
        return []
    
    @staticmethod
    def get_blender_objs():
        return  [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
    
    def search_link(self, game_mesh, objs):

        for obj in objs:
            bmesh    = obj.data
            new_link = None

            if CMeshLink.is_mesh_match(bmesh, game_mesh):
                new_link                 = CMeshLink()
                new_link.blender_obj_id  = obj.name
                new_link.target_mesh     = game_mesh
                new_link.name            = game_mesh.name
                return new_link
            
        return None
    
    def scan_meshes(self, game_scene_p):  
        # Initialize Scene
        scene  = CNBAScene(game_scene_p)
        models = [None] * scene.num_models
        objs   = self.get_blender_objs()

        # Load all sub meshes
        for i in range(scene.num_models):
            models[i] = cNBASkinModel(game_scene_p, i)

            for mesh in models[i].getMeshes():
                self.game_meshes.append(mesh)

        # Find mesh matches
        for mesh in self.game_meshes:
            new_link = self.search_link(mesh, objs)

            if new_link:
                new_link.index = self.ns_link.num_links()
                self.ns_link.links.append(new_link)

            self.ns_link.push_mesh(mesh)
        return
    
    def run(self): 
        module = ExternalLibary()

        scene_file = ctypes.c_void_p()
        nba_scene = module.load_scene(
            self.file_path.encode('utf-8'), 
            ctypes.byref(scene_file), 
            False,
            False)
    
        if (not scene_file or not nba_scene): 
            print("[NSLink] Failed to load .SCNE file")
        else:
            self.scan_meshes(nba_scene)
            module.release_scene_file(scene_file)  # clean model + file heap
            print("[NSLink] Scene read success.")
        return
    
    def __init__(self, link, path):
        self.file_path   = path
        self.ns_link     = link
        self.game_meshes = []

class CSceneLink():
    
    def reset(self):
        # Set/Clear all attributes to defaults
        self.__file_path   = str()
        self.__game_meshes = []
        self.links         = []
        return
    
    def load_mesh(self, index):

        target_mesh = self.get_mesh(index)
        if not target_mesh: return False
        
        collection = getNewSceneCollection(target_mesh.name)
        object     = loadSkinMesh(target_mesh, None, target_mesh.name)
        collection.objects.link(object)

        return True
    
    def push_mesh(self, mesh):
        self.__game_meshes.append(mesh)
        return
    
    def get_mesh(self, index):
        if index > self.num_meshes():
            return None
        
        return self.__game_meshes[index]
    
    def get_game_meshes(self):
        if self.num_meshes == 0:
            return None
        
        return self.__game_meshes

    def get_link(self, index):
        if index > self.num_links():
            return None
        
        return self.links[index]
    
    def num_meshes(self):
        return len(self.__game_meshes)
    
    def num_links(self):
        return len(self.links)

    def ready(self):
        # Check Link status
        return (self.__file_path)
    
    def load_file(self):
        # clear data
        self.links.clear()

        # Parse all meshes - find all links
        link_builder = CLinkBuilder(self, self.path())
        link_builder.run()
        return
    
    def path(self):
        # Returns file location
        return self.__file_path
    
    def setup(self, path):
        # Try and get .scne interface link - update attributes
        self.reset()
        self.__file_path = path
        
        if not self.ready(): return
        
        self.load_file()
        print("[NSLink] Link requested and created.")
        return
    
    def __init__(self, path=None):
        self.reset()
        if path: self.setup(path)

        return

