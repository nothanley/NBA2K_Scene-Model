import bpy
import ctypes
from ...Link.wrapper import ExternalLibary, cscnelib
from ...SkinModel.skinmodel import CNBAScene, cNBASkinModel
from ..blender_save import *
from ..blender_scene import *

class CMeshLink():

    def load_mesh(self, context):
        collection = getNewSceneCollection(self.name)
        object = loadSkinMesh(self.target_mesh, None, self.name)
        collection.objects.link(object)
        return True
    
    def send_update(self, context, path):

        # Validate scene source
        if not self.blender_mesh:
            print("\n[NSLink] Blender mesh was deleted or destroyed. Failed to update.")
            return False
        
        # Define Mesh Data
        mesh_name     = self.target_mesh.name
        verts         = getMeshVertices(self.blender_obj)
        texcoords     = verts # getTexCoords(self.blender_obj)
        norms         = getVertexNorms(self.blender_obj)
        num_tris      = len(self.target_mesh.index_list)
        search_method = int(context.scene.ns_inject_mode)

        # Perform scene json update
        return cscnelib.updateFileMesh(path, mesh_name, verts, texcoords, norms, len(verts), num_tris, search_method)
    
    def __init__(self):
        self.name         = None
        self.target_mesh  = None
        self.blender_mesh = None
        self.blender_obj  = None
        self.index        = -1
        return

class CLinkBuilder():
    @staticmethod
    def find_links(game_meshes, blender_meshes):
        # Scan Scene for target/source meshes
        return []
    
    @staticmethod
    def get_blender_objs():
        return  [obj for obj in bpy.context.scene.objects if obj.type == 'MESH']
    
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
    
    def search_link(self, game_mesh, objs):

        for obj in objs:
            bmesh    = obj.data
            new_link = None
            
            is_vtx_match = self.is_vertex_match(game_mesh, bmesh)
            is_tri_match = self.is_triangle_match(game_mesh, bmesh)

            # Compare VTX Data
            if is_vtx_match and is_tri_match:
                new_link = CMeshLink()
                new_link.blender_obj  = obj
                new_link.blender_mesh = bmesh
                new_link.target_mesh  = game_mesh
                new_link.name         = game_mesh.name
                return new_link
            
            # todo: compare uv's
            # ...

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
            result = self.search_link(mesh, objs)
            if result:
                print(f"\n[BPY-SCAN] Found Match: \nBLEND:{result.blender_mesh.name} \nGAME: {result.target_mesh.name}")
                result.index = self.ns_link.num_links()
                self.ns_link.links.append(result)
        return
    
    def run(self): 
        print("\n[LinkBuilder] Finding targets...")
        module = ExternalLibary()

        scene_file = ctypes.c_void_p()
        nba_scene = module.load_scene(
            self.file_path.encode('utf-8'), 
            ctypes.byref(scene_file), 
            False,
            False)
    
        if (not scene_file or not nba_scene): 
            print("\n[BPY-Debug] Failed to load .SCNE file")
        else:
            self.scan_meshes(nba_scene)
            module.release_scene_file(scene_file)  # clean model + file heap
            print("\n[BPY-Debug] Scene read success.")

        return
    
    def __init__(self, link, path):
        self.file_path   = path
        self.ns_link     = link
        self.game_meshes = []

class CSceneLink():
    
    def reset(self):
        # Set/Clear all attributes to defaults
        self.__file_path = str()
        self.links       = []
        return
    
    def get_link(self, index):
        if index > self.num_links():
            return None
        
        return self.links[index]
    
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
        self.__file_path = path
        
        if self.ready():
            self.load_file()

            print("\n[BPY-DEBUG] Link requested and created.")

        return
    
    def __init__(self, path=None):
        self.reset()

        if path:
            self.setup(path)


