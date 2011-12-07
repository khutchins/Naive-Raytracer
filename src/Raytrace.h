#ifndef _RAYTRACE_MAIN_
#define _RAYTRACE_MAIN_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "EasyBMP\EasyBMP.h"
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
#include <limits>

using namespace std;

#define W 320
#define H 240

Plane *findClosestPlane(Ray *r, Point &pInt);
Sphere *findClosestSphere(Ray *r, Point &sInt);
Light *findClosestLight(Ray *r, Point &lInt);

Camera* makeCamera(ifstream &f);
Light* makeLight(ifstream &f);
Plane* makePlane(ifstream &f);
Sphere* makeSphere(ifstream &f);
Color* raytrace(Ray* r, bool &light);
int processInput(string filename);

#endif