import bpy # type: ignore
import os
from .blender_mesh import *
from .blender_skeleton import *
from math import radians

def getNewSceneCollection(name, link_to_scene=True):
    collection = bpy.data.collections.new(name)
    if (link_to_scene):
        bpy.context.scene.collection.children.link(collection)
    return collection

def splitAllMaterialGroups(obj):
    bpy.context.view_layer.objects.active = obj
    bpy.ops.object.mode_set( mode = 'EDIT' )
    bpy.ops.mesh.select_all( action = 'SELECT' )
    bpy.ops.mesh.separate( type = 'MATERIAL' )  
    bpy.ops.object.mode_set( mode = 'OBJECT' )

    for o in bpy.context.selected_objects:
        o.name = o.active_material.name

    bpy.context.active_object.select_set(False) # deselect active object
    return

def loadSkinModel(model, parent, args, model_path):
    # check mesh array size
    if model.getNumMeshes() == 0:
        return
    
    # construct skeleton
    if model.has_skeleton():
        armature_obj = loadSkeleton(parent, model.getSkeleton() )
    
    # construct all child meshes
    for mesh in model.getMeshes():
        object = loadSkinMesh(mesh, args, model_path)
        parent.objects.link(object)

        if ( model.has_skeleton() ): # Link to skeleton if model has armature
            object.parent   = armature_obj
            modifier        = object.modifiers.new(type='ARMATURE', name="Armature")
            modifier.object = armature_obj

        if args["split_groups"]:
            splitAllMaterialGroups(object)
    return


class cNBABlenderScene():
    
    def __frame(self):
        # Frames viewport mesh and sets up rendering settings 
        try:
            bpy.ops.view3d.view_all()
            bpy.context.space_data.shading.show_object_outline = False
            bpy.context.space_data.clip_start = 0.01
            bpy.context.space_data.clip_end   = 50000.0
        except:
            if ( bpy.app.version < (3,5,0) ):
                for screen in bpy.data.screens:
                    for area in (a for a in screen.areas if a.type == 'VIEW_3D'):
                        region = next((region for region in area.regions if region.type == 'WINDOW'), None)
                        if region is not None:
                            override = {'area': area, 'region': region} 
                            bpy.ops.view3d.view_all(override)
                            area.spaces.active.shading.show_object_outline = False
            pass
        return
    
    def build(self):
        # build unique collection for models in specified list
        for model in self.models:
            collection = getNewSceneCollection(self.scene_name)
            self.collections.append(collection)

            # Load model to blender scene context
            loadSkinModel(model, collection, self.__import_args, self.model_path)
        
        self.__frame()
        return

    def set_load_args(self, args):
        self.__import_args = args
        return
    
    def set_name(self, path):
        self.model_path = path
        self.scene_name = os.path.basename(path).split(".")[0]
        return
    
    def __init__(self, model_list):
        self.scene_name = ""
        self.model_path = ""
        self.collections = []
        self.__import_args = []
        self.models = model_list

