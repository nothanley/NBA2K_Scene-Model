import bpy, bmesh
from mathutils import Vector

class CVertexSkin():
    @staticmethod
    def get_vertex_limit():
        # Defines total weights allowed per mesh vertex
        return 8
    
    def __set_bones(self, obj):
        self.bones = [bone.name for bone in obj.parent.data.bones]
        return
    
    def prepopulate_skin(self, source_object):
        # Prepopulates skin with empty index/skin values
        num_vertices = len(source_object.data.vertices)

        self.blendindices = [0]   * (num_vertices * self.weights_per_vertex)
        self.blendweights = [0.0] * (num_vertices * self.weights_per_vertex)
        self.__set_bones(source_object)
        return
    
    def __init__(self):
        self.blendindices = []
        self.blendweights = []
        self.bones = None
        self.weights_per_vertex = None

def setBlenderMode(target_mode, select_obj=None):
    if (select_obj):
        bpy.context.view_layer.objects.active = select_obj

    # Check if the scene is already in the target mode
    if bpy.context.mode == target_mode:
        return
    
    # Switch to the target mode
    bpy.ops.object.mode_set(mode=target_mode)
    return

def getEditMesh(obj):
    setBlenderMode('EDIT', obj)
    bm = bmesh.from_edit_mesh(obj.data)
    return bm

def has_armature_modifier(obj):
    return any(mod.type == 'ARMATURE' for mod in obj.modifiers)

def markMeshSeamsFromUVs():
    # mark seams from uv islands
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.uv.select_all(action='SELECT')
    bpy.ops.uv.seams_from_islands()
    return

def triangulate_mesh(bm):
    bmesh.ops.triangulate(bm, faces=bm.faces[:])
    return

def isTriangulated(obj):
    for face in obj.data.polygons:
        if len( face.vertices ) != 3:
            return False
        
    return True

def doObjEdgeSplit(obj):
    for modifier in obj.modifiers:
        if modifier.type == "EDGE_SPLIT":
            bpy.ops.object.modifier_apply(modifier)
            return
        
    obj.modifiers.new(name = 'OptimNorms' , type='EDGE_SPLIT')
    bpy.ops.object.modifier_apply(modifier="OptimNorms")
    return

def unpack_values(packed_values):
    unpacked_values = []

    for value in packed_values:
        for ele in value:
            unpacked_values.append(ele)

    return unpacked_values

def getMeshVertices(obj):
    # unpack all verts relative to world space
    coords = [position for vert in obj.data.vertices for position in obj.matrix_world @ vert.co]
    return coords

def getMeshFaces(obj):
    index_list = []

    for f in obj.data.polygons:
        for vertex in f.vertices:
            vert_index = (obj.data.vertices[vertex].index)
            index_list.append( vert_index )

    return index_list

def apply_transfrom(ob):
    bpy.context.view_layer.objects.active = ob
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    return

def getVertexNorms(obj):
    numVerts = len( obj.data.vertices )
    normals  = list( range( numVerts * 3 ) )

    for loop in obj.data.loops:
        normal_local = loop.normal.to_4d()
        normal_local.w = 0
        normal_local = (obj.matrix_world @ normal_local).to_3d()

        # Append normals to unpacked list
        index = loop.vertex_index * 3
        for i, ray in enumerate(normal_local):
            normals[index + i] = ray

    return normals

def getUvChannelMap(bmesh, size, uv_layer):
    model_uvs = [ [0,0] ] * size

    for face in bmesh.polygons:
        for vert_idx, loop_idx in zip(face.vertices, face.loop_indices):
            uv_coords = uv_layer.data[loop_idx].uv
            model_uvs[vert_idx] = ( uv_coords.x , uv_coords.y )
    
    # Unpack for target cmesh (optional) ...
    model_uvs = unpack_values(model_uvs)
    return model_uvs

def getTexCoords(obj):
    uv_sets     = obj.data.uv_layers
    uv_maps     = []
    total_verts = len(obj.data.vertices)
    
    for layer in uv_sets:
        map = getUvChannelMap(obj.data, total_verts, layer)
        uv_maps.append( map )

    return uv_maps

def unpack_matrices(matrix):
    matrices = []
    for row in matrix:
        for value in row:
            matrices.append(value)
    return matrices
        

