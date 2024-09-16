import bpy
import bmesh

def setMeshSmoothing(bmesh):
    for f in bmesh.polygons:
        f.use_smooth = True
    return

def setTexCoords(bmesh, uv_maps):
    if (uv_maps == None): 
        return
    
    # Iterate and create unique channels per given map array
    for index, map in enumerate(uv_maps):
        channel_name = (bmesh.name + "_UV" + str(index))
        uv_layer = bmesh.uv_layers.new(name=channel_name)

        # Specify uv coords for each mesh vertex
        for face in bmesh.polygons:
            for vert_idx, loop_idx in zip(face.vertices, face.loop_indices):
                uv_layer.data[loop_idx].uv = map[vert_idx]
    return

def setMeshNormals(bmesh, normals):
    if ( bpy.app.version < (4,1,0)):
        # 'use_auto_smooth' and 'create_normals_split' deprecated in BPY v4.1
        bmesh.use_auto_smooth = True
        bmesh.create_normals_split()

    # initialize custom split normal dataset 
    setMeshSmoothing(bmesh)
    bmesh.normals_split_custom_set_from_vertices( normals )
    return bmesh.update()

def unpackVertex(verts, index):
    pos = (index*3)
    return  [ verts[pos], verts[pos+1], verts[pos+2] ]

def remove_loose_verts(mesh):
    bm = bmesh.new()
    bm.from_mesh(mesh)

    # Iterate over all vertices and remove those that aren't connected to any faces
    loose_verts = [v for v in bm.verts if not v.link_faces]
    bmesh.ops.delete(bm, geom=loose_verts, context='VERTS')

    # Update the original mesh
    bm.to_mesh(mesh)
    bm.free()
    return

def loadSkinMesh(skinmesh, settings, model_path):
    # Create BPY mesh
    new_mesh = bpy.data.meshes.new( skinmesh.mesh_name )
    new_mesh.from_pydata(skinmesh.vertices, [], skinmesh.index_list)
    new_object = bpy.data.objects.new( skinmesh.name , new_mesh ) # Create object container

    # Set mesh attributes
    setMeshNormals   (new_mesh,    skinmesh.vertex_normals)
    setTexCoords     (new_mesh,    skinmesh.texcoords)

    # Debug to remove extraneous geo
    remove_loose_verts(new_mesh)

    return new_object

