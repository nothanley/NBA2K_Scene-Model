import bpy
from .Link.interface import *
from bpy.types import Operator
from bpy.props import StringProperty, BoolProperty
from bpy_extras.io_utils import ImportHelper

# decompression includes
import os
import gzip
import shutil 

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

    # Checkbox to decide whether to convert .bin files to .gz
    convert_bin_to_gz: BoolProperty(
        name="Convert .bin to .gz",
        description="Temporarily convert .bin files to .gz before importing",
        default=False,
    )

    include_lods: BoolProperty(
        name="Import all mesh LODs",
        description="Imports all lower level LOD models to scene.",
        default=False,
    )

    split_groups: BoolProperty(
        name="Split material groups",
        description="Splits mesh per material group",
        default=False,
    )
    

    def draw(self, context):
        layout = self.layout
        box = layout.box()
        box.label(text="Import Options:")
        row = box.row()
        row.prop(self, "include_lods")
        row = box.row()
        row.prop(self, "convert_bin_to_gz")
        row = box.row()
        row.prop(self, "split_groups")
        return

    def convert_bin_files_to_gz(self, scne_path):
        """
        Converts all .bin files to .gz files in the directory of the selected .scne file.
        """
        directory = os.path.dirname(scne_path)  # Get the directory of the selected SCNE file
        for file_name in os.listdir(directory):
            if file_name.endswith('.bin'):
                bin_path = os.path.join(directory, file_name)
                gz_path = bin_path.replace('.bin', '.gz')
                
                # Skip if the .gz file already exists
                if os.path.exists(gz_path):
                    print(f"[Skip] .gz file already exists for: {gz_path}")
                    continue
                
                # Convert .bin to .gz
                try:
                    with open(bin_path, 'rb') as bin_file:
                        with gzip.open(gz_path, 'wb') as gz_file:
                            shutil.copyfileobj(bin_file, gz_file)
                    print(f"[Convert] Successfully converted {bin_path} to {gz_path}")
                except Exception as e:
                    print(f"[Error] Failed to convert {bin_path} to .gz: {e}")

    def load(self, filepath, keywords):
        """
        Attempts to load the scene file after converting .bin files to .gz.
        """
        try:
            loadSceneFile(filepath, keywords)
            print("[Success] Scene loaded successfully.")
            return True
        except Exception as e:
            print(f"[Error] Scene loading failed: {e}")
            return False
        
    def doImport(self, context, filepath , keywords):
        
        if self.convert_bin_to_gz:
            self.convert_bin_files_to_gz(filepath)

        success = self.load(filepath, keywords)
        
        return {'FINISHED' if success else 'CANCELLED'}

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
        return self.doImport(context, self.filepath, keywords)
    
def menu_bar_vc_import(self, context):
    self.layout.operator(vCNBAModelImport.bl_idname, text="Visual Concepts SCNE (.SCNE)", icon='EVENT_S')

def register_vcs_import():
    bpy.utils.register_class(vCNBAModelImport)
    bpy.types.TOPBAR_MT_file_import.append(menu_bar_vc_import)
    return

def unregister_vcs_import():
    bpy.utils.unregister_class(vCNBAModelImport)
    bpy.types.TOPBAR_MT_file_import.remove(menu_bar_vc_import)
    return