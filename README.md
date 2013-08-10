CPlusPlus-Opengl-Dragonbones
============================

This is a c++ port of the Skeletal Animation System DragonBones which was originally created for as3. It can be found
at http://dragonbones.github.io/

The flash panel is not included in this project and has to be obtained at the offical dragonbones website.

The current version that this port represents is version 2.3 of dragonbones.
This is a very early port and not everything works. The only test tried is the test included. 

Things that do not work yet:
Color transforms
XML Parsing(Only JSON works but XML will soon be added)

Things that will never be included:
Sound Events
SWF parsing

The source comes with a example and library build project for visual studio. It uses GLFW for window rendering but
can be switched if you wish. The only thing that C++ DragonBones uses GLFW for is time which can be changed 
in WorldClock.h to whatever you want without recompiling. 

The Lincense for the source code is do the hell what you want with it.
I do not own any of the artwork/animations and recevied them from the tests for the offical Dragonbones.

The project also uses GLM for matrix math, SOIL for image loading, and rapidjson for json parsing.
GLM is header only and is included in the DragonBone includes. SOIL and rapidjson are compiled into the lib.
The only libs you need to include in a new project are: opengl32.lib, glfw3.lib(if you are using it for window 
rendering), glew32.lib, and DragonBones.lib.

If you create a new project you need to include the DragonBones folder located in the DragonBonesExample project.





