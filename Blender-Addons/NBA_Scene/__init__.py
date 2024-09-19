import bpy
from .vcnbamodel_import import *
from .Blender.Panel.blender_panel import *

bl_info = {
    "name": "Visual Concepts | NBA (.SCNE) Import Add-On (R&D) v0.20",
    "author": "Hanleys",
    "version": (0, 2, 0),
    "blender": (2, 93, 4),
    "description": "Import plugin for 2K (.scne) models.",
    "category": "Object",
}

def register():
    register_vcs_import()
    register_panel()

def unregister():
    unregister_vcs_import()
    unregister_panel()

if __name__ == "__main__":
    register()
    bpy.ops.import_vcnbamodel.load('INVOKE_DEFAULT')

