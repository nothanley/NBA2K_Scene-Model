from ..Link.wrapper import cmodellib
import numpy as np


class vCRigJoint():

    @staticmethod
    def getBoneParentIndex(joint):
        index = cmodellib.getBoneParentIndex(joint.data, joint.id)

        if (index == -1): return None
        return index

    @staticmethod
    def getBoneMatrix(joint):
        matrix_data = cmodellib.getBoneMatrix(joint.data, joint.id)

        # Creates a NumPy array that points to original memory allocation
        data = np.ctypeslib.as_array(matrix_data, shape=(16,)).tolist()

        # Free all float memory
        cmodellib.free_float_array(matrix_data)
        return [tuple(data[i:i+4]) for i in range(0, len(data), 4)]

    @staticmethod
    def getBoneName(joint):
        name = cmodellib.getBoneName(joint.data, joint.id)

        if (name != None):
            return name.decode("utf-8")
        return None

    def isValidJoint(self):
        self.name = vCRigJoint.getBoneName(self)
        return (self.name != None)
    
    def __setup(self):
        # Define joint properties 
        if (not self.isValidJoint()): return
        self.parent = vCRigJoint.getBoneParentIndex(self)
        self.matrix = vCRigJoint.getBoneMatrix(self)

        return
     
    def __init__(self, data=None, index=None):
        self.data   = data
        self.id     = index
        self.name   = None
        self.parent = None
        self.matrix = None
        self.scene_bone = None

        if (self.data != None):
            self.__setup()

class vCSkeleton():
    
    @staticmethod
    def getNumBones(skel):
        return cmodellib.getNumBones(skel.data)

    def __setup(self):
        num_bones = self.getNumBones(self)

        for i in range(num_bones):
            joint = vCRigJoint(self.data, i)
            self.bones.append(joint)

    def __init__(self, data=None):
        self.data  = data
        self.bones = []

        if (data != None):
            self.__setup()
