#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Color.h"
#include "Vector.h"
#include "Point.h"
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class Sphere {
public:
    Point center;
    Material material;
    double radius;

	Sphere(ifstream &f);
	Vector calculateNormalForPoint(Point p);
};

#endif