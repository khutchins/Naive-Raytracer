#ifndef _PLANE_H_
#define _PLANE_H_

#include "SceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include <fstream>
#include <string>
#include <queue>
#include "EasyBMP\EasyBMP.h"

using namespace std;

class Plane : public SceneObject {
public:
    Vector normal;
    Vector up;
    double width;
    double height;
    Material material;
    BMP texture;

	Plane(std::ifstream &f);
	bool intersect(Ray* r, Point &intersect);
	Vector calculateNormalForPoint(Point p, Point raySource);
	Color calculateTextureFromMaterial(Point intercept);
	double getReflection();
	double getRefraction();
	Color getColor();
};

#endif