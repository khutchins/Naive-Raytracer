#ifndef _DISK_H_
#define _DISK_H_

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

class Disk : public SceneObject {
public:
    Material material;
    double radius;
	Vector up;
	Vector normal;

	explicit Disk(ifstream &f);
	Disk(Material m, double radius, Vector up, Vector normal, Point origin);
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	SceneObject* intersect(Ray* r, Point &intersect) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	void sharedInit(Material m, double radius, Vector up, Vector normal, Point origin);
};

#endif