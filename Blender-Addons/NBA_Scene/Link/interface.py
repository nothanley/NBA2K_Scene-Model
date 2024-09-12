import time
import ctypes
from .wrapper import ExternalLibary
from ..SkinModel.skinmodel    import cNBASkinModel
from ..Blender.blender_scene  import cNBABlenderScene

def buildModel(skin_model_pointer, model_path, args):
    model = cNBASkinModel(skin_model_pointer)

    # Send loaded model to blender for import
    scene = cNBABlenderScene([model])
    scene.set_name(model_path)
    scene.set_load_args(args)
    scene.build()
    return

def loadModelFile( filepath , args ):
    # begin timer
    start_time = time.time()

    # setup all 'vcmodel' dll exports into a single class
    module = ExternalLibary()

    # loads the model from specified path and retrieves the skinmodel pointer 
    model_file = ctypes.c_void_p()
    skin_model = module.load_model( filepath.encode('utf-8'), ctypes.byref(model_file) )

    # evaluate result type
    if (not model_file or not skin_model): 
        print("Could not load invalid model.")
    else:
        buildModel(skin_model, filepath, args)
        module.release_model_file(model_file)  # clean model + file heap

   # end timer and print elapsed time
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Elapsed time: {elapsed_time} seconds")
    return

