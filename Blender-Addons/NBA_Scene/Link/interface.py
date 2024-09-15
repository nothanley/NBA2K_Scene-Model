import time
import ctypes
from .wrapper import ExternalLibary
from ..SkinModel.skinmodel    import CNBAScene , cNBASkinModel
from ..Blender.blender_scene  import cNBABlenderScene

def buildScene(scene_pointer, model_path, args):

    # initialize scene models  
    scene = CNBAScene(scene_pointer)
    models = [None] * scene.num_models

    for i in range(scene.num_models):
        models[i] = cNBASkinModel(scene_pointer, i)
    
    # Send loaded models to blender for import
    scene = cNBABlenderScene(models)
    scene.set_name(model_path)
    scene.set_load_args(args)
    scene.build()

    return

def loadSceneFile( filepath , args ):
    # begin timer
    start_time = time.time()

    # setup all 'nbamodel' dll exports into a single class
    module = ExternalLibary()
    
    # loads the model from specified path and retrieves the cnbascene pointer 
    scene_file = ctypes.c_void_p()
    nba_scene = module.load_scene( filepath.encode('utf-8'), ctypes.byref(scene_file), args['include_lods'] )

    # evaluate result type
    if (not scene_file or not nba_scene): 
        print("Could not load invalid model.")
    else:
        buildScene(nba_scene, filepath, args)
        module.release_scene_file(scene_file)  # clean model + file heap

   # end timer and print elapsed time
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"\nElapsed time: {elapsed_time} seconds")
    return

