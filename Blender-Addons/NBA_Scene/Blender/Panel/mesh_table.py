import bpy
from bpy.types           import Panel, Operator
from bpy.props           import StringProperty, IntProperty, BoolProperty
from bpy_extras.io_utils import ImportHelper

# Non BPY includes
from .mesh_link import *

class NS_Mesh_Table_Row(Operator):
    # gui attributes
    bl_idname = "wm.nsmesh_row_operator"
    bl_label = ""
    bl_description = "Adds the game mesh to the current Blender scene."

    # Non-GUI Attributes
    mesh_index: bpy.props.IntProperty()

    @staticmethod
    def draw_gui(index, mesh, layout):
        label_width  = .4
        button_width = .7

        row         = layout.row()
        row.scale_x = button_width
        row.label(text= f"Game Mesh:", icon='META_CUBE')

        id_box      = row.box()
        row.scale_x = label_width
        id_box.label(text= f"{mesh.name}")
        
        row.scale_x          = button_width
        op_load              = row.operator("wm.nsmesh_row_operator", text=f"Import", icon='EXPORT')
        op_load.mesh_index   = index
        return

    def execute(self, context):
        ns_link        = context.scene.ns_link
        game_meshes    = ns_link.get_game_meshes()

        if not game_meshes or not ns_link.ready() or not (self.mesh_index < len(game_meshes)):
            return{'FINISHED'}

        mesh = ns_link.get_mesh(self.mesh_index)
        self.report({'INFO'}, f"Loading Game Mesh: \"{mesh.name}\"")

        if ns_link.load_mesh(self.mesh_index):
            self.report({'INFO'}, f"Mesh '{mesh.name}' Loaded.")
        
        return{'FINISHED'}

class NS_Mesh_Table_Panel(Panel):
    # GUI attributes
    bl_label       = "Mesh Table"
    bl_idname      = "NS_Mesh_Table_Panel"
    bl_space_type  = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category    = 'NBA 2K'
    
    @staticmethod
    def get_link(context):
        path     = context.scene.ns_panel_path
        ns_link  = context.scene.ns_link
        meshes   = ns_link.get_game_meshes()

        # Validate scene settings
        if not path or not meshes: return None

        return ns_link
    
    def draw(self, context):
        
        ns_link = self.get_link(context)
        if not ns_link: return

        # Draw Link Table GUI
        layout  = self.layout

        if ns_link.ready():
            game_meshes = ns_link.get_game_meshes()
            if not game_meshes:
                return

            box = layout.box()

            for x, mesh in enumerate(game_meshes):
                NS_Mesh_Table_Row.draw_gui(x, mesh, box)

        return

# Register properties and classes
def register_mesh_table_panel():
    bpy.utils.register_class(NS_Mesh_Table_Panel)
    bpy.utils.register_class(NS_Mesh_Table_Row)


# Unregister properties and classes
def unregister_mesh_table_panel():
    bpy.utils.unregister_class(NS_Mesh_Table_Panel)
    bpy.utils.unregister_class(NS_Mesh_Table_Row)

