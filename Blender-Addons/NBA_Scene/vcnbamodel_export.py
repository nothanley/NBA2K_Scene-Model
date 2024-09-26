from .Link.interface_save import *
from bpy.types import Operator
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy_extras.io_utils import ExportHelper

 # Define enum items for the dropdown
COMPILE_TARGETS = \
[
    ("option_vcexport_25", "NBA 2K25 .SCNE", "Formats model as NBA 2K25 MDL")
]

class vCNBAModelExport(Operator, ExportHelper):
    """Export Visual Concepts | NBA2K model"""
    bl_idname = "export_vcnbamodel.save"
    bl_label = "Export SCNE"
    filename_ext = ".SCNE"

    filter_glob: StringProperty(
        default="*.SCNE",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    compile_target: EnumProperty(
        name="",
        items=COMPILE_TARGETS,
        default="option_vcexport_25",
        description="Select target SCNE format",
    )

    def doExport( context, filepath , keywords):
        saveModelFile( filepath , keywords)
        return {'FINISHED'}

    # Adjust the text width for the properties
    def draw(self, context):
        layout = self.layout
        box = layout.box()
        
        box.label(text="Game Options:")
        split = box.split(factor=0.2) 
        split.label(text="Target:")
        split.prop(self, "compile_target")
        
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
        return self.doExport(self.filepath, keywords )
    
def menu_bar_vc_export(self, context):
    self.layout.operator(vCNBAModelExport.bl_idname, text="Visual Concepts SCNE (.SCNE)", icon='EVENT_S')

def register_vcs_export():
    bpy.utils.register_class(vCNBAModelExport)
    bpy.types.TOPBAR_MT_file_export.append(menu_bar_vc_export)
    return

def unregister_vcs_export():
    bpy.utils.unregister_class(vCNBAModelExport)
    bpy.types.TOPBAR_MT_file_export.remove(menu_bar_vc_export)
    return