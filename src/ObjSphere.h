#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "ObjSceneObject.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Color.h"
#include "Vector.h"
#include "Point.h"
#include "VectorMath.h"
#include <cmath>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class Sphere : public SceneObject {
public:
    Material material;
    double radius;

	Sphere(ifstream &f);
	Vector calculateNormalForPoint(Point p, Point raySource);
	SceneObject* intersect(Ray* r, Point &intersect);
	double getReflection();
	double getRefraction();
	Color getColor();
};

#endif