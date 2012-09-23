#ifndef _PLANE_H_
#define _PLANE_H_

#include "ObjSceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>
#include "EasyBMP/EasyBMP.h"

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
	Plane(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin);
	SceneObject* intersect(Ray* r, Point &intersect);
	Vector calculateNormalForPoint(Point p, Point raySource);
	Color calculateTextureFromMaterial(Point intercept);
	double getReflection();
	double getRefraction();
	Color getColor();

private:
	Point vertex1;
	Point vertex2;
	Point vertex3;
	Point vertex4;

	void sharedInit(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin);
};

#endif
