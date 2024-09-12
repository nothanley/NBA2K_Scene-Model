import numpy as np
from ..Link.wrapper        import cmodellib
from mathutils import Vector # type: ignore

def getMeshName(mesh):
    return cmodellib.getMeshName(mesh.data, mesh.id).decode("utf-8")
    
def getNumVerts(mesh):
    return cmodellib.getNumVerts(mesh.data, mesh.id)

def getNumTris(mesh):
    return cmodellib.getNumTriangles(mesh.data, mesh.id)

def getNumUvChannels(mesh):
    return cmodellib.getNumUvChannels(mesh.data, mesh.id)

def getMeshVerts(mesh):
    num_items = getNumVerts(mesh)
    vert_data = cmodellib.getVertexData(mesh.data, mesh.id)
    if vert_data is None or num_items == 0:
        return None

    # Creates a NumPy array that points to original memory allocation
    data = np.ctypeslib.as_array(vert_data, shape=(num_items * 3,)).tolist()
    
    # Convert the 2D NumPy array to a list of Vector objects
    return [Vector(data[i:i+3]) for i in range(0, len(data), 3)]

def getMeshNormals(mesh):
    num_items = getNumVerts(mesh)
    vert_data = cmodellib.getMeshNormals(mesh.data, mesh.id)
    if vert_data is None or num_items == 0:
        return None

    # Creates a NumPy array that points to original memory allocation
    data = np.ctypeslib.as_array(vert_data, shape=(num_items * 3,)).tolist()
    normals = [Vector(data[i:i+3]) for i in range(0, len(data), 3)]

    # Normalize all vectors to maintain smoothing
    for normal in normals:
        normal.normalize()

    # Convert the 2D NumPy array to a list of Vector objects
    return normals

def getIndexList(mesh):
    num_indices = getNumTris(mesh)
    index_data = cmodellib.getMeshTriangleList(mesh.data, mesh.id)
    if (index_data == None or num_indices == 0):
        return None
    
    # Creates a NumPy array that points to original memory allocation
    data = np.ctypeslib.as_array(index_data, shape=(num_indices*3,)).tolist()

    # Convert the 2D NumPy array to a list of Tuple objects
    return [tuple(data[i:i+3]) for i in range(0, len(data), 3)]

def getUVChannel(mesh, index):
    num_verts = getNumVerts(mesh)
    uv_data = cmodellib.getMeshUvChannel(mesh.data, mesh.id, index)
    if uv_data is None or num_verts == 0:
        return None

    # Creates a NumPy array that points to original memory allocation
    data = np.ctypeslib.as_array(uv_data, shape=(num_verts * 2,)).tolist()
    
    # Convert the 2D NumPy array to a list of Vector objects
    return [tuple(data[i:i+2]) for i in range(0, len(data), 2)]

def getTexCoords(mesh):
    num_channels = getNumUvChannels(mesh)
    uv_channels = []

    for i in range(num_channels):
        map = getUVChannel(mesh, i)

        if (map != None):
            uv_channels.append(map)

    return uv_channels

class vCSkinMesh():
    def __debug_log(self):
        print("\n[NBA2K Addon] Reading Mesh Data For: ", self.mesh_name)
        print("\n[NBA2K Addon] Total Vertices: ",  len(self.vertices))
        print("\n[NBA2K Addon] Total Triangles: ", len(self.index_list) )
        print("\n[NBA2K Addon] Tri List: ", self.index_list)
        return
    
    def __setup(self):
        # Populate mesh data 
        self.name            = getMeshName(self)
        self.mesh_name       = getMeshName(self)
        self.vertices        = getMeshVerts(self)
        # self.vertex_normals  = getMeshNormals(self)
        self.index_list      = getIndexList(self)
        self.texcoords       = getTexCoords(self)

        return
    
    def __init__(self, data=None, index=None):
        self.data            = data
        self.scene_flag      = None
        self.motion_flag     = None
        self.name            = None
        self.mesh_name       = None
        self.id              = index
        self.vertices        = None
        self.vertex_normals  = None
        self.index_list      = None
        self.texcoords       = None
        self.skin            = None
        self.vertex_colors   = None
        self.blendshapes     = None
        self.material_groups = None

        if (self.data != None):
            self.__setup()

