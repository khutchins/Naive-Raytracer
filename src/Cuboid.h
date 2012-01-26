#ifndef _CUBOID_H_
#define _CUBOID_H_

#include "SceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Plane.h"
#include "Material.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>
#include <cmath>

class Cuboid : public SceneObject {
public:
	Plane* sides[6];

	Vector up;
	Vector front;
	Point origin;
	double height,width,length;

	Cuboid(ifstream &f);
	Vector calculateNormalForPoint(Point p, Point raySource);
	bool intersect(Ray* r, Point &intersect);
	double getReflection();
	double getRefraction();
	Color getColor();
};

#endif
