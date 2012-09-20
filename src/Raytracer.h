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

#include "ObjCamera.h"
#include "ObjCuboid.h"
#include "ObjCylinder.h"
#include "ObjDisk.h"
#include "ObjLight.h"
#include "ObjPlane.h"
#include "ObjSceneObject.h"
#include "ObjSphere.h"
#include "ObjTriangle.h"
#include "ObjTube.h"

#include "Color.h"
#include "Diagnostic.h"
#include "Material.h"
#include "Point.h"
#include "Ray.h"
#include "StringProcessing.h"
#include "Vector.h"

using namespace std;

class Camera;
class SceneObject;

class Raytracer {

public:
	Raytracer();
	~Raytracer();
	int start(string fn);
	Color raytrace(Ray* r, bool &lightWasSeen);

	void addSceneObject(SceneObject *o);
	void removeAllSceneObjects();

private:
	///Variables
	//Global camera and object queues
	queue<Camera*> cameraQueue;
	queue<SceneObject*> objectQueue;

	//maximum z value (currently ignored)
	double zmaxG;

	//current camera number
	int cameraNum;
	//number of iterations the ray has gone through (this is capped)
	int iterations;

	///Methods
	//Parse input file
	int processInput(string filename);

	//Lighting calculations
	Color calculateLocalLighting(Point intercept, Vector normal, EntityID id);
	Color calculateReflectedRay(Ray r, Point intercept, Vector normal, EntityID id);
	Color calculateRefractedRay(Ray r, Point intercept, Vector normal, EntityID id);
	Color calculateTextureOnPlaneFromMaterial(Plane* plane, Point intercept);

	//Object location
	SceneObject* findClosestObject(Ray *r, Point &oInt);
};

#endif