import bpy
from ..SkinModel.skinmaterial import *

def get_node_tex_img(node, target):
    if not node.type == 'BSDF_PRINCIPLED':
        return None

    for input in node.inputs:
        if input.name == target and input.is_linked:
            link = input.links[0]
            texture_node = link.from_node

            if texture_node.type == 'TEX_IMAGE':
                return texture_node.image
            
    return None

def get_material_image(material, target):
    if not material.use_nodes:
        return None
    
    img = None
    for node in material.node_tree.nodes:
        img = get_node_tex_img(node, target)
        if img: 
            return img
    
    return None

def get_blender_material(obj):
    if obj.material_slots:
        # Return the first material if it exists
        return obj.material_slots[0].material
    return None

def getPBRTextures(bmat, new_mat):
    # Get the color map texture
    color_img              = get_material_image(bmat, 'Base Color')
    new_mat.color_map      = CMaterialTex.from_blend_img(color_img, "ColorTexture")

    # Get the normal map texture
    normal_img              = get_material_image(bmat, 'Normal')
    new_mat.normal_map      = CMaterialTex.from_blend_img(normal_img, "NormalTexture")

    # Get the roughness/metallic map texture
    roughness_img         = get_material_image(bmat, 'Roughness')
    new_mat.rdmo_map      = CMaterialTex.from_blend_img(roughness_img, "RoughnessDensityMetalnessMap")
    return

def getMaterials(obj):
    # Get the linked Blender material
    bmat = get_blender_material(obj)
    if not bmat: 
        return []
    
    # Initialize a new material object
    new_mat = CSkinMaterial.from_blend_mat(bmat)
    getPBRTextures(bmat, new_mat)
    return  [new_mat]


