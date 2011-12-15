#ifndef _DISK_H_
#define _DISK_H_

#include "SceneObject.h"
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

class Disk : public SceneObject {
public:
    Material material;
    double radius;
	Vector up;
	Vector normal;

	Disk(ifstream &f);
	Disk(Material m, double radius, Vector up, Vector normal, Point origin);
	Vector calculateNormalForPoint(Point p, Point raySource);
	bool intersect(Ray* r, Point &intersect);
	double getReflection();
	double getRefraction();
	Color getColor();
};

#endif