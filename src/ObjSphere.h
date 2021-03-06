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
    double radius;
	BMP texture;

	explicit Sphere(ifstream &f);
	Sphere(Point origin, Material material, double radius, string textureName);
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	SceneObject* intersect(Ray* r, Point &intersect) override;
	Color calculateTextureFromMaterial(Point intercept, bool diagnosticEnabled) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	void sharedInit(Point origin, Material material, double radius, string textureName);
};

#endif