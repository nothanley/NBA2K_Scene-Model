import time
from .wrapper                     import ExternalLibary
from ..Blender.blender_save       import *
from ..Serialize.model_serialize  import *

def get_obj_selection():
    selection_names = bpy.context.selected_objects
    selection_ = []
    
    for sel in selection_names:
        if sel.type == 'MESH':
            selection_.append( sel )
            
    return selection_

def handle_context():
    objs = get_obj_selection()
    if len(objs) == 0: return False

    print("[MDL Addon] invalid object context. attempting resolve...")
    bpy.context.view_layer.objects.active = objs[0]
    bpy.ops.object.mode_set(mode='OBJECT')
    return True

def is_valid_context():
    try:
        bpy.ops.object.mode_set(mode='OBJECT')
    except:
        return handle_context()
    return True

def saveSkinModel(path, lib, args):
    # Check if user has selected any objects
    if (not is_valid_context()): return

    # Collect all skinmeshes from active scene (user selection)
    scene_meshes = getSceneMeshes()
    if (not scene_meshes): return

    # Interface with blender and send all model data to dll
    # todo: get armature ...
    cskinmodel     = CModelSerializer.to_cmodel(scene_meshes)

    # Save model to disk file
    cskinmodel.save_to_file(path)
    cskinmodel.free()
    return

def saveModelFile(filepath, args):
    # Begin timer
    start_time = time.time()

    # Call save function...
    saveSkinModel(filepath, ExternalLibary(), args)

   # End timer and print elapsed time
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Elapsed time: {elapsed_time} seconds")
    return
