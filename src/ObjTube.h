#ifndef _TUBE_H_
#define _TUBE_H_

#include "ObjSceneObject.h"
#include "VectorMath.h"
#include "StringProcessing.h"
#include "Point.h"
#include "Material.h"
#include "Diagnostic.h"
#include <fstream>
#include <string>
#include <queue>
#include <cmath>

class Tube : public SceneObject {
public:
	double height;
	double radius;
	Material material;
	Vector up;

	explicit Tube(ifstream &f);
	Tube(Material m, double radius, double height, Vector up, Point origin);
	Vector calculateNormalForPoint(Point p, Point raySource) override;
	SceneObject* intersect(Ray* r, Point &intersect) override;
	double getReflection() override;
	double getRefraction() override;
	Color getColor() override;

private:
	void sharedInit(Material m, double radius, double height, Vector up, Point origin);
};

#endif
