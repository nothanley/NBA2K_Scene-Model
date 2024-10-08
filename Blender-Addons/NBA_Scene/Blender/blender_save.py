import bpy
import re
from ..SkinModel.skinmodel import vCSkinMesh
from .blender_utils import *
from .blender_materials import *

def getMeshSelection():
    selection_objs = bpy.context.selected_objects
    selection = []
    
    for obj in selection_objs:
        if obj.type == 'MESH':
            selection.append( obj )
            
    return selection

def getBaseName(obj):
    name = obj.name
    name = name.replace( "!","")
    name = name.replace( "$","")
    if ( "." in  name ):
        name = name.split(".")[0]
    return name

def getBoneIndex(name, skin):
    try:
        index = skin.bones.index(name)
    except ValueError:
        return -1
    return index

def getVertexWeights(vertex, skin, groups):
    table_index = (vertex.index * skin.weights_per_vertex)
    for i, elem in enumerate(vertex.groups):
        bone_index = getBoneIndex( groups[elem.group] , skin)
    
        if (bone_index != -1):
            skin.blendweights[table_index + i] = elem.weight
            skin.blendindices[table_index + i] = bone_index 
    return

def getVertexSkin(obj, weight_limit):
    skin = CVertexSkin()
    skin.weights_per_vertex = weight_limit
    skin.prepopulate_skin(obj)

    obj_groups = [group.name for group in obj.vertex_groups]
    for vertex in obj.data.vertices:
        getVertexWeights(vertex, skin, obj_groups)
    return skin

def get_closest_vertex_weight_mode(object):
    group_length = len(object.data.vertices[0].groups)
    weight_modes = {4: 'WEIGHT_4', 8: 'WEIGHT_8'}

    return 8 #min(weight_modes.keys(), key=lambda x: abs(x - group_length))

def getMeshSkin(obj):
    if not has_armature_modifier(obj):
        return None

    try:
        weight_limit = get_closest_vertex_weight_mode(obj)
        bpy.context.view_layer.objects.active  = obj
        bpy.ops.object.vertex_group_limit_total(group_select_mode='ALL', limit=weight_limit)
    except:
        return None
                
    return getVertexSkin(obj, weight_limit)

def makeSkinMesh(obj):
    # Build skin mesh from scene object
    skinmesh = vCSkinMesh()
    skinmesh.name           = getBaseName(obj)
    skinmesh.mesh_name      = getBaseName(obj.data)
    skinmesh.index_list     = getMeshFaces(obj)
    skinmesh.vertices       = getMeshVertices(obj)
    skinmesh.vertex_normals = getVertexNorms(obj)
    skinmesh.texcoords      = getTexCoords(obj)
    skinmesh.materials      = getMaterials(obj)
    skinmesh.skin           = getMeshSkin(obj)
    # skinmesh.vertex_colors  = getVertexColors(obj)
    # skinmesh.blendshapes    = getShapeKeys(obj)
    # skinmesh.scene_flag     = getGameSceneFlag(obj)
    # skinmesh.motion_flag    = getGameMotionFlag(obj)
    
    return skinmesh

def cache_vertex_normals(obj, mesh):
    vertex_cache = {}
    vertex_normals = {}

    # Iterate over all faces in the mesh
    for face in mesh.faces:
        for vert, loop in zip(face.verts, face.loops):
            vertex_coords = tuple(vert.co)
            normal = vert.normal

            if vertex_coords not in vertex_normals:
                vertex_normals[vertex_coords] = []

            vertex_normals[vertex_coords].append(normal)

    # Calculate average normals for each vertex
    for vertex_coords, normals in vertex_normals.items():
        average_normal = sum(normals, Vector()) / len(normals)
        vertex_cache[vertex_coords] = average_normal

    return vertex_cache

def get_transferred_split_normals(mesh, vertex_cache):
    if (not vertex_cache): return

    split_normals = [Vector()] * len(mesh.vertices)

    for loop in mesh.loops:
        vertex = mesh.vertices[ loop.vertex_index ]
        coord = tuple(vertex.co)
        if coord in vertex_cache:
            split_normals[loop.vertex_index] = vertex_cache[coord]
        
    return split_normals

def split_mesh_edges(obj, bm, vertex_cache):
    print(f"[Mesh Format] Target Mesh requires a uv split")
    if (not vertex_cache):
        vertex_cache = cache_vertex_normals(obj, bm)

    target_edges = [e for e in bm.edges if e.seam]
    bmesh.ops.split_edges(bm, edges=target_edges)
    return vertex_cache

def check_uv_seams(obj, bm, vertex_cache):
    # Unmark old seams
    for edge in bm.edges:
        edge.seam = False
    
    # Try and mark all uv seams
    markMeshSeamsFromUVs()
    
    # Check if any seams are marked
    for e in bm.edges:
        if e.seam: return split_mesh_edges(obj, bm, vertex_cache)
    return vertex_cache

def format_game_uv_seams(obj):
    # Split all mesh UV seam
    if ( bpy.app.version < (4,1,0) ):
        obj.data.calc_normals_split()

    mesh = obj.data
    edit_mesh = getEditMesh(obj)
    vertex_cache = None

    # Create vertex cache to remap any skewed vertex normals created by triangulating or splitting uv seams
    if ( not isTriangulated(obj) ):
        print("[Mesh Format] Mesh is not trianglulated")
        vertex_cache = cache_vertex_normals(obj, edit_mesh)
        triangulate_mesh(edit_mesh)
    
    vertex_cache = check_uv_seams(obj, edit_mesh, vertex_cache)
    bmesh.update_edit_mesh(mesh)
    
    if (vertex_cache):
        print("[Mesh Format] Retargeting normals...")
        setBlenderMode('OBJECT')
        split_normals = get_transferred_split_normals(mesh, vertex_cache)

        if ( bpy.app.version < (4,1,0) ):
            mesh.use_auto_smooth = True
            
        mesh.normals_split_custom_set_from_vertices(split_normals)

    return

def format_game_objects(objs): 
    # To avoid redundant context switching,
    # we split all 'edit' and 'object' context operations
    USE_EDGE_SPLIT = False
        
    # Seperate all mesh uv seams
    for obj in objs:
        format_game_uv_seams(obj)

    # Split mesh edges + normals
    setBlenderMode('OBJECT')
    for obj in objs:
        if (USE_EDGE_SPLIT): 
            doObjEdgeSplit(obj)

        if ( bpy.app.version < (4,1,0) ):
            obj.data.calc_normals_split()
    return

def getSceneMeshes():
    # Collect all user selection targets
    obj_selection = getMeshSelection()
    if ( len( obj_selection ) == 0 ):
        print( "\n[MDL Addon] Please select an object to save file." )
        return None

    # Preprocess all scene meshes to game-ready objs
    format_game_objects(obj_selection)

    models = []
    # Read all scene object data to skinmesh obj
    for obj in obj_selection:
        # print( "[MDL Addon] Loading Object: " + obj.name )
        skinmesh = makeSkinMesh(obj)
        models.append(skinmesh)
    
    return models

def getSceneArmature():
    selection = getMeshSelection()
    bpy.context.view_layer.update()

    for obj in selection:
        if (has_armature_modifier(obj)):
            armature = obj.parent
            target_rig = vCSkeleton()

            # print("\n[MDL Addon] Loading Armature: ", armature.name)
            for bone in armature.data.bones:
                target_rig.bones.append( getRigBone(bone) )

            return target_rig
    
    return None
