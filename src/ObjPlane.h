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
	SceneObject* intersect(Ray* r, Point &intersect) override;
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	Color calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	Point vertex1;
	Point vertex2;
	Point vertex3;
	Point vertex4;
	Vector topLine;
	Vector leftLine;

	void sharedInit(Material m, double width, double height, Vector up, Vector normal, string textureName, Point origin);
};

#endif
