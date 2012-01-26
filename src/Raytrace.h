#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <fstream>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <string>
#include <string.h>
#include <queue>
#include "AntiAliasing.h"
#include "Camera.h"
#include "Color.h"
#include "Cuboid.h"
#include "Cylinder.h"
#include "Diagnostic.h"
#include "Disk.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Point.h"
#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "StringProcessing.h"
#include "Tube.h"
#include "Vector.h"

using namespace std;

Color calculateLocalLighting(Point intercept, Vector normal, EntityID id);
Color calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id);
Color calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id);
Color calculateTextureOnPlaneFromMaterial(Plane* plane, Point intercept);

SceneObject* findClosestObject(Ray *r, Point &oInt);

Color raytrace(Ray* r, bool &light);
int processInput(string filename);

#endif