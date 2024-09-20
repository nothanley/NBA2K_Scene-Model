import bpy
from bpy.types import Panel, Operator
from bpy.props import StringProperty
from bpy_extras.io_utils import ImportHelper

# tool panels ...
from .mesh_inject import *
from .mesh_table import *

# Operator to open the file browser
class NS_Main_File_Sel(Operator, ImportHelper):
    bl_idname = "wm.file_browser_operator"
    bl_label = ""
    
    # Define filter options (this filters by file extension)
    filter_glob: StringProperty(
        default="*.scne",
        options={'HIDDEN'},
        maxlen=255,
    )

    def execute(self, context):
        # Update the text field with the selected file path
        context.scene.ns_panel_path = self.filepath
        return {'FINISHED'}

class NS_UI_Panel(Panel):
    bl_label = "NBA 2K Tools Panel"
    bl_idname = "NS_UI_Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'NBA 2K'

    def draw(self, context):
        layout = self.layout
        scene = context.scene
        
        row = layout.row()

        # Display the file browser operator button
        row.scale_x = .5
        row.operator("wm.file_browser_operator", text="Open .SCNE file", icon="FILE_FOLDER")

        # Display the file path input field
        row.scale_x = 1
        row.prop(scene, "ns_panel_path")


# Register properties and classes
def register_panel():
    bpy.types.Scene.ns_panel_path = StringProperty(name="", default="")
    
    bpy.utils.register_class(NS_Main_File_Sel)
    bpy.utils.register_class(NS_UI_Panel)

    # Additional Widgets ...
    register_mesh_inject_tool()
    register_mesh_table_panel()

# Unregister properties and classes
def unregister_panel():
    del bpy.types.Scene.ns_panel_path

    bpy.utils.unregister_class(NS_Main_File_Sel)
    bpy.utils.unregister_class(NS_UI_Panel)

    # Additional Widgets ...
    unregister_mesh_inject_tool()
    unregister_mesh_table_panel()

