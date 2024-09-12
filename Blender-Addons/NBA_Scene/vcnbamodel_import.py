import bpy
from .Link.interface import *
from bpy.types import Operator
from bpy.props import StringProperty, BoolProperty
from bpy_extras.io_utils import ImportHelper

class vCNBAModelImport(Operator, ImportHelper):
    """Import Visual Concepts | NBA2K Model"""
    bl_idname = "import_vcnbamodel.load"
    bl_label = "Import SCNE"
    filename_ext = ".scne"

    filter_glob: StringProperty(
        default="*.scne",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    def draw(self, context):
        return

    def doImport( context, filepath , keywords):
        loadModelFile( filepath , keywords)
        return {'FINISHED'}

    def execute(self, context):
        keywords = self.as_keywords(
            ignore=(
                "axis_forward",
                "axis_up",
                "global_scale",
                "check_existing",
                "filter_glob",
            ),
        )
        return self.doImport(self.filepath, keywords )
    
def menu_bar_vc_import(self, context):
    self.layout.operator(vCNBAModelImport.bl_idname, text="Visual Concepts SCNE (.SCNE)", icon='EVENT_V')

def register_vcs_import():
    bpy.utils.register_class(vCNBAModelImport)
    bpy.types.TOPBAR_MT_file_import.append(menu_bar_vc_import)
    return

def unregister_vcs_import():
    bpy.utils.unregister_class(vCNBAModelImport)
    bpy.types.TOPBAR_MT_file_import.remove(menu_bar_vc_import)
    return