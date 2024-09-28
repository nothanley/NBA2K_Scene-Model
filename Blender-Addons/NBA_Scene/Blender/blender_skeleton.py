import bpy
from mathutils import Vector
from math import radians

def getBlenderMode(obj):
    if obj:
        mode = bpy.context.object.mode
        return mode
    else:
        return None 

def createRigJoint(armature, bones, joint):
    if joint.name == None:
        return

    # Define bone's tail to access it's transform
    bbone      = armature.edit_bones.new( joint.name )
    bbone.tail = Vector((0.0, 1.0, 0.0)) * max(0.01, 1.0)

    # Use X, -Z, Y to match Blender's coordinate system
    bbone.head = Vector( (joint.matrix[0][0], -joint.matrix[0][2], joint.matrix[0][1]) )

    # Link hierarchy 
    if joint.parent:
        bbone.parent = bones[joint.parent].scene_bone
        bbone.head = bbone.head + bbone.parent.head
        bbone.tail = bbone.parent.head

    # Set minimum length to prevent culling
    if( bbone.length < .00000001 ):        
        bbone.length = bbone.length + .0001  

    # Update and link blender bone to 'vCRigJoint'
    bones[joint.id].scene_bone = bbone
    return

def loadSkeleton(scene, skeleton):
    # Create an armature object in the scene    
    armature = bpy.data.armatures.new("Skeleton")
    armature.display_type = "WIRE"
    obj = bpy.data.objects.new("Skeleton", armature)

    # Focus armature obj in edit mode to create rig
    scene.objects.link(obj)
    bpy.context.view_layer.objects.active = obj

    restore_mode = getBlenderMode(obj)
    bpy.ops.object.editmode_toggle()
    
    # Add all child joints to armature
    for joint in skeleton.bones:
        createRigJoint(armature, skeleton.bones, joint)

    # Switch back to original view mode
    bpy.ops.object.mode_set ( mode = restore_mode )
    return obj

def build_sphere(name, collection, translate=Vector(), scale=Vector((1, 1, 1))):
    # Create a new empty object
    empty_obj = bpy.data.objects.new(name, None)
    empty_obj.empty_display_type = 'SPHERE'

    # Set the location of the empty sphere
    empty_obj.location = translate
    empty_obj.scale = scale
    collection.objects.link(empty_obj)

    return empty_obj

def add_sphere_constraint(armature, sphere_obj, target_index):
    bpy.context.view_layer.objects.active = sphere_obj
    bpy.ops.object.constraint_add(type='ARMATURE')

    constraint_bone           = bpy.context.object.constraints["Armature"].targets.new()    
    constraint_bone.target    = armature
    constraint_bone.subtarget = armature.data.bones[target_index].name
    return

def find_bone_index(armature, target):
    for i, bone in enumerate(armature.data.bones):
        if (bone.name == target):
            return i
    
    return None

def load_attachment_points(collection, root_node, armature_obj, points):
    if (not points): 
        return
    
    skeleton = armature_obj.data
    num_bones = len(skeleton.bones)

    for point in points:
        if (point.joint < num_bones):
            translate = Vector(point.co)
            obj = build_sphere( f"ap_{point.id_a}-{point.id_b}-{point.flag}", collection, translate )
            obj.parent = root_node
            add_sphere_constraint(armature_obj, obj, point.joint)
            
    return



