#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "Diagnostic.h"
#include "SceneObject.h"
#include "AntiAliasing.h"
#include "Sphere.h"
#include "Point.h"
#include "Material.h"
#include "Color.h"
#include "Camera.h"
#include "Vector.h"
#include "Ray.h"
#include "Plane.h"
#include "Light.h"
#include "StringProcessing.h"
#include <limits>

using namespace std;

Color calculateLocalLighting(Point intercept, Vector normal, EntityID id);
Color calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id);
Color calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id);
Color calculateTextureOnPlaneFromMaterial(Plane* plane, Point intercept);

SceneObject* findClosestObject(Ray *r, Point &oInt);

Color raytrace(Ray* r, bool &light);
int processInput(string filename);

#endif