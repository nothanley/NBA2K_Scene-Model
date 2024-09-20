import bpy
from bpy.types import Panel, Operator
from bpy.props import StringProperty, IntProperty, BoolProperty

# Non BPY includes
from .mesh_link  import *

# GUI Single Refresh Operator - Reloads .SCNE file and rescans scene
class NSRefreshOperator(Operator):
    # gui attributes
    bl_idname = "wm.nsmesh_refresh_operator"
    bl_label = ""
    bl_description = "Reloads the specified .scne file and all meshes. Re-scans blender scene for matching meshes."

    @staticmethod
    def reset_link(scene):
        user_path = scene.ns_panel_path
        link      = scene.ns_link

        link.reset()
        link.setup( user_path )
        return

    def execute(self, context):
        self.reset_link(context.scene)
        self.report({'INFO'}, f"Scene Link Refreshed.")
        return {'FINISHED'}
    
# GUI Load Operator - Builds selected mesh in blender context
class NSMeshLoadOperator(Operator):
    # gui attributes
    bl_idname = "wm.nsmesh_load_operator"
    bl_label = ""
    bl_description = "Adds the linked mesh to the current Blender scene."

    # Non-GUI Attributes
    link_index: IntProperty()

    def execute(self, context):
        links    = context.scene.ns_link.links

        if self.link_index > len(links):
            print("[NSLink] Invaid MeshLink Index. Failed to load.")
            return

        # Request .SCNE file update from Blender mesh
        mesh_link = links[self.link_index]

        if mesh_link.load_mesh(context):
            self.report({'INFO'}, f"Mesh '{mesh_link.name}' loaded")
        
        return {'FINISHED'}
    
# GUI Single Link Operator - Executes on user inject/update request
class NSMeshOperator(Operator):
    # gui attributes
    bl_idname = "wm.nsmesh_link_operator"
    bl_label = ""
    bl_description = "Updates mesh data for linked item from current Blender scene to .SCNE"

    # Non-GUI Attributes
    link_index: bpy.props.IntProperty()

    @staticmethod
    def draw_gui(link, context, layout):
        label_width = .23
        button_width = .7

        row   = layout.row()
        row.scale_x = button_width
        row.label(text= f"Mesh Link:", icon='SEQUENCE_COLOR_04')

        id_box     = row.box()
        row.scale_x = label_width
        id_box.label(text= f"{link.name}")
        
        row.scale_x = button_width
        op_load   = row.operator("wm.nsmesh_load_operator", text=f"Reload", icon='FILE_REFRESH')
        op_inject = row.operator("wm.nsmesh_link_operator", text=f"Inject", icon='IMPORT')
        op_inject.link_index = link.index
        op_load.link_index   = link.index
        return

    def execute(self, context):
        path     = context.scene.ns_panel_path
        links    = context.scene.ns_link.links

        # Validate scene settings
        if not path:
            print("[NSLink] Invalid .SCNE path. Failed to inject.")
            return

        if self.link_index > len(links):
            print("[NSLink] Invaid MeshLink Index. Failed to inject.")
            return

        # Request .SCNE file update from Blender mesh
        mesh_link = links[self.link_index]

        if mesh_link.send_update(context, path):
            self.report({'INFO'}, f"OBJECT \"{mesh_link.blender_obj_id}\" injected to GAME mesh: \"{mesh_link.name}\"")
            NSRefreshOperator.reset_link(context.scene) # Refresh scene
        else:
            self.report({'ERROR'}, f"Could not locate valid blender obj: \"{mesh_link.blender_obj_id}\". Mesh inject failed.")

        return{'FINISHED'}

# GUI Main Link Class - Parents child link widgets
class NS_Mesh_Link_Panel(Panel):
    @staticmethod
    def user_has_nba_path(context, scene):
        # Check if user has selected a file path in GUI
        return scene.ns_panel_path
    
    # GUI attributes
    bl_label = "Mesh Link (Blender/SCNE)"
    bl_idname = "NS_Mesh_Link_Panel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'NBA 2K'

    # Non-GUI Attributes
    bpy.types.Scene.ns_inject_mode = bpy.props.EnumProperty(
        name="Inject Mode",
        description="Choose between using UVs or Vertex ID",
        items=[
            ('0', "Use UVs", "Use UVs for injection"),
            ('1', "Use Vertex ID", "Use Vertex ID for injection")
        ],
        default='1'  # Set a default value
    )

    def build_file_link(self, scene):
        user_path = scene.ns_panel_path
        link      = scene.ns_link

        if (user_path != link.path()):
            # user and link path mismatch - reset link
            print("[NSLink] Link and path mismatch. reseting scene link ...")
            link.setup( user_path )
            return
        
        if not link.ready():
            # Builds CSceneLink global obj - opens and fetches scene and mesh links
            link.setup( user_path )

        return
    
    def draw_settings(self, context, layout):
        scene = context.scene
        row = layout.row().box().row()
        row.label(text="Inject Settings:", icon='MODIFIER')
        row.prop(scene, "ns_inject_mode", expand=True)
        return
    
    def draw(self, context):
        header_scale = 1.3
        ns_link = context.scene.ns_link
        layout  = self.layout

        # Draw Header
        row = layout.row()
        box = layout.box()
        row.label(text="Status:", icon='STATUSBAR')
        op = row.operator("wm.nsmesh_refresh_operator", text=f"Refresh", icon='FILE_REFRESH')
        box.label(text= f"\tFound {ns_link.num_links()} matching objects in scene.")

        # Link Header
        row = layout.row()
        row.scale_y = header_scale
        row.scale_x = header_scale
        row.label(text="Scene Links:", icon='RESTRICT_INSTANCED_OFF')

        # Check User Input
        if self.user_has_nba_path(context, context.scene):
            
            # Initialize File-Link
            self.build_file_link(context.scene)
            
            # Draw Link Table
            if ns_link.ready():

                if len(ns_link.links) > 0:
                    box = layout.box()
                
                    for link in ns_link.links:
                        NSMeshOperator.draw_gui(link, context, box)

        # Update Settings
        self.draw_settings(context, layout)

        return

# Register properties and classes
def register_mesh_inject_tool():
    bpy.types.Scene.ns_link = CSceneLink()
    bpy.utils.register_class(NS_Mesh_Link_Panel)
    bpy.utils.register_class(NSMeshOperator)
    bpy.utils.register_class(NSRefreshOperator)
    bpy.utils.register_class(NSMeshLoadOperator)


# Unregister properties and classes
def unregister_mesh_inject_tool():
    del bpy.types.Scene.ns_inject_mode
    del bpy.types.Scene.ns_link
    bpy.utils.unregister_class(NS_Mesh_Link_Panel)
    bpy.utils.unregister_class(NSMeshOperator)
    bpy.utils.unregister_class(NSRefreshOperator)
    bpy.utils.unregister_class(NSMeshLoadOperator)

