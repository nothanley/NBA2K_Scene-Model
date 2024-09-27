import bpy
import numpy as np
import ctypes

class CMaterialTex():
    
    @staticmethod
    def get_filename(path):
        return path.split('/')[-1].split('.')[0]

    @staticmethod
    def from_blend_img(image, type):        
        # Get image dimensions and pixel data
        if not image: return None
        image.reload()
        width, height = image.size
        total_pixels = width * height * 4  # (RGBA)
        pixel_data = np.empty(total_pixels, dtype=np.float32)

        # Copy pixel data from Blender image to NumPy array
        image.pixels.foreach_get(pixel_data)
        pixmap = pixel_data.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

        # Create a new texture object
        tex_name = CMaterialTex.get_filename(image.name)
        new_tex  = CMaterialTex(tex_name, type, width, height)
        new_tex.set_pixel_buffer(pixmap)
        return new_tex
    
    def set_pixel_buffer(self, data):
        self.__pixel_buffer = data
        return

    def get_pixel_buffer(self):
        return self.__pixel_buffer

    def __init__(self, name=None, type=None, width=None, height=None):
        self.name           = name
        self.type           = type
        self.width          = width
        self.height         = height
        self.__pixel_buffer = None

        
class CSkinMaterial():
    
    @staticmethod
    def from_blend_mat(material):
        # Initialize a new material object
        new_mat = CSkinMaterial()
        new_mat.name = material.name
        return new_mat
    
    def __init__(self):
        self.name        = None
        self.index_begin = None
        self.index_end   = None
        self.color_map   = None
        self.normal_map  = None
        self.rdmo_map    = None
    


