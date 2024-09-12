from ..Link.wrapper import cmodellib
import ctypes

class vCMeshGroup():
    def __init__(self):
        self.material   = None
        self.face_begin = None
        self.face_end   = None
        self.diffuse_id  = None

def getGroupLabels(cmodel, mesh_index):
    numGroups = ctypes.c_int()
    data      = cmodellib.getAllFaceGroups(cmodel, mesh_index, ctypes.byref(numGroups))
    if (data == None): 
        return []

    # Convert the received data to a Python list of strings
    labels = [ctypes.string_at(data[i]).decode("utf-8") for i in range(numGroups.value)]
    cmodellib.free_string_arr(data)
    return labels

def getGroupData(cmodel, mesh_index, group_index):
    faceBegin = ctypes.c_int()
    faceSize  = ctypes.c_int()

    # Get group triangle index and size
    cmodellib.getMaterialFaceGroup(cmodel, mesh_index, group_index, ctypes.byref(faceBegin), ctypes.byref(faceSize))
    return faceBegin.value, faceSize.value

def getMaterialDiffuseName(cmodel, mesh_index, group_index):
    map_id = cmodellib.getMaterialDiffuseMap(cmodel, mesh_index, group_index)
    return map_id.decode("utf-8")

def getMaterialGroups(cmodel, mesh_index):
    mesh_groups = []
    faceGroupNames = getGroupLabels(cmodel, mesh_index)

    for i, label in enumerate(faceGroupNames):
        triangle_index, num_triangles = getGroupData(cmodel, mesh_index, i)

        if (triangle_index != -1): 
            group = vCMeshGroup()
            group.material   = label
            group.face_begin = triangle_index
            group.face_end   = triangle_index + num_triangles
            group.diffuse_id  = getMaterialDiffuseName(cmodel, mesh_index, i)
            mesh_groups.append(group)

    return mesh_groups