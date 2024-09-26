import bpy
from .vcnbamodel_import import *
from .vcnbamodel_export import *
from .Blender.Panel.blender_panel import *

bl_info = {
    "name": "Visual Concepts | NBA (.SCNE) Import Add-On v0.50",
    "author": "Hanleys",
    "version": (0, 5, 0),
    "blender": (2, 93, 4),
    "description": "Import plugin for 2K (.scne) models.",
    "category": "Object",
}

def register():
    register_vcs_import()
    register_vcs_export()
    register_panel()

def unregister():
    unregister_vcs_import()
    unregister_vcs_export()
    unregister_panel()

if __name__ == "__main__":
    register()
    bpy.ops.import_vcnbamodel.load('INVOKE_DEFAULT')

