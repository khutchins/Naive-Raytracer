cmake_minimum_required(VERSION 3.6)
project(Naive_Raytracer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    src/EasyBMP/EasyBMP.cpp
    src/EasyBMP/EasyBMP.h
    src/EasyBMP/EasyBMP_BMP.h
    src/EasyBMP/EasyBMP_DataStructures.h
    src/EasyBMP/EasyBMP_VariousBMPutilities.h
    src/AntiAliasing.cpp
    src/AntiAliasing.h
    src/Color.cpp
    src/Color.h
    src/Convolution.cpp
    src/Convolution.h
    src/Diagnostic.h
    src/main.cpp
    src/Material.cpp
    src/Material.h
    src/Matrix.cpp
    src/Matrix.h
    src/ObjCamera.cpp
    src/ObjCamera.h
    src/ObjContainerObject.cpp
    src/ObjContainerObject.h
    src/ObjCuboid.cpp
    src/ObjCuboid.h
    src/ObjCylinder.cpp
    src/ObjCylinder.h
    src/ObjDisk.cpp
    src/ObjDisk.h
    src/ObjInfPlane.cpp
    src/ObjInfPlane.h
    src/ObjLight.cpp
    src/ObjLight.h
    src/ObjPlane.cpp
    src/ObjPlane.h
    src/ObjSceneObject.cpp
    src/ObjSceneObject.h
    src/ObjSphere.cpp
    src/ObjSphere.h
    src/ObjTriangle.cpp
    src/ObjTriangle.h
    src/ObjTube.cpp
    src/ObjTube.h
    src/Point.cpp
    src/Point.h
    src/Ray.cpp
    src/Ray.h
    src/Raytracer.cpp
    src/Raytracer.h
    src/SpeedTest.cpp
    src/SpeedTest.h
    src/StringProcessing.cpp
    src/StringProcessing.h
    src/Vector.cpp
    src/Vector.h
    src/VectorMath.cpp
    src/VectorMath.h src/StaticRandom.cpp src/StaticRandom.h)

add_executable(Naive_Raytracer ${SOURCE_FILES})