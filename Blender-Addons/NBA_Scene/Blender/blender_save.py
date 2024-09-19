import bpy
import re
from ..SkinModel.skinmodel import vCSkinMesh
from .blender_utils import *

def getMeshSelection():
    selection_objs = bpy.context.selected_objects
    selection = []
    
    for obj in selection_objs:
        if obj.type == 'MESH':
            selection.append( obj )
            
    return selection

def getBaseName(obj):
    name = obj.name
    name = name.replace( "!","")
    name = name.replace( "$","")
    if ( "." in  name ):
        name = name.split(".")[0]
    return name

def makeSkinMesh(obj):
    # Build skin mesh from scene object
    skinmesh = vCSkinMesh()
    skinmesh.name           = getBaseName(obj)
    skinmesh.mesh_name      = getBaseName(obj.data)
    skinmesh.index_list     = getMeshFaces(obj)
    skinmesh.vertices       = getMeshVertices(obj)
    skinmesh.vertex_normals = getVertexNorms(obj)
    skinmesh.texcoords      = getTexCoords(obj)
    # skinmesh.vertex_colors  = getVertexColors(obj)
    # skinmesh.skin           = getMeshSkin(obj)
    # skinmesh.blendshapes    = getShapeKeys(obj)
    # skinmesh.scene_flag     = getGameSceneFlag(obj)
    # skinmesh.motion_flag    = getGameMotionFlag(obj)
    
    return skinmesh

def getSceneMeshes():
    # Collect all user selection targets
    obj_selection = getMeshSelection()
    if ( len( obj_selection ) == 0 ):
        print( "\n[MDL Addon] Please select an object to save file." )
        return None

    # Preprocess all scene meshes to game-ready objs
    # format_game_objects(obj_selection)

    models = []
    # Read all scene object data to skinmesh obj
    for obj in obj_selection:
        # print( "[MDL Addon] Loading Object: " + obj.name )
        skinmesh = makeSkinMesh(obj)
        models.append(skinmesh)
    
    return models

def getSceneArmature():
    selection = getMeshSelection()
    bpy.context.view_layer.update()

    for obj in selection:
        if (has_armature_modifier(obj)):
            armature = obj.parent
            target_rig = vCSkeleton()

            # print("\n[MDL Addon] Loading Armature: ", armature.name)
            for bone in armature.data.bones:
                target_rig.bones.append( getRigBone(bone) )

            return target_rig
    
    return None
